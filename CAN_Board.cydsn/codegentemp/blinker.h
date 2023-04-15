/*******************************************************************************
* File Name: blinker.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the blinker
*  component.
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

#if !defined(CY_TCPWM_blinker_H)
#define CY_TCPWM_blinker_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} blinker_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  blinker_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define blinker_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define blinker_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define blinker_CONFIG                         (7lu)

/* Quad Mode */
/* Parameters */
#define blinker_QUAD_ENCODING_MODES            (0lu)
#define blinker_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define blinker_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define blinker_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define blinker_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define blinker_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define blinker_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define blinker_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define blinker_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define blinker_TC_RUN_MODE                    (0lu)
#define blinker_TC_COUNTER_MODE                (0lu)
#define blinker_TC_COMP_CAP_MODE               (2lu)
#define blinker_TC_PRESCALER                   (0lu)

/* Signal modes */
#define blinker_TC_RELOAD_SIGNAL_MODE          (0lu)
#define blinker_TC_COUNT_SIGNAL_MODE           (3lu)
#define blinker_TC_START_SIGNAL_MODE           (0lu)
#define blinker_TC_STOP_SIGNAL_MODE            (0lu)
#define blinker_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define blinker_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define blinker_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define blinker_TC_START_SIGNAL_PRESENT        (0lu)
#define blinker_TC_STOP_SIGNAL_PRESENT         (0lu)
#define blinker_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define blinker_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define blinker_PWM_KILL_EVENT                 (0lu)
#define blinker_PWM_STOP_EVENT                 (0lu)
#define blinker_PWM_MODE                       (4lu)
#define blinker_PWM_OUT_N_INVERT               (0lu)
#define blinker_PWM_OUT_INVERT                 (0lu)
#define blinker_PWM_ALIGN                      (0lu)
#define blinker_PWM_RUN_MODE                   (0lu)
#define blinker_PWM_DEAD_TIME_CYCLE            (0lu)
#define blinker_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define blinker_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define blinker_PWM_COUNT_SIGNAL_MODE          (3lu)
#define blinker_PWM_START_SIGNAL_MODE          (0lu)
#define blinker_PWM_STOP_SIGNAL_MODE           (0lu)
#define blinker_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define blinker_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define blinker_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define blinker_PWM_START_SIGNAL_PRESENT       (0lu)
#define blinker_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define blinker_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define blinker_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define blinker_TC_PERIOD_VALUE                (65535lu)
#define blinker_TC_COMPARE_VALUE               (65535lu)
#define blinker_TC_COMPARE_BUF_VALUE           (65535lu)
#define blinker_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define blinker_PWM_PERIOD_VALUE               (100lu)
#define blinker_PWM_PERIOD_BUF_VALUE           (65535lu)
#define blinker_PWM_PERIOD_SWAP                (0lu)
#define blinker_PWM_COMPARE_VALUE              (50lu)
#define blinker_PWM_COMPARE_BUF_VALUE          (65535lu)
#define blinker_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define blinker__LEFT 0
#define blinker__RIGHT 1
#define blinker__CENTER 2
#define blinker__ASYMMETRIC 3

#define blinker__X1 0
#define blinker__X2 1
#define blinker__X4 2

#define blinker__PWM 4
#define blinker__PWM_DT 5
#define blinker__PWM_PR 6

#define blinker__INVERSE 1
#define blinker__DIRECT 0

#define blinker__CAPTURE 2
#define blinker__COMPARE 0

#define blinker__TRIG_LEVEL 3
#define blinker__TRIG_RISING 0
#define blinker__TRIG_FALLING 1
#define blinker__TRIG_BOTH 2

#define blinker__INTR_MASK_TC 1
#define blinker__INTR_MASK_CC_MATCH 2
#define blinker__INTR_MASK_NONE 0
#define blinker__INTR_MASK_TC_CC 3

#define blinker__UNCONFIG 8
#define blinker__TIMER 1
#define blinker__QUAD 3
#define blinker__PWM_SEL 7

#define blinker__COUNT_UP 0
#define blinker__COUNT_DOWN 1
#define blinker__COUNT_UPDOWN0 2
#define blinker__COUNT_UPDOWN1 3


