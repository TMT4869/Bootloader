#ifndef __MKE16Z4_HAL_CORE
#define __MKE16Z4_HAL_CORE

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_def.h"
/******************************************************************************
 * EXPORTED TYPEDEF
 ******************************************************************************/

/******************************************************************************
 * EXPORTED CONSTANTS
 ******************************************************************************/
#define SYSTICK_1000HZ_FREQ                 (1000U)

/** @defgroup NVIC_Priority define
  * @{
  */
#define NVIC_PRIORITY_0                     (0)
#define NVIC_PRIORITY_1                     (1U)
#define NVIC_PRIORITY_2                     (2U)
#define NVIC_PRIORITY_3                     (3U)
#define NVIC_DISABLE                        (0xFF)
/**
  * @}
  */

/** @defgroup Peripheral_IRQn define Because we don't want to public CMSIS file so we will define again
  * @{
  */
#define NVIC_NonMaskableInt          (-14)              /**< Non Maskable Interrupt */
#define NVIC_HardFault               (-13)              /**< Cortex-M0 SV Hard Fault Interrupt */
#define NVIC_SVCall                  (-5)               /**< Cortex-M0 SV Call Interrupt */
#define NVIC_PendSV                  (-2)               /**< Cortex-M0 Pend SV Interrupt */
#define NVIC_SysTick                 (-1)               /**< Cortex-M0 System Tick Interrupt */
#define NVIC_FTFA                    (5)                /**< Flash memory single interrupt vector for all sources */
#define NVIC_LVD_LVW                 (6)                /**< Low-voltage detect) low-voltage warning */
#define NVIC_PORTAE                  (7)                /**< Pin detect (Port A) E) */
#define NVIC_LPI2C0                  (8)                /**< Inter-integrated circuit 0 interrupt */
#define NVIC_LPSPI0                  (10)               /**< Serial peripheral Interface 0 interrupt */
#define NVIC_LPUART0                 (12)               /**< Single interrupt vector for all sources */
#define NVIC_LPUART1                 (13)               /**< Single interrupt vector for all sources */
#define NVIC_LPUART2                 (14)               /**< Single interrupt vector for all sources */
#define NVIC_ADC0                    (15)               /**< ADC0 conversion complete interrupt */
#define NVIC_CMP0                    (16)               /**< CMP0 interrupt */
#define NVIC_FTM0                    (17)               /**< FTM0 single interrupt vector for all sources */
#define NVIC_FTM1                    (18)               /**< FTM1 single interrupt vector for all sources */
#define NVIC_RTC                     (20)               /**< Single interrupt vector for all sources */
#define NVIC_LPIT0                   (22)               /**< LPIT channel 0-1 */
#define NVIC_TSI                     (24)               /**< TSI interrupt */
#define NVIC_PDB0                    (25)               /**< Programmable delay block interrupt */
#define NVIC_PORTBCD                 (26)               /**< Pin detect (Port B) C) D) */
#define NVIC_SCG_RCM                 (27)               /**< Single interrupt vector for SCG and RCM */
#define NVIC_WDOG_EWM                (28)               /**< Single interrupt vector for WDOG and EWM */
#define NVIC_PWT_LPTMR0              (29)               /**< Single interrupt vector for PWT and LPTMR0 */
#define NVIC_MSCAN_Rx                (30)               /**< MSCAN Rx Interrupt */
#define NVIC_MSCAN_ORed              (31)               /**< MSCAN Tx) Err and Wake-up interrupt */

/******************************************************************************
 * EXPORTED MACROS
 ******************************************************************************/

/******************************************************************************
 * EXPORTED VARIABLES
 ******************************************************************************/

/******************************************************************************
 * EXPORTED FUNCTIONS PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************/

/**
 * @brief  Initialize SysTick with specified frequency and priority
 */
void SysTick_Init(uint32_t freq, uint32_t priority);

/**
 * @brief Deinitialize SysTick
 */
void SysTick_DeInit(void);

/**
 * @brief  Get current SysTick frequency
 */
uint32_t SysTick_GetFrequency(void);

/**
 * @brief  Get current SysTick ticks
 */
uint32_t SysTick_GetTick(void);

/**
 * @brief  Register callback function for interrupt
 */
void SysTick_RegisterCallback(void (*callbackFunc)(void));

/**
 * @brief  NVIC Configuration
 */
void NVIC_InterruptConfig(uint32_t IRQn, FunctionalState NewState, uint8_t Priority);

/**
 * @brief  Change the vector table
 */
void SCB_VectorTableOffSetConfig(uint32_t offset);

/**
 * @brief  Set Main Stack Pointer
 */
void ARM_SetMSP(uint32_t address);

#endif /* __MKE16Z4_HAL_CORE */

/******************************************************************************
 * END OF FILE
 ******************************************************************************/
