/*******************************************************************************
* File Name: blinker.c
* Version 2.10
*
* Description:
*  This file provides the source code to the API for the blinker
*  component
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

uint8 blinker_initVar = 0u;


/*******************************************************************************
* Function Name: blinker_Init
********************************************************************************
*
* Summary:
*  Initialize/Restore default blinker configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void blinker_Init(void)
{

    /* Set values from customizer to CTRL */
    #if (blinker__QUAD == blinker_CONFIG)
        blinker_CONTROL_REG = blinker_CTRL_QUAD_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        blinker_TRIG_CONTROL1_REG  = blinker_QUAD_SIGNALS_MODES;

        /* Set values from customizer to INTR */
        blinker_SetInterruptMode(blinker_QUAD_INTERRUPT_MASK);
        
         /* Set other values */
        blinker_SetCounterMode(blinker_COUNT_DOWN);
        blinker_WritePeriod(blinker_QUAD_PERIOD_INIT_VALUE);
        blinker_WriteCounter(blinker_QUAD_PERIOD_INIT_VALUE);
    #endif  /* (blinker__QUAD == blinker_CONFIG) */

    #if (blinker__TIMER == blinker_CONFIG)
        blinker_CONTROL_REG = blinker_CTRL_TIMER_BASE_CONFIG;
        
        /* Set values from customizer to CTRL1 */
        blinker_TRIG_CONTROL1_REG  = blinker_TIMER_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        blinker_SetInterruptMode(blinker_TC_INTERRUPT_MASK);
        
        /* Set other values from customizer */
        blinker_WritePeriod(blinker_TC_PERIOD_VALUE );

        #if (blinker__COMPARE == blinker_TC_COMP_CAP_MODE)
            blinker_WriteCompare(blinker_TC_COMPARE_VALUE);

            #if (1u == blinker_TC_COMPARE_SWAP)
                blinker_SetCompareSwap(1u);
                blinker_WriteCompareBuf(blinker_TC_COMPARE_BUF_VALUE);
            #endif  /* (1u == blinker_TC_COMPARE_SWAP) */
        #endif  /* (blinker__COMPARE == blinker_TC_COMP_CAP_MODE) */

        /* Initialize counter value */
        #if (blinker_CY_TCPWM_V2 && blinker_TIMER_UPDOWN_CNT_USED && !blinker_CY_TCPWM_4000)
            blinker_WriteCounter(1u);
        #elif(blinker__COUNT_DOWN == blinker_TC_COUNTER_MODE)
            blinker_WriteCounter(blinker_TC_PERIOD_VALUE);
        #else
            blinker_WriteCounter(0u);
        #endif /* (blinker_CY_TCPWM_V2 && blinker_TIMER_UPDOWN_CNT_USED && !blinker_CY_TCPWM_4000) */
    #endif  /* (blinker__TIMER == blinker_CONFIG) */

    #if (blinker__PWM_SEL == blinker_CONFIG)
        blinker_CONTROL_REG = blinker_CTRL_PWM_BASE_CONFIG;

        #if (blinker__PWM_PR == blinker_PWM_MODE)
            blinker_CONTROL_REG |= blinker_CTRL_PWM_RUN_MODE;
            blinker_WriteCounter(blinker_PWM_PR_INIT_VALUE);
        #else
            blinker_CONTROL_REG |= blinker_CTRL_PWM_ALIGN | blinker_CTRL_PWM_KILL_EVENT;
            
            /* Initialize counter value */
            #if (blinker_CY_TCPWM_V2 && blinker_PWM_UPDOWN_CNT_USED && !blinker_CY_TCPWM_4000)
                blinker_WriteCounter(1u);
            #elif (blinker__RIGHT == blinker_PWM_ALIGN)
                blinker_WriteCounter(blinker_PWM_PERIOD_VALUE);
            #else 
                blinker_WriteCounter(0u);
            #endif  /* (blinker_CY_TCPWM_V2 && blinker_PWM_UPDOWN_CNT_USED && !blinker_CY_TCPWM_4000) */
        #endif  /* (blinker__PWM_PR == blinker_PWM_MODE) */

        #if (blinker__PWM_DT == blinker_PWM_MODE)
            blinker_CONTROL_REG |= blinker_CTRL_PWM_DEAD_TIME_CYCLE;
        #endif  /* (blinker__PWM_DT == blinker_PWM_MODE) */

        #if (blinker__PWM == blinker_PWM_MODE)
            blinker_CONTROL_REG |= blinker_CTRL_PWM_PRESCALER;
        #endif  /* (blinker__PWM == blinker_PWM_MODE) */

        /* Set values from customizer to CTRL1 */
        blinker_TRIG_CONTROL1_REG  = blinker_PWM_SIGNALS_MODES;
    
        /* Set values from customizer to INTR */
        blinker_SetInterruptMode(blinker_PWM_INTERRUPT_MASK);

        /* Set values from customizer to CTRL2 */
        #if (blinker__PWM_PR == blinker_PWM_MODE)
            blinker_TRIG_CONTROL2_REG =
                    (blinker_CC_MATCH_NO_CHANGE    |
                    blinker_OVERLOW_NO_CHANGE      |
                    blinker_UNDERFLOW_NO_CHANGE);
        #else
            #if (blinker__LEFT == blinker_PWM_ALIGN)
                blinker_TRIG_CONTROL2_REG = blinker_PWM_MODE_LEFT;
            #endif  /* ( blinker_PWM_LEFT == blinker_PWM_ALIGN) */

            #if (blinker__RIGHT == blinker_PWM_ALIGN)
                blinker_TRIG_CONTROL2_REG = blinker_PWM_MODE_RIGHT;
            #endif  /* ( blinker_PWM_RIGHT == blinker_PWM_ALIGN) */

            #if (blinker__CENTER == blinker_PWM_ALIGN)
                blinker_TRIG_CONTROL2_REG = blinker_PWM_MODE_CENTER;
            #endif  /* ( blinker_PWM_CENTER == blinker_PWM_ALIGN) */

            #if (blinker__ASYMMETRIC == blinker_PWM_ALIGN)
                blinker_TRIG_CONTROL2_REG = blinker_PWM_MODE_ASYM;
            #endif  /* (blinker__ASYMMETRIC == blinker_PWM_ALIGN) */
        #endif  /* (blinker__PWM_PR == blinker_PWM_MODE) */

        /* Set other values from customizer */
        blinker_WritePeriod(blinker_PWM_PERIOD_VALUE );
        blinker_WriteCompare(blinker_PWM_COMPARE_VALUE);

        #if (1u == blinker_PWM_COMPARE_SWAP)
            blinker_SetCompareSwap(1u);
            blinker_WriteCompareBuf(blinker_PWM_COMPARE_BUF_VALUE);
        #endif  /* (1u == blinker_PWM_COMPARE_SWAP) */

        #if (1u == blinker_PWM_PERIOD_SWAP)
            blinker_SetPeriodSwap(1u);
            blinker_WritePeriodBuf(blinker_PWM_PERIOD_BUF_VALUE);
        #endif  /* (1u == blinker_PWM_PERIOD_SWAP) */
    #endif  /* (blinker__PWM_SEL == blinker_CONFIG) */
    
}