/* Prescaler */
#define blinker_PRESCALE_DIVBY1                ((uint32)(0u << blinker_PRESCALER_SHIFT))
#define blinker_PRESCALE_DIVBY2                ((uint32)(1u << blinker_PRESCALER_SHIFT))
#define blinker_PRESCALE_DIVBY4                ((uint32)(2u << blinker_PRESCALER_SHIFT))
#define blinker_PRESCALE_DIVBY8                ((uint32)(3u << blinker_PRESCALER_SHIFT))
#define blinker_PRESCALE_DIVBY16               ((uint32)(4u << blinker_PRESCALER_SHIFT))
#define blinker_PRESCALE_DIVBY32               ((uint32)(5u << blinker_PRESCALER_SHIFT))
#define blinker_PRESCALE_DIVBY64               ((uint32)(6u << blinker_PRESCALER_SHIFT))
#define blinker_PRESCALE_DIVBY128              ((uint32)(7u << blinker_PRESCALER_SHIFT))

/* TCPWM set modes */
#define blinker_MODE_TIMER_COMPARE             ((uint32)(blinker__COMPARE         <<  \
                                                                  blinker_MODE_SHIFT))
#define blinker_MODE_TIMER_CAPTURE             ((uint32)(blinker__CAPTURE         <<  \
                                                                  blinker_MODE_SHIFT))
#define blinker_MODE_QUAD                      ((uint32)(blinker__QUAD            <<  \
                                                                  blinker_MODE_SHIFT))
#define blinker_MODE_PWM                       ((uint32)(blinker__PWM             <<  \
                                                                  blinker_MODE_SHIFT))
#define blinker_MODE_PWM_DT                    ((uint32)(blinker__PWM_DT          <<  \
                                                                  blinker_MODE_SHIFT))
#define blinker_MODE_PWM_PR                    ((uint32)(blinker__PWM_PR          <<  \
                                                                  blinker_MODE_SHIFT))

/* Quad Modes */
#define blinker_MODE_X1                        ((uint32)(blinker__X1              <<  \
                                                                  blinker_QUAD_MODE_SHIFT))
#define blinker_MODE_X2                        ((uint32)(blinker__X2              <<  \
                                                                  blinker_QUAD_MODE_SHIFT))
#define blinker_MODE_X4                        ((uint32)(blinker__X4              <<  \
                                                                  blinker_QUAD_MODE_SHIFT))

/* Counter modes */
#define blinker_COUNT_UP                       ((uint32)(blinker__COUNT_UP        <<  \
                                                                  blinker_UPDOWN_SHIFT))
#define blinker_COUNT_DOWN                     ((uint32)(blinker__COUNT_DOWN      <<  \
                                                                  blinker_UPDOWN_SHIFT))
#define blinker_COUNT_UPDOWN0                  ((uint32)(blinker__COUNT_UPDOWN0   <<  \
                                                                  blinker_UPDOWN_SHIFT))
#define blinker_COUNT_UPDOWN1                  ((uint32)(blinker__COUNT_UPDOWN1   <<  \
                                                                  blinker_UPDOWN_SHIFT))

/* PWM output invert */
#define blinker_INVERT_LINE                    ((uint32)(blinker__INVERSE         <<  \
                                                                  blinker_INV_OUT_SHIFT))
#define blinker_INVERT_LINE_N                  ((uint32)(blinker__INVERSE         <<  \
                                                                  blinker_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define blinker_TRIG_RISING                    ((uint32)blinker__TRIG_RISING)
#define blinker_TRIG_FALLING                   ((uint32)blinker__TRIG_FALLING)
#define blinker_TRIG_BOTH                      ((uint32)blinker__TRIG_BOTH)
#define blinker_TRIG_LEVEL                     ((uint32)blinker__TRIG_LEVEL)

