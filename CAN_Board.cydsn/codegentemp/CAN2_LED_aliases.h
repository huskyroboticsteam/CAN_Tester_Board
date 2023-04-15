/*******************************************************************************
* File Name: CAN2_LED.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_CAN2_LED_ALIASES_H) /* Pins CAN2_LED_ALIASES_H */
#define CY_PINS_CAN2_LED_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define CAN2_LED_0			(CAN2_LED__0__PC)
#define CAN2_LED_0_PS		(CAN2_LED__0__PS)
#define CAN2_LED_0_PC		(CAN2_LED__0__PC)
#define CAN2_LED_0_DR		(CAN2_LED__0__DR)
#define CAN2_LED_0_SHIFT	(CAN2_LED__0__SHIFT)
#define CAN2_LED_0_INTR	((uint16)((uint16)0x0003u << (CAN2_LED__0__SHIFT*2u)))

#define CAN2_LED_INTR_ALL	 ((uint16)(CAN2_LED_0_INTR))


#endif /* End Pins CAN2_LED_ALIASES_H */


/* [] END OF FILE */
