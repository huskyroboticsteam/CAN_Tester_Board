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

#include "project.h"
#include "cytypes.h"
#include "cyfitter.h"

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

typedef struct {
    uint16_t id;
    uint8_t dlc;
    uint8_t data[8];
} CANPacket;

int SendCANPacket(CANPacket *packetToSend);

void InitCAN();
int SendCANPacket(CANPacket *packetToSend);
int PollAndReceiveCANPacket(CANPacket *receivedPacket);
uint8_t FIFOSize();
void countAddFIFO();
void countRemoveFIFO();
void sprintCANPacket(CANPacket* packet, char* buffer);

/* [] END OF FILE */