/* Interrupt mask */
#define blinker_INTR_MASK_TC                   ((uint32)blinker__INTR_MASK_TC)
#define blinker_INTR_MASK_CC_MATCH             ((uint32)blinker__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define blinker_CC_MATCH_SET                   (0x00u)
#define blinker_CC_MATCH_CLEAR                 (0x01u)
#define blinker_CC_MATCH_INVERT                (0x02u)
#define blinker_CC_MATCH_NO_CHANGE             (0x03u)
#define blinker_OVERLOW_SET                    (0x00u)
#define blinker_OVERLOW_CLEAR                  (0x04u)
#define blinker_OVERLOW_INVERT                 (0x08u)
#define blinker_OVERLOW_NO_CHANGE              (0x0Cu)
#define blinker_UNDERFLOW_SET                  (0x00u)
#define blinker_UNDERFLOW_CLEAR                (0x10u)
#define blinker_UNDERFLOW_INVERT               (0x20u)
#define blinker_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define blinker_PWM_MODE_LEFT                  (blinker_CC_MATCH_CLEAR        |   \
                                                         blinker_OVERLOW_SET           |   \
                                                         blinker_UNDERFLOW_NO_CHANGE)
#define blinker_PWM_MODE_RIGHT                 (blinker_CC_MATCH_SET          |   \
                                                         blinker_OVERLOW_NO_CHANGE     |   \
                                                         blinker_UNDERFLOW_CLEAR)
#define blinker_PWM_MODE_ASYM                  (blinker_CC_MATCH_INVERT       |   \
                                                         blinker_OVERLOW_SET           |   \
                                                         blinker_UNDERFLOW_CLEAR)

#if (blinker_CY_TCPWM_V2)
    #if(blinker_CY_TCPWM_4000)
        #define blinker_PWM_MODE_CENTER                (blinker_CC_MATCH_INVERT       |   \
                                                                 blinker_OVERLOW_NO_CHANGE     |   \
                                                                 blinker_UNDERFLOW_CLEAR)
    #else
        #define blinker_PWM_MODE_CENTER                (blinker_CC_MATCH_INVERT       |   \
                                                                 blinker_OVERLOW_SET           |   \
                                                                 blinker_UNDERFLOW_CLEAR)
    #endif /* (blinker_CY_TCPWM_4000) */
#else
    #define blinker_PWM_MODE_CENTER                (blinker_CC_MATCH_INVERT       |   \
                                                             blinker_OVERLOW_NO_CHANGE     |   \
                                                             blinker_UNDERFLOW_CLEAR)
#endif /* (blinker_CY_TCPWM_NEW) */

/* Command operations without condition */
#define blinker_CMD_CAPTURE                    (0u)
#define blinker_CMD_RELOAD                     (8u)
#define blinker_CMD_STOP                       (16u)
#define blinker_CMD_START                      (24u)

/* Status */
#define blinker_STATUS_DOWN                    (1u)
#define blinker_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   blinker_Init(void);
void   blinker_Enable(void);
void   blinker_Start(void);
void   blinker_Stop(void);

void   blinker_SetMode(uint32 mode);
void   blinker_SetCounterMode(uint32 counterMode);
void   blinker_SetPWMMode(uint32 modeMask);
void   blinker_SetQDMode(uint32 qdMode);

void   blinker_SetPrescaler(uint32 prescaler);
void   blinker_TriggerCommand(uint32 mask, uint32 command);
void   blinker_SetOneShot(uint32 oneShotEnable);
uint32 blinker_ReadStatus(void);

void   blinker_SetPWMSyncKill(uint32 syncKillEnable);
void   blinker_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   blinker_SetPWMDeadTime(uint32 deadTime);
void   blinker_SetPWMInvert(uint32 mask);

void   blinker_SetInterruptMode(uint32 interruptMask);
uint32 blinker_GetInterruptSourceMasked(void);
uint32 blinker_GetInterruptSource(void);
void   blinker_ClearInterrupt(uint32 interruptMask);
void   blinker_SetInterrupt(uint32 interruptMask);

void   blinker_WriteCounter(uint32 count);
uint32 blinker_ReadCounter(void);

uint32 blinker_ReadCapture(void);
uint32 blinker_ReadCaptureBuf(void);

void   blinker_WritePeriod(uint32 period);
uint32 blinker_ReadPeriod(void);
void   blinker_WritePeriodBuf(uint32 periodBuf);
uint32 blinker_ReadPeriodBuf(void);

void   blinker_WriteCompare(uint32 compare);
uint32 blinker_ReadCompare(void);
void   blinker_WriteCompareBuf(uint32 compareBuf);
uint32 blinker_ReadCompareBuf(void);

