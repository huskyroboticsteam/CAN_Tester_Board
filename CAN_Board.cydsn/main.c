/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "cytypes.h"
#include "cyfitter.h"
#include "cydevice_trm.h"
#include "main.h"

#define CHIP_TYPE_TEMPLATE              0x00
#define CHIP_TYPE_STM32Fxxx             0x01
#define CHIP_TYPE_PSOC_CY8C4248AZI_L485 0x02
#define CHIP_TYPE_AT90CANxxx            0x03

#define STATUS_MAILBOX0 0x1
#define STATUS_MAILBOX1 0x2
#define STATUS_MAILBOX2 0x4
#define STATUS_MAILBOX3 0x8
#define STATUS_MAILBOX4 0x10
#define STATUS_MAILBOX5 0x20

#define CAN_RX_MAILBOX_0_SHIFT      (1u)
#define CAN_RX_MAILBOX_1_SHIFT      (2u)

//Error code constants
#define ERROR_NONE              0x00
#define ERROR_GENERIC_ERROR     0x01
#define ERROR_NULL_POINTER      0x02 

char8 uart_tx[100];
char8 uart_rx[100];

volatile uint8_t messagePresentFlag = 0xFF;
volatile uint8_t messageReadFlag = 0x0;
uint8_t FIFOSize(void);
void countAddFIFO(void);
void countRemoveFIFO(void);

#define FIFO_SIZE   16
volatile CANPacket latestMessage[FIFO_SIZE];//internal to this port acts like a FIFO with 10 packet storage
volatile uint8_t latestMessageHead = 0; //which index to read
volatile uint8_t latestMessageTail = 0; //which index to write to next
volatile uint8_t latestMessageFull = 0; //FIFO is full

CY_ISR_PROTO(CAN_FLAG_ISR);

int deviceAddress;
int deviceGroup;
CAN_RX_CFG rxMailbox;

int mailbox = 0;

CANPacket packet;

CY_ISR_PROTO(ISR_CAN);

CAN_TX_MSG messagePWM;
volatile uint8 receiveMailboxNumber = 0xFFu;

volatile uint16 CAN_time_LED = 0;

int uart_rx_len = 0;


CANPacket known[100];
int known_counter[100];
int known_n = 0;

int main(void)
{
    CyGlobalIntEnable;
    char8 txData[8];
    
    InitCAN();
    
    UART_Start();
    UART_UartPutString("startup\n\r");
    blinker_Start();
    
    UART_UartPutString("format:\r\n");
    UART_UartPutString("XXX XX XX XX XX XX XX XX XX XX\r\n");
    UART_UartPutString(" |  |  |  |  |  |  |  |  |  |\r\n");
    UART_UartPutString(" id |  |  |  |  |  |  |  |  |\r\n");
    UART_UartPutString("   dlc |  |  |  |  |  |  |  |\r\n");
    UART_UartPutString("       0  1  2  3  4  5  6  7 (data)\r\n");
    for(;;)
    {
        if (CAN_time_LED > 0) {
            CAN1_LED_Write(0);
            CAN_time_LED--;
        } else {
            CAN1_LED_Write(1);
        }
            
        uint32 c = UART_UartGetChar();
        if (c) {
            if (c == '\r') {
                CANPacket p;
                parseLine(&p, uart_rx+1, uart_rx_len-1);
                if (SendCANPacket(&p) == 0) {
                    UART_UartPutString("sent ");
                    sprintf(uart_tx,"%03X %02X ",p.id,p.dlc);
                    UART_UartPutString(uart_tx);
                    for (int i = 0; i < p.dlc; i++){
                        sprintf(uart_tx,"%02X ",p.data[i]);
                        UART_UartPutString(uart_tx);
                    }
                    UART_UartPutCRLF(0);
                    uart_rx_len = 0;
                } else {
                    UART_UartPutString("Epic FAIL\r\n");
                }
            } else {
                uart_rx[uart_rx_len] = c;
                uart_rx_len++;
            }
        }
        
        if (PollAndReceiveCANPacket(&packet) == ERROR_NONE) {
            sprintf(uart_tx, "ID:%03X Type:%02X\r\n", packet.id, packet.data[0]);
            UART_UartPutString(uart_tx);
            for (int i = 0; i < packet.dlc; i++){
               sprintf(uart_tx,"%02X ",packet.data[i]);
               UART_UartPutString(uart_tx);
            }
            UART_UartPutString("\r\n");
            
            //printPacket(&packet);
            UART_UartPutString("\r\n");
        }
    }
}

