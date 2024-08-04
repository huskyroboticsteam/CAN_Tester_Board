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

#include "CAN_Stuff.h"
#include <stdio.h>

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

volatile uint8 receiveMailboxNumber = 0xFFu;

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

void InitCAN() {
    CAN_Start(); // must name CAN Top Design block as "CAN"
    
    // sets up mailbox to recieve EVERYTHING
    rxMailbox.rxmailbox = 0;
    rxMailbox.rxacr = 0x00000000;
    rxMailbox.rxamr = 0xFFFFFFFF;
    rxMailbox.rxcmd = CAN_RX_CMD_REG(CAN_RX_MAILBOX_0);
    CAN_RxBufConfig(&rxMailbox);
    
    CAN_GlobalIntEnable();
    CyIntSetVector(CAN_ISR_NUMBER, CAN_FLAG_ISR);
}

CY_ISR(CAN_FLAG_ISR)
{
    CAN_INT_SR_REG = CAN_RX_MESSAGE_MASK;
    // uint32_t statusReg = (uint32_t) CAN_BUF_SR_REG; //Hardcoded for speed, translation from reg
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

void sprintCANPacket(CANPacket* packet, char* buffer) {
    uint8 pri = (packet->id & 0x400) >> 10;
    uint8 dg = (packet->id & 0x3C0) >> 6;
    uint8 sn = (packet->id & 0x03F) >> 0;

    int len = sprintf(buffer, "%01X %02X %02X ", pri, dg, sn);
    for(int i = 0; i < packet->dlc; i++)
        len += sprintf(buffer+len," %02X", packet->data[i]);

    sprintf(buffer+len,"\r\n");
}

/* [] END OF FILE */