void   blinker_SetPeriodSwap(uint32 swapEnable);
void   blinker_SetCompareSwap(uint32 swapEnable);

void   blinker_SetCaptureMode(uint32 triggerMode);
void   blinker_SetReloadMode(uint32 triggerMode);
void   blinker_SetStartMode(uint32 triggerMode);
void   blinker_SetStopMode(uint32 triggerMode);
void   blinker_SetCountMode(uint32 triggerMode);

void   blinker_SaveConfig(void);
void   blinker_RestoreConfig(void);
void   blinker_Sleep(void);
void   blinker_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define blinker_BLOCK_CONTROL_REG              (*(reg32 *) blinker_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define blinker_BLOCK_CONTROL_PTR              ( (reg32 *) blinker_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define blinker_COMMAND_REG                    (*(reg32 *) blinker_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define blinker_COMMAND_PTR                    ( (reg32 *) blinker_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define blinker_INTRRUPT_CAUSE_REG             (*(reg32 *) blinker_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define blinker_INTRRUPT_CAUSE_PTR             ( (reg32 *) blinker_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define blinker_CONTROL_REG                    (*(reg32 *) blinker_cy_m0s8_tcpwm_1__CTRL )
#define blinker_CONTROL_PTR                    ( (reg32 *) blinker_cy_m0s8_tcpwm_1__CTRL )
#define blinker_STATUS_REG                     (*(reg32 *) blinker_cy_m0s8_tcpwm_1__STATUS )
#define blinker_STATUS_PTR                     ( (reg32 *) blinker_cy_m0s8_tcpwm_1__STATUS )
#define blinker_COUNTER_REG                    (*(reg32 *) blinker_cy_m0s8_tcpwm_1__COUNTER )
#define blinker_COUNTER_PTR                    ( (reg32 *) blinker_cy_m0s8_tcpwm_1__COUNTER )
#define blinker_COMP_CAP_REG                   (*(reg32 *) blinker_cy_m0s8_tcpwm_1__CC )
#define blinker_COMP_CAP_PTR                   ( (reg32 *) blinker_cy_m0s8_tcpwm_1__CC )
#define blinker_COMP_CAP_BUF_REG               (*(reg32 *) blinker_cy_m0s8_tcpwm_1__CC_BUFF )
#define blinker_COMP_CAP_BUF_PTR               ( (reg32 *) blinker_cy_m0s8_tcpwm_1__CC_BUFF )
#define blinker_PERIOD_REG                     (*(reg32 *) blinker_cy_m0s8_tcpwm_1__PERIOD )
#define blinker_PERIOD_PTR                     ( (reg32 *) blinker_cy_m0s8_tcpwm_1__PERIOD )
#define blinker_PERIOD_BUF_REG                 (*(reg32 *) blinker_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define blinker_PERIOD_BUF_PTR                 ( (reg32 *) blinker_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define blinker_TRIG_CONTROL0_REG              (*(reg32 *) blinker_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define blinker_TRIG_CONTROL0_PTR              ( (reg32 *) blinker_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define blinker_TRIG_CONTROL1_REG              (*(reg32 *) blinker_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define blinker_TRIG_CONTROL1_PTR              ( (reg32 *) blinker_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define blinker_TRIG_CONTROL2_REG              (*(reg32 *) blinker_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define blinker_TRIG_CONTROL2_PTR              ( (reg32 *) blinker_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define blinker_INTERRUPT_REQ_REG              (*(reg32 *) blinker_cy_m0s8_tcpwm_1__INTR )
#define blinker_INTERRUPT_REQ_PTR              ( (reg32 *) blinker_cy_m0s8_tcpwm_1__INTR )
#define blinker_INTERRUPT_SET_REG              (*(reg32 *) blinker_cy_m0s8_tcpwm_1__INTR_SET )
#define blinker_INTERRUPT_SET_PTR              ( (reg32 *) blinker_cy_m0s8_tcpwm_1__INTR_SET )
#define blinker_INTERRUPT_MASK_REG             (*(reg32 *) blinker_cy_m0s8_tcpwm_1__INTR_MASK )
#define blinker_INTERRUPT_MASK_PTR             ( (reg32 *) blinker_cy_m0s8_tcpwm_1__INTR_MASK )
#define blinker_INTERRUPT_MASKED_REG           (*(reg32 *) blinker_cy_m0s8_tcpwm_1__INTR_MASKED )
#define blinker_INTERRUPT_MASKED_PTR           ( (reg32 *) blinker_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define blinker_MASK                           ((uint32)blinker_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define blinker_RELOAD_CC_SHIFT                (0u)
#define blinker_RELOAD_PERIOD_SHIFT            (1u)
#define blinker_PWM_SYNC_KILL_SHIFT            (2u)
#define blinker_PWM_STOP_KILL_SHIFT            (3u)
#define blinker_PRESCALER_SHIFT                (8u)
#define blinker_UPDOWN_SHIFT                   (16u)
#define blinker_ONESHOT_SHIFT                  (18u)
#define blinker_QUAD_MODE_SHIFT                (20u)
#define blinker_INV_OUT_SHIFT                  (20u)
#define blinker_INV_COMPL_OUT_SHIFT            (21u)
#define blinker_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define blinker_RELOAD_CC_MASK                 ((uint32)(blinker_1BIT_MASK        <<  \
                                                                            blinker_RELOAD_CC_SHIFT))
#define blinker_RELOAD_PERIOD_MASK             ((uint32)(blinker_1BIT_MASK        <<  \
                                                                            blinker_RELOAD_PERIOD_SHIFT))
#define blinker_PWM_SYNC_KILL_MASK             ((uint32)(blinker_1BIT_MASK        <<  \
                                                                            blinker_PWM_SYNC_KILL_SHIFT))