int SendCANPacket(CANPacket *packetToSend) {
    if(!packetToSend) {return ERROR_NULL_POINTER;}
    CAN_TX_MSG PSoCMessage;//CAN structure
    CAN_DATA_BYTES_MSG PSoCData;//CAN data
    PSoCMessage.id = packetToSend->id;
    PSoCMessage.rtr = 0x0;
    PSoCMessage.ide = 0x0;//Not extended
    PSoCMessage.dlc = packetToSend->dlc;
    PSoCMessage.irq = 0x0;
    PSoCMessage.msg = &PSoCData;
    
    memcpy(PSoCData.byte, packetToSend->data, 8);

    if(CAN_SendMsg(&PSoCMessage) == CYRET_SUCCESS) {
        return ERROR_NONE;
    } else
    {
        return ERROR_GENERIC_ERROR;
    }
}

void InitCAN() {
    
    CAN_Start();//must name CAN Top Design block as "CAN"
    
    //sets up inidvidual recieve mailbox (3rd priority mailbox)
    rxMailbox.rxmailbox = 0;
    rxMailbox.rxacr = 0x00000000;
    rxMailbox.rxamr = 0xFFFFFFFF;
    rxMailbox.rxcmd = CAN_RX_CMD_REG(CAN_RX_MAILBOX_0);//need to know what this is
    CAN_RxBufConfig(&rxMailbox);
    
    CAN_GlobalIntEnable();
    CyIntSetVector(CAN_ISR_NUMBER, CAN_FLAG_ISR);
}

CY_ISR(CAN_FLAG_ISR)
{
    CAN_time_LED = 1000;
    CAN_INT_SR_REG = CAN_RX_MESSAGE_MASK;
    uint32_t statusReg = (uint32_t) CAN_BUF_SR_REG; //Hardcoded for speed, translation from reg
    uint8_t mailbox = CAN_RX_MAILBOX_0;
    
    latestMessage[latestMessageTail].id = CAN_GET_RX_ID(mailbox);
    latestMessage[latestMessageTail].dlc = CAN_GET_DLC(mailbox);
    latestMessage[latestMessageTail].data[0] = CAN_RX_DATA_BYTE1(mailbox);
    latestMessage[latestMessageTail].data[1] = CAN_RX_DATA_BYTE2(mailbox);
    latestMessage[latestMessageTail].data[2] = CAN_RX_DATA_BYTE3(mailbox);
    latestMessage[latestMessageTail].data[3] = CAN_RX_DATA_BYTE4(mailbox);
    latestMessage[latestMessageTail].data[4] = CAN_RX_DATA_BYTE5(mailbox);
    latestMessage[latestMessageTail].data[5] = CAN_RX_DATA_BYTE6(mailbox);
    latestMessage[latestMessageTail].data[6] = CAN_RX_DATA_BYTE7(mailbox);
    latestMessage[latestMessageTail].data[7] = CAN_RX_DATA_BYTE8(mailbox);
    countAddFIFO();

    //CAN_ReceiveMsg(messagePresentFlag);
    CAN_RX_ACK_MESSAGE(mailbox);
}

