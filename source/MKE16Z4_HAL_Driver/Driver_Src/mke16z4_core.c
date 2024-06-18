/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "MKE16Z4.h"
#include "mke16z4_core.h"
#include "mke16z4_clock.h"
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
static uint32_t SysTickTimeFreq = 0U; /*!< Default frequency       */
static uint32_t SysTickTicks = 0;        /*!< Number of ticks         */
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 ******************************************************************************/
static void SysTick_DefaultHandler(void);
/* Callback function pointer */
static void (*pISRCallback)(void) = SysTick_DefaultHandler;
/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/**
 * @brief  Initialize SysTick with specified frequency and priority
 * @param  freq: Frequency of SysTick
 * @param  priority: Priority level of SysTick
 * @retval None
 */
void SysTick_Init(uint32_t freq , uint32_t priority)
{
    assert(IS_NVIC_PRIORITY(priority));
    assert(freq != 0U);
    SysTickTimeFreq = freq;
    SysTick_Config(SCG_GetCoreSysClock()/freq);
    NVIC_SetPriority(SysTick_IRQn, priority);
}

/**
 * @brief  Deinitialize SysTick
 * @param  None
 * @retval None
 */
void SysTick_DeInit(void)
{
    SysTick->CTRL  &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}
/**
 * @brief  Get current SysTick frequency
 * @param  None
 * @return Current frequency of SysTick
 */
uint32_t SysTick_GetFrequency(void)
{
    return SysTickTimeFreq;
}

/**
 * @brief  Get current SysTick ticks
 * @param  None
 * @return Current ticks of SysTick
 */
uint32_t SysTick_GetTick(void)
{
    return SysTickTicks;
}

/**
 * @brief  Register callback function for interrupt handlers
 * @param  callbackFunc: Function pointer of the callback function
 * @retval None
 */
void SysTick_RegisterCallback(void (*callbackFunc)(void))
{
    pISRCallback = callbackFunc;
}

/**
 * @brief  Default Handler for SysTick interrupt besides ticks increment
 * @param  None
 * @retval None
 */
static void SysTick_DefaultHandler(void)
{
    /* Empty function */
}

/**
 * @brief  SysTick interrupt handler
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    SysTickTicks++;
    pISRCallback();
}

/**
 * @brief  NVIC Configuration
 */
void NVIC_InterruptConfig(uint32_t Periph, FunctionalState NewState, uint8_t Priority)
{
    if (NewState == ENABLE)
    {
        NVIC_SetPriority((IRQn_Type)Periph, Priority);
        NVIC_DisableIRQ((IRQn_Type)Periph);
        NVIC_EnableIRQ((IRQn_Type)Periph);
        NVIC_ClearPendingIRQ((IRQn_Type)Periph);
    }
    else
    {
        NVIC_DisableIRQ((IRQn_Type)Periph);
    }
}

/**
 * @brief  Change the vector table
 * @param  offset: Offset from 0x00000000
 */
void SCB_VectorTableOffSetConfig(uint32_t offset)
{
    SCB->VTOR = offset;
}

/**
 * @brief  Set Main Stack Pointer
 * @param  address: Address to top of stack
 */
void ARM_SetMSP(uint32_t address)
{
    __set_MSP(address);
}
/******************************************************************************
 * END OF FILE
 ******************************************************************************/