#define blinker_PWM_STOP_KILL_MASK             ((uint32)(blinker_1BIT_MASK        <<  \
                                                                            blinker_PWM_STOP_KILL_SHIFT))
#define blinker_PRESCALER_MASK                 ((uint32)(blinker_8BIT_MASK        <<  \
                                                                            blinker_PRESCALER_SHIFT))
#define blinker_UPDOWN_MASK                    ((uint32)(blinker_2BIT_MASK        <<  \
                                                                            blinker_UPDOWN_SHIFT))
#define blinker_ONESHOT_MASK                   ((uint32)(blinker_1BIT_MASK        <<  \
                                                                            blinker_ONESHOT_SHIFT))
#define blinker_QUAD_MODE_MASK                 ((uint32)(blinker_3BIT_MASK        <<  \
                                                                            blinker_QUAD_MODE_SHIFT))
#define blinker_INV_OUT_MASK                   ((uint32)(blinker_2BIT_MASK        <<  \
                                                                            blinker_INV_OUT_SHIFT))
#define blinker_MODE_MASK                      ((uint32)(blinker_3BIT_MASK        <<  \
                                                                            blinker_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define blinker_CAPTURE_SHIFT                  (0u)
#define blinker_COUNT_SHIFT                    (2u)
#define blinker_RELOAD_SHIFT                   (4u)
#define blinker_STOP_SHIFT                     (6u)
#define blinker_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define blinker_CAPTURE_MASK                   ((uint32)(blinker_2BIT_MASK        <<  \
                                                                  blinker_CAPTURE_SHIFT))
#define blinker_COUNT_MASK                     ((uint32)(blinker_2BIT_MASK        <<  \
                                                                  blinker_COUNT_SHIFT))
#define blinker_RELOAD_MASK                    ((uint32)(blinker_2BIT_MASK        <<  \
                                                                  blinker_RELOAD_SHIFT))
#define blinker_STOP_MASK                      ((uint32)(blinker_2BIT_MASK        <<  \
                                                                  blinker_STOP_SHIFT))
#define blinker_START_MASK                     ((uint32)(blinker_2BIT_MASK        <<  \
                                                                  blinker_START_SHIFT))

/* MASK */
#define blinker_1BIT_MASK                      ((uint32)0x01u)
#define blinker_2BIT_MASK                      ((uint32)0x03u)
#define blinker_3BIT_MASK                      ((uint32)0x07u)
#define blinker_6BIT_MASK                      ((uint32)0x3Fu)
#define blinker_8BIT_MASK                      ((uint32)0xFFu)
#define blinker_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define blinker_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define blinker_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(blinker_QUAD_ENCODING_MODES     << blinker_QUAD_MODE_SHIFT))       |\
         ((uint32)(blinker_CONFIG                  << blinker_MODE_SHIFT)))

