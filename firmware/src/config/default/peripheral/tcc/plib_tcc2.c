/*******************************************************************************
  Timer/Counter(TCC2) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_tcc2.c

  Summary
    TCC2 PLIB Implementation File.

  Description
    This file defines the interface to the TCC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/* This section lists the other files that are included in this file.
*/

#include "interrupts.h"
#include "plib_tcc2.h"







// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: TCC2 Implementation
// *****************************************************************************
// *****************************************************************************

/* Initialize TCC module in Compare Mode */
void TCC2_CompareInitialize( void )
{
    /* Reset TCC */
    TCC2_REGS->TCC_CTRLA = TCC_CTRLA_SWRST_Msk;

    while((TCC2_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_SWRST_Msk) == TCC_SYNCBUSY_SWRST_Msk)
    {
        /* Wait for Write Synchronization */
    }

    /* Configure counter mode & prescaler */
    TCC2_REGS->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1 | TCC_CTRLA_PRESCSYNC_PRESC ;
    /* Configure waveform generation mode */
    TCC2_REGS->TCC_WAVE = TCC_WAVE_WAVEGEN_NPWM;



    
    TCC2_REGS->TCC_PER = 60000U;
    
    TCC2_REGS->TCC_CC[0] = 0U;
    TCC2_REGS->TCC_CC[1] = 0U;

    /* Clear all interrupt flags */
    TCC2_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;


    while((TCC2_REGS->TCC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }
}

/* Enable the counter */
void TCC2_CompareStart( void )
{
    TCC2_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
    while((TCC2_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Disable the counter */
void TCC2_CompareStop( void )
{
    TCC2_REGS->TCC_CTRLA &= ~TCC_CTRLA_ENABLE_Msk;
    while((TCC2_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) == TCC_SYNCBUSY_ENABLE_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

uint32_t TCC2_CompareFrequencyGet( void )
{
    return (uint32_t)3000000;
}

void TCC2_CompareCommandSet(TCC_COMMAND command)
{
    TCC2_REGS->TCC_CTRLBSET = (uint8_t)((uint32_t)command << TCC_CTRLBSET_CMD_Pos);
    while((TCC2_REGS->TCC_SYNCBUSY) != 0U)
    {
        /* Wait for Write Synchronization */
    }    
}

/* Get the current counter value */
uint16_t TCC2_Compare16bitCounterGet( void )
{
    /* Write command to force COUNT register read synchronization */
    TCC2_REGS->TCC_CTRLBSET |= (uint8_t)TCC_CTRLBSET_CMD_READSYNC;

    while((TCC2_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_CTRLB_Msk) == TCC_SYNCBUSY_CTRLB_Msk)
    {
        /* Wait for Write Synchronization */
    }

    while((TCC2_REGS->TCC_CTRLBSET & TCC_CTRLBSET_CMD_Msk) != 0U)
    {
        /* Wait for CMD to become zero */
    }

    /* Read current count value */
    return (uint16_t)TCC2_REGS->TCC_COUNT;
}

/* Configure counter value */
void TCC2_Compare16bitCounterSet( uint16_t count )
{
    TCC2_REGS->TCC_COUNT = count;

    while((TCC2_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_COUNT_Msk) == TCC_SYNCBUSY_COUNT_Msk)
    {
        /* Wait for Write Synchronization */
    }
}

/* Configure period value */
bool TCC2_Compare16bitPeriodSet( uint16_t period )
{
    bool status = false;
    if((TCC2_REGS->TCC_STATUS & TCC_STATUS_PERBV_Msk) == 0U)
    {
        /* Configure period value */
        TCC2_REGS->TCC_PERB = period;
        status = true;
    }
    return status;
}

/* Read period value */
uint16_t TCC2_Compare16bitPeriodGet( void )
{
    /* Get period value */
    return (uint16_t)TCC2_REGS->TCC_PER;
}


/* Configure duty cycle value */
bool TCC2_Compare16bitMatchSet(TCC2_CHANNEL_NUM channel, uint16_t compareValue )
{
    bool status = false;
    if ((TCC2_REGS->TCC_STATUS & (1UL << (TCC_STATUS_CCBV0_Pos + (uint32_t)channel))) == 0U)
    {
        /* Set new compare value for compare channel */
        TCC2_REGS->TCC_CCB[channel] = compareValue & 0xFFFFFFU;
        status = true;
    }
    return status;
}




uint32_t TCC2_CompareStatusGet( void )
{
    uint32_t compare_status;
    compare_status = ((TCC2_REGS->TCC_INTFLAG));
    TCC2_REGS->TCC_INTFLAG = compare_status;
    return compare_status;
}