/*******************************************************************************
* Function Name: blinker_Enable
********************************************************************************
*
* Summary:
*  Enables the blinker.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void blinker_Enable(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    blinker_BLOCK_CONTROL_REG |= blinker_MASK;
    CyExitCriticalSection(enableInterrupts);

    /* Start Timer or PWM if start input is absent */
    #if (blinker__PWM_SEL == blinker_CONFIG)
        #if (0u == blinker_PWM_START_SIGNAL_PRESENT)
            blinker_TriggerCommand(blinker_MASK, blinker_CMD_START);
        #endif /* (0u == blinker_PWM_START_SIGNAL_PRESENT) */
    #endif /* (blinker__PWM_SEL == blinker_CONFIG) */

    #if (blinker__TIMER == blinker_CONFIG)
        #if (0u == blinker_TC_START_SIGNAL_PRESENT)
            blinker_TriggerCommand(blinker_MASK, blinker_CMD_START);
        #endif /* (0u == blinker_TC_START_SIGNAL_PRESENT) */
    #endif /* (blinker__TIMER == blinker_CONFIG) */
    
    #if (blinker__QUAD == blinker_CONFIG)
        #if (0u != blinker_QUAD_AUTO_START)
            blinker_TriggerCommand(blinker_MASK, blinker_CMD_RELOAD);
        #endif /* (0u != blinker_QUAD_AUTO_START) */
    #endif  /* (blinker__QUAD == blinker_CONFIG) */
}