#define blinker_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(blinker_PWM_STOP_EVENT          << blinker_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(blinker_PWM_OUT_INVERT          << blinker_INV_OUT_SHIFT))         |\
         ((uint32)(blinker_PWM_OUT_N_INVERT        << blinker_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(blinker_PWM_MODE                << blinker_MODE_SHIFT)))

#define blinker_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(blinker_PWM_RUN_MODE         << blinker_ONESHOT_SHIFT))
            
#define blinker_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(blinker_PWM_ALIGN            << blinker_UPDOWN_SHIFT))

#define blinker_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(blinker_PWM_KILL_EVENT      << blinker_PWM_SYNC_KILL_SHIFT))

#define blinker_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(blinker_PWM_DEAD_TIME_CYCLE  << blinker_PRESCALER_SHIFT))

#define blinker_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(blinker_PWM_PRESCALER        << blinker_PRESCALER_SHIFT))

#define blinker_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(blinker_TC_PRESCALER            << blinker_PRESCALER_SHIFT))       |\
         ((uint32)(blinker_TC_COUNTER_MODE         << blinker_UPDOWN_SHIFT))          |\
         ((uint32)(blinker_TC_RUN_MODE             << blinker_ONESHOT_SHIFT))         |\
         ((uint32)(blinker_TC_COMP_CAP_MODE        << blinker_MODE_SHIFT)))
        
#define blinker_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(blinker_QUAD_PHIA_SIGNAL_MODE   << blinker_COUNT_SHIFT))           |\
         ((uint32)(blinker_QUAD_INDEX_SIGNAL_MODE  << blinker_RELOAD_SHIFT))          |\
         ((uint32)(blinker_QUAD_STOP_SIGNAL_MODE   << blinker_STOP_SHIFT))            |\
         ((uint32)(blinker_QUAD_PHIB_SIGNAL_MODE   << blinker_START_SHIFT)))

#define blinker_PWM_SIGNALS_MODES                                                              \
        (((uint32)(blinker_PWM_SWITCH_SIGNAL_MODE  << blinker_CAPTURE_SHIFT))         |\
         ((uint32)(blinker_PWM_COUNT_SIGNAL_MODE   << blinker_COUNT_SHIFT))           |\
         ((uint32)(blinker_PWM_RELOAD_SIGNAL_MODE  << blinker_RELOAD_SHIFT))          |\
         ((uint32)(blinker_PWM_STOP_SIGNAL_MODE    << blinker_STOP_SHIFT))            |\
         ((uint32)(blinker_PWM_START_SIGNAL_MODE   << blinker_START_SHIFT)))

#define blinker_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(blinker_TC_CAPTURE_SIGNAL_MODE  << blinker_CAPTURE_SHIFT))         |\
         ((uint32)(blinker_TC_COUNT_SIGNAL_MODE    << blinker_COUNT_SHIFT))           |\
         ((uint32)(blinker_TC_RELOAD_SIGNAL_MODE   << blinker_RELOAD_SHIFT))          |\
         ((uint32)(blinker_TC_STOP_SIGNAL_MODE     << blinker_STOP_SHIFT))            |\
         ((uint32)(blinker_TC_START_SIGNAL_MODE    << blinker_START_SHIFT)))
        
#define blinker_TIMER_UPDOWN_CNT_USED                                                          \
                ((blinker__COUNT_UPDOWN0 == blinker_TC_COUNTER_MODE)                  ||\
                 (blinker__COUNT_UPDOWN1 == blinker_TC_COUNTER_MODE))

#define blinker_PWM_UPDOWN_CNT_USED                                                            \
                ((blinker__CENTER == blinker_PWM_ALIGN)                               ||\
                 (blinker__ASYMMETRIC == blinker_PWM_ALIGN))               
        
#define blinker_PWM_PR_INIT_VALUE              (1u)
#define blinker_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_blinker_H */

/* [] END OF FILE */
