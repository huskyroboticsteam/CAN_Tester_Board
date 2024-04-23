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
#include "CAN_Stuff.h"

char8 uart_tx[100];
char8 uart_rx[100];

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