/*******************************************************************************
* Function Name: blinker_Start
********************************************************************************
*
* Summary:
*  Initializes the blinker with default customizer
*  values when called the first time and enables the blinker.
*  For subsequent calls the configuration is left unchanged and the component is
*  just enabled.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  blinker_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time blinker_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the blinker_Start() routine.
*
*******************************************************************************/
void blinker_Start(void)
{
    if (0u == blinker_initVar)
    {
        blinker_Init();
        blinker_initVar = 1u;
    }

    blinker_Enable();
}


/*******************************************************************************
* Function Name: blinker_Stop
********************************************************************************
*
* Summary:
*  Disables the blinker.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void blinker_Stop(void)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_BLOCK_CONTROL_REG &= (uint32)~blinker_MASK;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetMode
********************************************************************************
*
* Summary:
*  Sets the operation mode of the blinker. This function is used when
*  configured as a generic blinker and the actual mode of operation is
*  set at runtime. The mode must be set while the component is disabled.
*
* Parameters:
*  mode: Mode for the blinker to operate in
*   Values:
*   - blinker_MODE_TIMER_COMPARE - Timer / Counter with
*                                                 compare capability
*         - blinker_MODE_TIMER_CAPTURE - Timer / Counter with
*                                                 capture capability
*         - blinker_MODE_QUAD - Quadrature decoder
*         - blinker_MODE_PWM - PWM
*         - blinker_MODE_PWM_DT - PWM with dead time
*         - blinker_MODE_PWM_PR - PWM with pseudo random capability
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetMode(uint32 mode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_MODE_MASK;
    blinker_CONTROL_REG |= mode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetQDMode
********************************************************************************
*
* Summary:
*  Sets the the Quadrature Decoder to one of the 3 supported modes.
*  Its functionality is only applicable to Quadrature Decoder operation.
*
* Parameters:
*  qdMode: Quadrature Decoder mode
*   Values:
*         - blinker_MODE_X1 - Counts on phi 1 rising
*         - blinker_MODE_X2 - Counts on both edges of phi1 (2x faster)
*         - blinker_MODE_X4 - Counts on both edges of phi1 and phi2
*                                        (4x faster)
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetQDMode(uint32 qdMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_QUAD_MODE_MASK;
    blinker_CONTROL_REG |= qdMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetPrescaler
********************************************************************************
*
* Summary:
*  Sets the prescaler value that is applied to the clock input.  Not applicable
*  to a PWM with the dead time mode or Quadrature Decoder mode.
*
* Parameters:
*  prescaler: Prescaler divider value
*   Values:
*         - blinker_PRESCALE_DIVBY1    - Divide by 1 (no prescaling)
*         - blinker_PRESCALE_DIVBY2    - Divide by 2
*         - blinker_PRESCALE_DIVBY4    - Divide by 4
*         - blinker_PRESCALE_DIVBY8    - Divide by 8
*         - blinker_PRESCALE_DIVBY16   - Divide by 16
*         - blinker_PRESCALE_DIVBY32   - Divide by 32
*         - blinker_PRESCALE_DIVBY64   - Divide by 64
*         - blinker_PRESCALE_DIVBY128  - Divide by 128
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetPrescaler(uint32 prescaler)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_PRESCALER_MASK;
    blinker_CONTROL_REG |= prescaler;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetOneShot
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the blinker runs
*  continuously or stops when terminal count is reached.  By default the
*  blinker operates in the continuous mode.
*
* Parameters:
*  oneShotEnable
*   Values:
*     - 0 - Continuous
*     - 1 - Enable One Shot
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetOneShot(uint32 oneShotEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_ONESHOT_MASK;
    blinker_CONTROL_REG |= ((uint32)((oneShotEnable & blinker_1BIT_MASK) <<
                                                               blinker_ONESHOT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetPWMMode
********************************************************************************
*
* Summary:
*  Writes the control register that determines what mode of operation the PWM
*  output lines are driven in.  There is a setting for what to do on a
*  comparison match (CC_MATCH), on an overflow (OVERFLOW) and on an underflow
*  (UNDERFLOW).  The value for each of the three must be ORed together to form
*  the mode.
*
* Parameters:
*  modeMask: A combination of three mode settings.  Mask must include a value
*  for each of the three or use one of the preconfigured PWM settings.
*   Values:
*     - CC_MATCH_SET        - Set on comparison match
*     - CC_MATCH_CLEAR      - Clear on comparison match
*     - CC_MATCH_INVERT     - Invert on comparison match
*     - CC_MATCH_NO_CHANGE  - No change on comparison match
*     - OVERLOW_SET         - Set on overflow
*     - OVERLOW_CLEAR       - Clear on  overflow
*     - OVERLOW_INVERT      - Invert on overflow
*     - OVERLOW_NO_CHANGE   - No change on overflow
*     - UNDERFLOW_SET       - Set on underflow
*     - UNDERFLOW_CLEAR     - Clear on underflow
*     - UNDERFLOW_INVERT    - Invert on underflow
*     - UNDERFLOW_NO_CHANGE - No change on underflow
*     - PWM_MODE_LEFT       - Setting for left aligned PWM.  Should be combined
*                             with up counting mode
*     - PWM_MODE_RIGHT      - Setting for right aligned PWM.  Should be combined
*                             with down counting mode
*     - PWM_MODE_CENTER     - Setting for center aligned PWM.  Should be
*                             combined with up/down 0 mode
*     - PWM_MODE_ASYM       - Setting for asymmetric PWM.  Should be combined
*                             with up/down 1 mode
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetPWMMode(uint32 modeMask)
{
    blinker_TRIG_CONTROL2_REG = (modeMask & blinker_6BIT_MASK);
}



/*******************************************************************************
* Function Name: blinker_SetPWMSyncKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes asynchronous or synchronous kill operation.  By default the kill
*  operation is asynchronous.  This functionality is only applicable to the PWM
*  and PWM with dead time modes.
*
*  For Synchronous mode the kill signal disables both the line and line_n
*  signals until the next terminal count.
*
*  For Asynchronous mode the kill signal disables both the line and line_n
*  signals when the kill signal is present.  This mode should only be used
*  when the kill signal (stop input) is configured in the pass through mode
*  (Level sensitive signal).

*
* Parameters:
*  syncKillEnable
*   Values:
*     - 0 - Asynchronous
*     - 1 - Synchronous
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetPWMSyncKill(uint32 syncKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_PWM_SYNC_KILL_MASK;
    blinker_CONTROL_REG |= ((uint32)((syncKillEnable & blinker_1BIT_MASK)  <<
                                               blinker_PWM_SYNC_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetPWMStopOnKill
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the PWM kill signal (stop input)
*  causes the PWM counter to stop.  By default the kill operation does not stop
*  the counter.  This functionality is only applicable to the three PWM modes.
*
*
* Parameters:
*  stopOnKillEnable
*   Values:
*     - 0 - Don't stop
*     - 1 - Stop
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetPWMStopOnKill(uint32 stopOnKillEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_PWM_STOP_KILL_MASK;
    blinker_CONTROL_REG |= ((uint32)((stopOnKillEnable & blinker_1BIT_MASK)  <<
                                                         blinker_PWM_STOP_KILL_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetPWMDeadTime
********************************************************************************
*
* Summary:
*  Writes the dead time control value.  This value delays the rising edge of
*  both the line and line_n signals the designated number of cycles resulting
*  in both signals being inactive for that many cycles.  This functionality is
*  only applicable to the PWM in the dead time mode.

*
* Parameters:
*  Dead time to insert
*   Values: 0 to 255
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetPWMDeadTime(uint32 deadTime)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_PRESCALER_MASK;
    blinker_CONTROL_REG |= ((uint32)((deadTime & blinker_8BIT_MASK) <<
                                                          blinker_PRESCALER_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetPWMInvert
********************************************************************************
*
* Summary:
*  Writes the bits that control whether the line and line_n outputs are
*  inverted from their normal output values.  This functionality is only
*  applicable to the three PWM modes.
*
* Parameters:
*  mask: Mask of outputs to invert.
*   Values:
*         - blinker_INVERT_LINE   - Inverts the line output
*         - blinker_INVERT_LINE_N - Inverts the line_n output
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetPWMInvert(uint32 mask)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_INV_OUT_MASK;
    blinker_CONTROL_REG |= mask;

    CyExitCriticalSection(enableInterrupts);
}



/*******************************************************************************
* Function Name: blinker_WriteCounter
********************************************************************************
*
* Summary:
*  Writes a new 16bit counter value directly into the counter register, thus
*  setting the counter (not the period) to the value written. It is not
*  advised to write to this field when the counter is running.
*
* Parameters:
*  count: value to write
*
* Return:
*  None
*
*******************************************************************************/
void blinker_WriteCounter(uint32 count)
{
    blinker_COUNTER_REG = (count & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_ReadCounter
********************************************************************************
*
* Summary:
*  Reads the current counter value.
*
* Parameters:
*  None
*
* Return:
*  Current counter value
*
*******************************************************************************/
uint32 blinker_ReadCounter(void)
{
    return (blinker_COUNTER_REG & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_SetCounterMode
********************************************************************************
*
* Summary:
*  Sets the counter mode.  Applicable to all modes except Quadrature Decoder
*  and the PWM with a pseudo random output.
*
* Parameters:
*  counterMode: Enumerated counter type values
*   Values:
*     - blinker_COUNT_UP       - Counts up
*     - blinker_COUNT_DOWN     - Counts down
*     - blinker_COUNT_UPDOWN0  - Counts up and down. Terminal count
*                                         generated when counter reaches 0
*     - blinker_COUNT_UPDOWN1  - Counts up and down. Terminal count
*                                         generated both when counter reaches 0
*                                         and period
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetCounterMode(uint32 counterMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_UPDOWN_MASK;
    blinker_CONTROL_REG |= counterMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_WritePeriod
********************************************************************************
*
* Summary:
*  Writes the 16 bit period register with the new period value.
*  To cause the counter to count for N cycles this register should be written
*  with N-1 (counts from 0 to period inclusive).
*
* Parameters:
*  period: Period value
*
* Return:
*  None
*
*******************************************************************************/
void blinker_WritePeriod(uint32 period)
{
    blinker_PERIOD_REG = (period & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the 16 bit period register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 blinker_ReadPeriod(void)
{
    return (blinker_PERIOD_REG & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_SetCompareSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the compare registers are
*  swapped. When enabled in the Timer/Counter mode(without capture) the swap
*  occurs at a TC event. In the PWM mode the swap occurs at the next TC event
*  following a hardware switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetCompareSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_RELOAD_CC_MASK;
    blinker_CONTROL_REG |= (swapEnable & blinker_1BIT_MASK);

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_WritePeriodBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit period buf register with the new period value.
*
* Parameters:
*  periodBuf: Period value
*
* Return:
*  None
*
*******************************************************************************/
void blinker_WritePeriodBuf(uint32 periodBuf)
{
    blinker_PERIOD_BUF_REG = (periodBuf & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_ReadPeriodBuf
********************************************************************************
*
* Summary:
*  Reads the 16 bit period buf register.
*
* Parameters:
*  None
*
* Return:
*  Period value
*
*******************************************************************************/
uint32 blinker_ReadPeriodBuf(void)
{
    return (blinker_PERIOD_BUF_REG & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_SetPeriodSwap
********************************************************************************
*
* Summary:
*  Writes the register that controls whether the period registers are
*  swapped. When enabled in Timer/Counter mode the swap occurs at a TC event.
*  In the PWM mode the swap occurs at the next TC event following a hardware
*  switch event.
*
* Parameters:
*  swapEnable
*   Values:
*     - 0 - Disable swap
*     - 1 - Enable swap
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetPeriodSwap(uint32 swapEnable)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_CONTROL_REG &= (uint32)~blinker_RELOAD_PERIOD_MASK;
    blinker_CONTROL_REG |= ((uint32)((swapEnable & blinker_1BIT_MASK) <<
                                                            blinker_RELOAD_PERIOD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_WriteCompare
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compare: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void blinker_WriteCompare(uint32 compare)
{
    #if (blinker_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (blinker_CY_TCPWM_4000) */

    #if (blinker_CY_TCPWM_4000)
        currentMode = ((blinker_CONTROL_REG & blinker_UPDOWN_MASK) >> blinker_UPDOWN_SHIFT);

        if (((uint32)blinker__COUNT_DOWN == currentMode) && (0xFFFFu != compare))
        {
            compare++;
        }
        else if (((uint32)blinker__COUNT_UP == currentMode) && (0u != compare))
        {
            compare--;
        }
        else
        {
        }
        
    
    #endif /* (blinker_CY_TCPWM_4000) */
    
    blinker_COMP_CAP_REG = (compare & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_ReadCompare
********************************************************************************
*
* Summary:
*  Reads the compare register. Not applicable for Timer/Counter with Capture
*  or in Quadrature Decoder modes.
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
* Parameters:
*  None
*
* Return:
*  Compare value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 blinker_ReadCompare(void)
{
    #if (blinker_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (blinker_CY_TCPWM_4000) */

    #if (blinker_CY_TCPWM_4000)
        currentMode = ((blinker_CONTROL_REG & blinker_UPDOWN_MASK) >> blinker_UPDOWN_SHIFT);
        
        regVal = blinker_COMP_CAP_REG;
        
        if (((uint32)blinker__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)blinker__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & blinker_16BIT_MASK);
    #else
        return (blinker_COMP_CAP_REG & blinker_16BIT_MASK);
    #endif /* (blinker_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: blinker_WriteCompareBuf
********************************************************************************
*
* Summary:
*  Writes the 16 bit compare buffer register with the new compare value. Not
*  applicable for Timer/Counter with Capture or in Quadrature Decoder modes.
*
* Parameters:
*  compareBuf: Compare value
*
* Return:
*  None
*
* Note:
*  It is not recommended to use the value equal to "0" or equal to 
*  "period value" in Center or Asymmetric align PWM modes on the 
*  PSoC 4100/PSoC 4200 devices.
*  PSoC 4000 devices write the 16 bit compare register with the decremented 
*  compare value in the Up counting mode (except 0x0u), and the incremented 
*  compare value in the Down counting mode (except 0xFFFFu).
*
*******************************************************************************/
void blinker_WriteCompareBuf(uint32 compareBuf)
{
    #if (blinker_CY_TCPWM_4000)
        uint32 currentMode;
    #endif /* (blinker_CY_TCPWM_4000) */

    #if (blinker_CY_TCPWM_4000)
        currentMode = ((blinker_CONTROL_REG & blinker_UPDOWN_MASK) >> blinker_UPDOWN_SHIFT);

        if (((uint32)blinker__COUNT_DOWN == currentMode) && (0xFFFFu != compareBuf))
        {
            compareBuf++;
        }
        else if (((uint32)blinker__COUNT_UP == currentMode) && (0u != compareBuf))
        {
            compareBuf --;
        }
        else
        {
        }
    #endif /* (blinker_CY_TCPWM_4000) */
    
    blinker_COMP_CAP_BUF_REG = (compareBuf & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_ReadCompareBuf
********************************************************************************
*
* Summary:
*  Reads the compare buffer register. Not applicable for Timer/Counter with
*  Capture or in Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Compare buffer value
*
* Note:
*  PSoC 4000 devices read the incremented compare register value in the 
*  Up counting mode (except 0xFFFFu), and the decremented value in the 
*  Down counting mode (except 0x0u).
*
*******************************************************************************/
uint32 blinker_ReadCompareBuf(void)
{
    #if (blinker_CY_TCPWM_4000)
        uint32 currentMode;
        uint32 regVal;
    #endif /* (blinker_CY_TCPWM_4000) */

    #if (blinker_CY_TCPWM_4000)
        currentMode = ((blinker_CONTROL_REG & blinker_UPDOWN_MASK) >> blinker_UPDOWN_SHIFT);

        regVal = blinker_COMP_CAP_BUF_REG;
        
        if (((uint32)blinker__COUNT_DOWN == currentMode) && (0u != regVal))
        {
            regVal--;
        }
        else if (((uint32)blinker__COUNT_UP == currentMode) && (0xFFFFu != regVal))
        {
            regVal++;
        }
        else
        {
        }

        return (regVal & blinker_16BIT_MASK);
    #else
        return (blinker_COMP_CAP_BUF_REG & blinker_16BIT_MASK);
    #endif /* (blinker_CY_TCPWM_4000) */
}


/*******************************************************************************
* Function Name: blinker_ReadCapture
********************************************************************************
*
* Summary:
*  Reads the captured counter value. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture value
*
*******************************************************************************/
uint32 blinker_ReadCapture(void)
{
    return (blinker_COMP_CAP_REG & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_ReadCaptureBuf
********************************************************************************
*
* Summary:
*  Reads the capture buffer register. This API is applicable only for
*  Timer/Counter with the capture mode and Quadrature Decoder modes.
*
* Parameters:
*  None
*
* Return:
*  Capture buffer value
*
*******************************************************************************/
uint32 blinker_ReadCaptureBuf(void)
{
    return (blinker_COMP_CAP_BUF_REG & blinker_16BIT_MASK);
}


/*******************************************************************************
* Function Name: blinker_SetCaptureMode
********************************************************************************
*
* Summary:
*  Sets the capture trigger mode. For PWM mode this is the switch input.
*  This input is not applicable to the Timer/Counter without Capture and
*  Quadrature Decoder modes.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - blinker_TRIG_LEVEL     - Level
*     - blinker_TRIG_RISING    - Rising edge
*     - blinker_TRIG_FALLING   - Falling edge
*     - blinker_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetCaptureMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_TRIG_CONTROL1_REG &= (uint32)~blinker_CAPTURE_MASK;
    blinker_TRIG_CONTROL1_REG |= triggerMode;

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetReloadMode
********************************************************************************
*
* Summary:
*  Sets the reload trigger mode. For Quadrature Decoder mode this is the index
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - blinker_TRIG_LEVEL     - Level
*     - blinker_TRIG_RISING    - Rising edge
*     - blinker_TRIG_FALLING   - Falling edge
*     - blinker_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetReloadMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_TRIG_CONTROL1_REG &= (uint32)~blinker_RELOAD_MASK;
    blinker_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << blinker_RELOAD_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetStartMode
********************************************************************************
*
* Summary:
*  Sets the start trigger mode. For Quadrature Decoder mode this is the
*  phiB input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - blinker_TRIG_LEVEL     - Level
*     - blinker_TRIG_RISING    - Rising edge
*     - blinker_TRIG_FALLING   - Falling edge
*     - blinker_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetStartMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_TRIG_CONTROL1_REG &= (uint32)~blinker_START_MASK;
    blinker_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << blinker_START_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetStopMode
********************************************************************************
*
* Summary:
*  Sets the stop trigger mode. For PWM mode this is the kill input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - blinker_TRIG_LEVEL     - Level
*     - blinker_TRIG_RISING    - Rising edge
*     - blinker_TRIG_FALLING   - Falling edge
*     - blinker_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetStopMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_TRIG_CONTROL1_REG &= (uint32)~blinker_STOP_MASK;
    blinker_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << blinker_STOP_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_SetCountMode
********************************************************************************
*
* Summary:
*  Sets the count trigger mode. For Quadrature Decoder mode this is the phiA
*  input.
*
* Parameters:
*  triggerMode: Enumerated trigger mode value
*   Values:
*     - blinker_TRIG_LEVEL     - Level
*     - blinker_TRIG_RISING    - Rising edge
*     - blinker_TRIG_FALLING   - Falling edge
*     - blinker_TRIG_BOTH      - Both rising and falling edge
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetCountMode(uint32 triggerMode)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_TRIG_CONTROL1_REG &= (uint32)~blinker_COUNT_MASK;
    blinker_TRIG_CONTROL1_REG |= ((uint32)(triggerMode << blinker_COUNT_SHIFT));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_TriggerCommand
********************************************************************************
*
* Summary:
*  Triggers the designated command to occur on the designated TCPWM instances.
*  The mask can be used to apply this command simultaneously to more than one
*  instance.  This allows multiple TCPWM instances to be synchronized.
*
* Parameters:
*  mask: A combination of mask bits for each instance of the TCPWM that the
*        command should apply to.  This function from one instance can be used
*        to apply the command to any of the instances in the design.
*        The mask value for a specific instance is available with the MASK
*        define.
*  command: Enumerated command values. Capture command only applicable for
*           Timer/Counter with Capture and PWM modes.
*   Values:
*     - blinker_CMD_CAPTURE    - Trigger Capture/Switch command
*     - blinker_CMD_RELOAD     - Trigger Reload/Index command
*     - blinker_CMD_STOP       - Trigger Stop/Kill command
*     - blinker_CMD_START      - Trigger Start/phiB command
*
* Return:
*  None
*
*******************************************************************************/
void blinker_TriggerCommand(uint32 mask, uint32 command)
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();

    blinker_COMMAND_REG = ((uint32)(mask << command));

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: blinker_ReadStatus
********************************************************************************
*
* Summary:
*  Reads the status of the blinker.
*
* Parameters:
*  None
*
* Return:
*  Status
*   Values:
*     - blinker_STATUS_DOWN    - Set if counting down
*     - blinker_STATUS_RUNNING - Set if counter is running
*
*******************************************************************************/
uint32 blinker_ReadStatus(void)
{
    return ((blinker_STATUS_REG >> blinker_RUNNING_STATUS_SHIFT) |
            (blinker_STATUS_REG & blinker_STATUS_DOWN));
}


/*******************************************************************************
* Function Name: blinker_SetInterruptMode
********************************************************************************
*
* Summary:
*  Sets the interrupt mask to control which interrupt
*  requests generate the interrupt signal.
*
* Parameters:
*   interruptMask: Mask of bits to be enabled
*   Values:
*     - blinker_INTR_MASK_TC       - Terminal count mask
*     - blinker_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetInterruptMode(uint32 interruptMask)
{
    blinker_INTERRUPT_MASK_REG =  interruptMask;
}


/*******************************************************************************
* Function Name: blinker_GetInterruptSourceMasked
********************************************************************************
*
* Summary:
*  Gets the interrupt requests masked by the interrupt mask.
*
* Parameters:
*   None
*
* Return:
*  Masked interrupt source
*   Values:
*     - blinker_INTR_MASK_TC       - Terminal count mask
*     - blinker_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 blinker_GetInterruptSourceMasked(void)
{
    return (blinker_INTERRUPT_MASKED_REG);
}


/*******************************************************************************
* Function Name: blinker_GetInterruptSource
********************************************************************************
*
* Summary:
*  Gets the interrupt requests (without masking).
*
* Parameters:
*  None
*
* Return:
*  Interrupt request value
*   Values:
*     - blinker_INTR_MASK_TC       - Terminal count mask
*     - blinker_INTR_MASK_CC_MATCH - Compare count / capture mask
*
*******************************************************************************/
uint32 blinker_GetInterruptSource(void)
{
    return (blinker_INTERRUPT_REQ_REG);
}


/*******************************************************************************
* Function Name: blinker_ClearInterrupt
********************************************************************************
*
* Summary:
*  Clears the interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to clear
*   Values:
*     - blinker_INTR_MASK_TC       - Terminal count mask
*     - blinker_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void blinker_ClearInterrupt(uint32 interruptMask)
{
    blinker_INTERRUPT_REQ_REG = interruptMask;
}


/*******************************************************************************
* Function Name: blinker_SetInterrupt
********************************************************************************
*
* Summary:
*  Sets a software interrupt request.
*
* Parameters:
*   interruptMask: Mask of interrupts to set
*   Values:
*     - blinker_INTR_MASK_TC       - Terminal count mask
*     - blinker_INTR_MASK_CC_MATCH - Compare count / capture mask
*
* Return:
*  None
*
*******************************************************************************/
void blinker_SetInterrupt(uint32 interruptMask)
{
    blinker_INTERRUPT_SET_REG = interruptMask;
}


/* [] END OF FILE */