int PollAndReceiveCANPacket(CANPacket *receivedPacket) {
    if(!receivedPacket)
        return ERROR_NULL_POINTER;
    
    volatile uint8_t size = FIFOSize();
    if(size)
    {
        *(receivedPacket) = latestMessage[latestMessageHead];
        countRemoveFIFO();
        return ERROR_NONE;
    }
    
    return 0x02; //No message received error
}


//helper function calculate size of Fifo
uint8_t FIFOSize() {
    if(latestMessageFull) {
        return FIFO_SIZE;
    }
    else if(latestMessageHead < latestMessageTail) {
        return latestMessageTail - latestMessageHead;
    }
    else if(latestMessageHead > latestMessageTail) {
        return (FIFO_SIZE - latestMessageHead) + latestMessageTail;
    }
    else { // latestMessageHead == latestMessageTail && !latestMessageFull
        return 0;
    }
}

void countAddFIFO() { 
    latestMessageTail++;
    if(latestMessageTail >= FIFO_SIZE){
        latestMessageTail = 0;
    }
    if(latestMessageFull) {
        latestMessageHead++;
        if(latestMessageHead >= FIFO_SIZE) {
            latestMessageHead = 0;
        } 
    }
    latestMessageFull = (latestMessageHead == latestMessageTail);
}

void countRemoveFIFO() {
    if(FIFOSize() > 0) {
        latestMessageHead++;
        if(latestMessageHead >= FIFO_SIZE) {
            latestMessageHead = 0;
        }
    }
}

uint8 s2x(char8 c) {
    switch(c) {
        case '1': return 1; case '2': return 2;
        case '3': return 3; case '4': return 4;
        case '5': return 5; case '6': return 6;
        case '7': return 7; case '8': return 8;
        case '9': return 9; case 'A': return 10;
        case 'B': return 11;case 'C': return 12;
        case 'D': return 13;case 'E': return 14;
        case 'F': return 15;case 'a': return 10;
        case 'b': return 11;case 'c': return 12;
        case 'd': return 13;case 'e': return 14;
        case 'f': return 15;default:  return 0;
    }
}

void parseLine(CANPacket* p, char8 line[], int length) {
    uint16 pr = (uint16) s2x(line[0]) << 10;
    uint16 dg = (uint16) s2x(line[2]) << 6;
    uint16 sn = (uint16) s2x(line[4]) << 4 | s2x(line[5]);
    p->id = (pr | dg | sn);
    
    int i = 0;
    int j = 8;
    while (j < length && i < 8) {
        p->data[i] = (s2x(line[j-1])<<4) | s2x(line[j]);
        j += 3;
        i++;
    }
    p->dlc = i;
}

uint32_t decodeFromBytes(int msb_index, int lsb_index, uint8_t data[]) {
    uint32_t result = 0;
    if (msb_index < lsb_index)
        for (int i = msb_index; i <= lsb_index; i++)
            result = result << 8 | data[i];
    else
        for (int i = msb_index; i >= lsb_index; i--)
            result = (result | data[i]) << 8;
    return result;
}

void printPacket(CANPacket* packet) {
    char8 info[100];
    uint8_t packetID = packet->data[0];
    sprintf(info,"id: %#02X ",packetID);
    UART_UartPutString(info);
    switch(packetID) {
        case 0x03: // PWM Set
            sprintf(info,"pwm_val: %i",(int16_t) decodeFromBytes(1,2,packet->data));
            break;
        case 0xF6: // Telemetry Report
            sprintf(info,"SN: %#02X TTC: %#02X data: %i",packet->data[1],packet->data[2],(int32_t) decodeFromBytes(3,6,packet->data));
            break;
        case 0x09:
            sprintf(info,"SN: %#02X val: %i LIMIT##########################################################",packet->data[1],packet->data[2]);
            break;
        default:
            sprintf(info,"not recognized");
    }
    UART_UartPutString(info);
    UART_UartPutString("\r\n");
}


/* [] END OF FILE */
