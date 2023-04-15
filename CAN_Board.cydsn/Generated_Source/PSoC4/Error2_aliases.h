/*******************************************************************************
* File Name: Error2.h  
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

#if !defined(CY_PINS_Error2_ALIASES_H) /* Pins Error2_ALIASES_H */
#define CY_PINS_Error2_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Error2_0			(Error2__0__PC)
#define Error2_0_PS		(Error2__0__PS)
#define Error2_0_PC		(Error2__0__PC)
#define Error2_0_DR		(Error2__0__DR)
#define Error2_0_SHIFT	(Error2__0__SHIFT)
#define Error2_0_INTR	((uint16)((uint16)0x0003u << (Error2__0__SHIFT*2u)))

#define Error2_INTR_ALL	 ((uint16)(Error2_0_INTR))


#endif /* End Pins Error2_ALIASES_H */


/* [] END OF FILE */
