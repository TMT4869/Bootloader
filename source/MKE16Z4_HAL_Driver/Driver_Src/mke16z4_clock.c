/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_hal_clock.h"
#include "mke16z4_clock.h"
#include "mke16z4_def.h"
#include <assert.h>
/******************************************************************************
 * PRIVATE TYPEDEF
 ******************************************************************************/

/******************************************************************************
 * PRIVATE CONSTANTS
 ******************************************************************************/

/******************************************************************************
 * PRIVATE MACROS
 ******************************************************************************/

/******************************************************************************
 * PRIVATE VARIABLES
 ******************************************************************************/
static uint32_t SysCoreClock = 48000000U;
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/**
  * @brief  Enables or disables the peripheral clock of PCC Module output.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @param  PCC_Periph: specifies the peripheral of PCC to gates its clock.
  *          This parameter can be a value of @ref PCC_Peripheral_define
  * @param  NewState: new state of the specified peripheral clock.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PCC_ClockGateControl(const uint32_t PCC_Periph, const FunctionalState NewState)
{
    /* Check the parameters */
    assert(IS_PCC_PERIPH(PCC_Periph));
    assert(IS_FUNCTIONAL_STATE(NewState));
    /* Enable or disable clock */
    if (NewState != DISABLE)
    {
        __HAL_PCC_CGC_ENABLE(PCC_Periph);
    }
    else
    {
        __HAL_PCC_CGC_DISABLE(PCC_Periph);
    }
}

/**
  * @brief  Enables or disables the peripheral clock of PCC Module output.
  * @note   Before configuring a functional clock, the module's interface
  *         clock must be disabled (CGC = 0).
  * @param  PCC_Periph: specifies the peripheral of PCC to gates its clock.
  *          This parameter can be a value of @ref PCC_Peripheral_define
  * @param  ClockSource: specifies the DIV2 clock source to be selected.
  *          This parameter can be a value of @ref PCC_ClockSource_define
  * @retval None
  */
void PCC_PeriphClockSrcSelect(const uint32_t PCC_Periph, const uint32_t ClockSource)
{
    /* Check parameter */
    assert(IS_PCC_PERIPH(PCC_Periph));
    assert(IS_PCC_PERIPH_CLOCK_SOURCE(ClockSource));
    /* Disable clock first */
    __HAL_PCC_CGC_DISABLE(PCC_Periph);
    /* Update clock source */
    __HAL_PCC_PCS_CLK_CONFIG(PCC_Periph, ClockSource);
    /* Enable clock */
    __HAL_PCC_CGC_ENABLE(PCC_Periph);
}

/**
 * @brief  Get the value of the core/system clock
 * @param  None
 * @retval Frequency value of Core/System Clock
 */
uint32_t SCG_GetCoreSysClock(void)
{
    return SysCoreClock;
}

/**
 * @brief  Get the frequency of the specified clock source
 * @param  ClockSource: specifies the DIV2 clock source to be selected.
 *         This parameter can be a value of @ref SCG_ClockSource_define
 * @return Frequency of the specified clock source
 */
uint32_t SCG_GetCLOCK(const uint32_t ClockSource)
{
    uint32_t freq = 0x00;
    /* Check parameters */
    assert(IS_SCG_CLOCK_SOURCE(ClockSource));
    switch (ClockSource)
    {
        case SCG_CLOCKSOURCE_SOSC:
            /* Check if SOSC Clock is valid and enabled */
            if (__HAL_SCG_SOSC_CHECK_VALID())
            {
                freq = SCG_CLKFREQ_SOSC;
            }
            break;
        case SCG_CLOCKSOURCE_SIRC:
            /* Check if SIRC Clock is valid and enabled */
            if (__HAL_SCG_SIRC_CHECK_VALID())
            {
                freq = SCG_CLKFREQ_SIRC_HIGH;
                /* SIRC Low (2MHz) or High (8MHz) range */
                if (__HAL_SCG_SIRC_CHECK_RANGE(SCG_SIRCCFG_LOW_RANGE))
                {
                    freq = SCG_CLKFREQ_SIRC_LOW;
                }
            }
            break;
        case SCG_CLOCKSOURCE_FIRC:
            /* Check if FIRC Clock is valid and enabled */
            if (__HAL_SCG_FIRC_CHECK_VALID())
            {
                freq = SCG_CLKFREQ_FIRC;
            }
            break;
        case SCG_CLOCKSOURCE_LPFLL:
            /* Check if LPFLL Clock is valid and enabled */
            if (__HAL_SCG_LPFLL_CHECK_VALID())
            {
                freq = SCG_CLKFREQ_LPFLL;
            }
            break;
        default:
            break;
    }
    return freq;
}

