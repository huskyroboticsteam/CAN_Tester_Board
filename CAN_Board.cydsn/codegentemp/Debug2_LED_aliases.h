/*******************************************************************************
* File Name: Debug2_LED.h  
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

#if !defined(CY_PINS_Debug2_LED_ALIASES_H) /* Pins Debug2_LED_ALIASES_H */
#define CY_PINS_Debug2_LED_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Debug2_LED_0			(Debug2_LED__0__PC)
#define Debug2_LED_0_PS		(Debug2_LED__0__PS)
#define Debug2_LED_0_PC		(Debug2_LED__0__PC)
#define Debug2_LED_0_DR		(Debug2_LED__0__DR)
#define Debug2_LED_0_SHIFT	(Debug2_LED__0__SHIFT)
#define Debug2_LED_0_INTR	((uint16)((uint16)0x0003u << (Debug2_LED__0__SHIFT*2u)))

#define Debug2_LED_INTR_ALL	 ((uint16)(Debug2_LED_0_INTR))


#endif /* End Pins Debug2_LED_ALIASES_H */


/* [] END OF FILE */
