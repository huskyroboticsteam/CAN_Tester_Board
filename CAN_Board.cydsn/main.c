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
#include "cydevice_trm.h"
#include "UART_interface.c"
#include "CAN_Stuff.h"

#define CHIP_TYPE_TEMPLATE              0x00
#define CHIP_TYPE_STM32Fxxx             0x01
#define CHIP_TYPE_PSOC_CY8C4248AZI_L485 0x02
#define CHIP_TYPE_AT90CANxxx            0x03

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


/* [] END OF FILE */