/**
 * @brief  Set the divider for the specified DIV2 clock source
 * @param  ClockSource: specifies clock source to be configured
  *        This parameter can be a value of @ref SCG_Clocksource_define
 * @param  ClockDivider: divide ratio of the specified clock source.
  *        This parameter can be a value of @ref SCG_Divider_define
 * @retval None
 */
void SCG_SetCLOCKDIV2(const uint32_t ClockSource, const uint32_t ClockDivider)
{
    /* Check parameters */
    assert(IS_SCG_DIVIDER(ClockDivider));
    assert(IS_SCG_CLOCK_SOURCE(ClockSource));
    /* Set (FIRC,SIRC,SOSC,LPFLL)DIV2 */
    switch (ClockSource)
    {
        case SCG_CLOCKSOURCE_SOSC:
            __HAL_SCG_SOSC_DIVIDER_CONFIG(ClockDivider);
            break;
        case SCG_CLOCKSOURCE_SIRC:
            __HAL_SCG_SIRC_DIVIDER_CONFIG(ClockDivider);
            break;
        case SCG_CLOCKSOURCE_FIRC:
            __HAL_SCG_FIRC_DIVIDER_CONFIG(ClockDivider);
            break;
        case SCG_CLOCKSOURCE_LPFLL:
            __HAL_SCG_LPFLL_DIVIDER_CONFIG(ClockDivider);
            break;
        default:
            break;
    }
}

/**
 * @brief  Get the frequency of the specified DIV2 clock source
 * @param  ClockSource: specifies the clock source
  *        This parameter can be a value of @ref SCG_Clocksource_define
 * @retval Frequency of the specified DIV2 clock source
 */
uint32_t SCG_GetCLOCKDIV2(const uint32_t ClockSource)
{
    uint32_t freq = 0x00;
    uint32_t divider = 0x00;
    /* Check parameters */
    assert(IS_SCG_CLOCK_SOURCE(ClockSource));
    /* Get clock source frequency */
    freq = SCG_GetCLOCK(ClockSource);
    /* Get divider ratio */
    switch (ClockSource)
    {
        case SCG_CLOCKSOURCE_SOSC:
            divider = __HAL_SCG_SOSC_GET_DIVIDER();
            break;
        case SCG_CLOCKSOURCE_SIRC:
            divider = __HAL_SCG_SIRC_GET_DIVIDER();
            break;
        case SCG_CLOCKSOURCE_FIRC:
            divider = __HAL_SCG_FIRC_GET_DIVIDER();
            break;
        case SCG_CLOCKSOURCE_LPFLL:
            divider = __HAL_SCG_LPFLL_GET_DIVIDER();
            break;
        default:
            break;
    }
    if (divider != SCG_OUTPUT_DISABLED)
    {
        freq = freq >> (divider - 1U);
    }
    else
    {
        freq = 0;
    }
    return freq;
}

/**
 * @brief  Get the asynchronous clock frequency of the specified peripheral
 * @param  PCC_Periph: specifies the peripheral of PCC
  *        This parameter can be a value of @ref PCC_Peripheral_define
 * @return Asynchronous clock frequency of the specified peripheral
 */
uint32_t PCC_GetAsyncClock(const uint32_t PCC_Periph)
{
    uint32_t clkSource = 0x00;
    uint32_t freq = 0x00;
    /* Check parameter */
    assert(IS_PCC_ASYNC_PERIPH(PCC_Periph));
    /* Get clock source */
    clkSource = __HAL_PCC_PCS_GET_CLK(PCC_Periph);
    /* Get frequency */
    switch (clkSource)
    {
        case PCC_CLOCKSOURCE_SOSC:
            freq = SCG_GetCLOCKDIV2(SCG_CLOCKSOURCE_SOSC);
            break;
        case PCC_CLOCKSOURCE_SIRC:
            freq = SCG_GetCLOCKDIV2(SCG_CLOCKSOURCE_SIRC);
            break;
        case PCC_CLOCKSOURCE_FIRC:
            freq = SCG_GetCLOCKDIV2(SCG_CLOCKSOURCE_FIRC);
            break;
        case PCC_CLOCKSOURCE_LPFLL:
            freq = SCG_GetCLOCKDIV2(SCG_CLOCKSOURCE_LPFLL);
            break;
        default:
            freq = 0;
            break;
    }
    return freq;
}

/******************************************************************************
 * End of file
 ******************************************************************************/
