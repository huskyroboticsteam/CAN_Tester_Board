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
#define CHIP_TYPE_TEMPLATE              0x00
#define CHIP_TYPE_STM32Fxxx             0x01
#define CHIP_TYPE_PSOC_CY8C4248AZI_L485 0x02
#define CHIP_TYPE_AT90CANxxx            0x03

//Error code constants
#define ERROR_NONE              0x00
#define ERROR_GENERIC_ERROR     0x01
#define ERROR_NULL_POINTER      0x02 

#include "project.h"
#include "cytypes.h"
#include "cyfitter.h"
#include "cydevice_trm.h"


#define CAN_RX_MAILBOX_0_SHIFT      (1u)
#define CAN_RX_MAILBOX_1_SHIFT      (2u)
 
char8 uart_tx[100];
char8 uart_rx[100];

int uart_rx_len = 0;

volatile uint16 CAN_time_LED = 0;

typedef struct
{
    uint16_t id;
    uint8_t dlc;
    uint8_t data[8];
} CANPacket;

CANPacket known[100];
int known_counter[100];
int known_n = 0;

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

#define STATUS_MAILBOX0 0x1
#define STATUS_MAILBOX1 0x2
#define STATUS_MAILBOX2 0x4
#define STATUS_MAILBOX3 0x8
#define STATUS_MAILBOX4 0x10
#define STATUS_MAILBOX5 0x20


CY_ISR_PROTO(CAN_FLAG_ISR);

int deviceAddress;
int deviceGroup;
CAN_RX_CFG rxMailbox;

int mailbox = 0;

volatile CANPacket packet;

CY_ISR_PROTO(ISR_CAN);

CAN_TX_MSG messagePWM;
volatile uint8 receiveMailboxNumber = 0xFFu;
    
// CY_ISR(ISR_CAN) {
    // CyDelay(10000);
    // UART_UartPutString("packet");
    // if (CAN_GetRxMessage(&message) == CYRET_SUCCESS)
// }

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

CANPacket parseLine(CANPacket* p, char8 line[]) {
    uint16 pr = (uint16) s2x(line[0]) << 10;
    uint16 dg = (uint16) s2x(line[2]) << 6;
    uint16 sn = (uint16) s2x(line[4]) << 4 | s2x(line[5]);
    p->id = (pr | dg | sn);
    p->dlc = 8;
    
    for (uint8 i = 0; i < 8; i++) {
        p->data[i] = i < p->dlc ? 
                    (s2x(line[3*i+7])<<4) | s2x(line[3*i+8]) : 0;
    }
}

int SendCANPacket(CANPacket *packetToSend)
{
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
CAN_RX_CFG rxMailbox;
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
                parseLine(&p, uart_rx+1);
                SendCANPacket(&p);
                UART_UartPutString("sent ");
                sprintf(uart_tx,"%03X %02X ",p.id,p.dlc);
                UART_UartPutString(uart_tx);
                for (int i = 0; i < 8; i++){
                    sprintf(uart_tx,"%02X ",p.data[i]);
                    UART_UartPutString(uart_tx);
                }
                UART_UartPutCRLF(0);
                uart_rx_len = 0;
            } else {
                uart_rx[uart_rx_len] = c;
                uart_rx_len++;
            }
        }
        
        if (PollAndReceiveCANPacket(&packet) == ERROR_NONE) {
            sprintf(uart_tx,"%03X %02X ",packet.id,packet.dlc);
            UART_UartPutString(uart_tx);
            for (int i = 0; i < 8; i++){
                sprintf(uart_tx,"%02X ",packet.data[i]);
                UART_UartPutString(uart_tx);
            }
            UART_UartPutString("\r\n");
            //printPacket(&packet);
            //UART_UartPutString("\r\n");
            
        }
    }
}
void InitCAN() {
    
    CAN_Start();//must name CAN Top Design block as "CAN"
    
    //TODO: I'm sure there's a better way of doing this part
    // deviceGroup = deviceGroupInput & 0xF; // 4bits of ID
    // deviceAddress = deviceAddressInput & (0x3f);//6bits of ID
    
    //sets up inidvidual recieve mailbox (3rd priority mailbox)
    rxMailbox.rxmailbox = 0;
    // rxMailbox.rxacr = ((deviceGroup << 6)|(deviceAddress)) << 21;  // first 11 bits are the CAN ID that is not extended
    rxMailbox.rxamr = 0x801FFFFF; // what bits to ignore
    rxMailbox.rxcmd = CAN_RX_CMD_REG(CAN_RX_MAILBOX_0);//need to know what this is
    CAN_RxBufConfig(&rxMailbox);
    
    //setup broadcast recieve mailbox (1st priority mailbox)
    rxMailbox.rxmailbox = 1;
    rxMailbox.rxacr = ((0x0 << 6)|(0x0)) << 21; //0x20F<<21; // first 11 bits are the CAN ID that is not extended
    rxMailbox.rxamr = 0x801FFFFF; // what bits to ignore
    rxMailbox.rxcmd = CAN_RX_CMD_REG(CAN_RX_MAILBOX_1);//need to know what this is
    CAN_RxBufConfig(&rxMailbox);
    
    //setup group broadcast recieve mailbox (2nd priority mailbox)
    rxMailbox.rxmailbox = 2;
    // rxMailbox.rxacr = ((deviceGroup << 6)|(0x3F)) << 21; //0x20F<<21; // first 11 bits are the CAN ID that is not extended
    rxMailbox.rxamr = 0x801FFFFF; // what bits to ignore
    rxMailbox.rxcmd = CAN_RX_CMD_REG(CAN_RX_MAILBOX_2);//need to know what this is
    //CAN_RxBufConfig(&rxMailbox);
    
    CAN_GlobalIntEnable();
    CyIntSetVector(CAN_ISR_NUMBER, CAN_FLAG_ISR);
}

CY_ISR(CAN_FLAG_ISR)
{
    CAN_time_LED = 1000;
    //*(reg32*)0x402F0000 = CAN_RX_MESSAGE_MASK & CAN_SST_FAILURE_MASK & CAN_CRC_ERROR_MASK; //Clear Receive Message flag
    CAN_INT_SR_REG = CAN_RX_MESSAGE_MASK;
    uint32_t statusReg = (uint32_t) CAN_BUF_SR_REG; //Hardcoded for speed, translation from reg
    uint8_t mailbox;
    
    if(statusReg & 0b1) { // mailbox0 is full (individual)
        mailbox = CAN_RX_MAILBOX_0;
    }
    else if(statusReg & 0b10) { // mailbox1 is full (broadcast)
        mailbox = CAN_RX_MAILBOX_1;
    }    
    else if(statusReg & 0b100) { // mailbox2 is full (group broadcast)
        mailbox = CAN_RX_MAILBOX_2;
    } 
    else if(statusReg & 0b1000) { // mailbox3 is full currently recieves anything enable in top design 
        mailbox = CAN_RX_MAILBOX_3;
    }
    
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

int PollAndReceiveCANPacket(CANPacket *receivedPacket)
{
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
uint8_t FIFOSize(){
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

void countAddFIFO(){ 
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

void countRemoveFIFO(){
    if(FIFOSize() > 0) {
        latestMessageHead++;
        if(latestMessageHead >= FIFO_SIZE) {
            latestMessageHead = 0;
        }
    }
}

/* [] END OF FILE */
