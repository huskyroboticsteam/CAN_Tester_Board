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

typedef struct {
    uint16_t id;
    uint8_t dlc;
    uint8_t data[8];
} CANPacket;

void InitCAN();
int SendCANPacket(CANPacket *packetToSend);
int PollAndReceiveCANPacket(CANPacket *receivedPacket);
uint8_t FIFOSize();
void countAddFIFO();
void countRemoveFIFO();
uint8 s2x(char8 c);
void parseLine(CANPacket* p, char8 line[], int length);
uint32_t decodeFromBytes(int msb_index, int lsb_index, uint8_t data[]);
void printPacket(CANPacket* packet);

/* [] END OF FILE */
