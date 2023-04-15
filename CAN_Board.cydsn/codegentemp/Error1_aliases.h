/*******************************************************************************
* File Name: Error1.h  
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

#if !defined(CY_PINS_Error1_ALIASES_H) /* Pins Error1_ALIASES_H */
#define CY_PINS_Error1_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Error1_0			(Error1__0__PC)
#define Error1_0_PS		(Error1__0__PS)
#define Error1_0_PC		(Error1__0__PC)
#define Error1_0_DR		(Error1__0__DR)
#define Error1_0_SHIFT	(Error1__0__SHIFT)
#define Error1_0_INTR	((uint16)((uint16)0x0003u << (Error1__0__SHIFT*2u)))

#define Error1_INTR_ALL	 ((uint16)(Error1_0_INTR))


#endif /* End Pins Error1_ALIASES_H */


/* [] END OF FILE */
