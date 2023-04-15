/*******************************************************************************
* File Name: blinker_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "blinker.h"

static blinker_BACKUP_STRUCT blinker_backup;


/*******************************************************************************
* Function Name: blinker_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: blinker_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void blinker_Sleep(void)
{
    if(0u != (blinker_BLOCK_CONTROL_REG & blinker_MASK))
    {
        blinker_backup.enableState = 1u;
    }
    else
    {
        blinker_backup.enableState = 0u;
    }

    blinker_Stop();
    blinker_SaveConfig();
}


/*******************************************************************************
* Function Name: blinker_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void blinker_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: blinker_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void blinker_Wakeup(void)
{
    blinker_RestoreConfig();

    if(0u != blinker_backup.enableState)
    {
        blinker_Enable();
    }
}


/* [] END OF FILE */
