#ifndef __MKE16Z4_HAL_CLOCK
#define __MKE16Z4_HAL_CLOCK

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "MKE16Z4.h"
/******************************************************************************
 * EXPORTED TYPEDEF
 ******************************************************************************/

/******************************************************************************
 * EXPORTED CONSTANTS
 ******************************************************************************/

/** @defgroup SCG_ClkFreq_define
  * @{
  */
#define SCG_CLKFREQ_SIRC_HIGH                     (8000000U)
#define SCG_CLKFREQ_SIRC_LOW                      (2000000U)
#define SCG_CLKFREQ_FIRC                         (48000000U)
#define SCG_CLKFREQ_SOSC                          (8000000U)
#define SCG_CLKFREQ_LPFLL                        (48000000U)
 /**
  * @}
  */

/** @defgroup SCG_SOSCCSR_SOSCVLD_define
  * @{
  */
#define SCG_SOSCCSR_SOSCVLD_INVALID                 (0x00000000U)
#define SCG_SOSCCSR_SOSCVLD_VALID                   (0x00000001U)
 /**
  * @}
  */

/** @defgroup SCG_SIRCCSR_SIRCVLD_define
  * @{
  */
#define SCG_SIRCCSR_SIRCVLD_INVALID                 (0x00000000U)
#define SCG_SIRCCSR_SIRCVLD_VALID                   (0x00000001U)
 /**
  * @}
  */

/** @defgroup SCG_FIRCCSR_FIRCVLD_define
  * @{
  */
#define SCG_FIRCCSR_FIRCVLD_INVALID                 (0x00000000U)
#define SCG_FIRCCSR_FIRCVLD_VALID                   (0x00000001U)
 /**
  * @}
  */

/** @defgroup SCG_LPFLLCSR_LPFLLVLD_define
  * @{
  */
#define SCG_LPFLLCSR_LPFLLVLD_INVALID                 (0x00000000U)
#define SCG_LPFLLCSR_LPFLLVLD_VALID                   (0x00000001U)
 /**
  * @}
  */

/** @defgroup SCG_SIRCCFG_RANGE_define
  * @{
  */
#define SCG_SIRCCFG_HIGH_RANGE                  (0x00000001U)
#define SCG_SIRCCFG_LOW_RANGE                   (0x00000000U)
 /**
  * @}
  */
/******************************************************************************
 * EXPORTED MACROS
 ******************************************************************************/
/** @defgroup SCG_PCC_Exported_Macros SCG and PCC Exported Macros
  * @{
  */

/** @defgroup PCC_ClockGateControl_Enable PCC Clock Gate Control Enable Disable
  * @brief  Enable or disable the CGC.
  * @note   After reset, the peripheral clock (CGC) (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @param  __PERIPH__ specifies the peripheral (index in CLKCFG)
  *         This parameter can be one of the following values:
  *                  @arg  PCC_FLASH_INDEX                          32
  *                  @arg  PCC_MSCAN0_INDEX                         36
  *                  @arg  PCC_LPSPI0_INDEX                         44
  *                  @arg  PCC_CRC_INDEX                            50
  *                  @arg  PCC_PDB0_INDEX                           54
  *                  @arg  PCC_LPIT0_INDEX                          55
  *                  @arg  PCC_FLEXTMR0_INDEX                       56
  *                  @arg  PCC_FLEXTMR1_INDEX                       57
  *                  @arg  PCC_ADC0_INDEX                           59
  *                  @arg  PCC_RTC_INDEX                            61
  *                  @arg  PCC_LPTMR0_INDEX                         64
  *                  @arg  PCC_TSI_INDEX                            69
  *                  @arg  PCC_PORTA_INDEX                          73
  *                  @arg  PCC_PORTB_INDEX                          74
  *                  @arg  PCC_PORTC_INDEX                          75
  *                  @arg  PCC_PORTD_INDEX                          76
  *                  @arg  PCC_PORTE_INDEX                          77
  *                  @arg  PCC_PWT_INDEX                            86
  *                  @arg  PCC_EWM_INDEX                            97
  *                  @arg  PCC_LPI2C0_INDEX                         102
  *                  @arg  PCC_LPUART0_INDEX                        106
  *                  @arg  PCC_LPUART1_INDEX                        107
  *                  @arg  PCC_LPUART2_INDEX                        108
  *                  @arg  PCC_CMP0_INDEX                           115
  * @{
  */
#define __HAL_PCC_CGC_ENABLE(__PERIPH__) \
                    (PCC->CLKCFG[__PERIPH__] |= PCC_CLKCFG_CGC_MASK)

#define __HAL_PCC_CGC_DISABLE(__PERIPH__) \
                    (PCC->CLKCFG[__PERIPH__] &= ~(PCC_CLKCFG_CGC_MASK))
/**
  * @}
  */

/** @defgroup PCC_PeripheralClockSelect_Config PCS Clock source select and get clock source
  * @brief  Config or get the clock source of the PCS.
  * @param  __PERIPH__ specifies the peripheral (index in CLKCFG)
  *         This parameter can be one of the following values:
  *                  @arg  PCC_FLASH_INDEX                          32
  *                  @arg  PCC_MSCAN0_INDEX                         36
  *                  @arg  PCC_LPSPI0_INDEX                         44
  *                  @arg  PCC_CRC_INDEX                            50
  *                  @arg  PCC_PDB0_INDEX                           54
  *                  @arg  PCC_LPIT0_INDEX                          55
  *                  @arg  PCC_FLEXTMR0_INDEX                       56
  *                  @arg  PCC_FLEXTMR1_INDEX                       57
  *                  @arg  PCC_ADC0_INDEX                           59
  *                  @arg  PCC_RTC_INDEX                            61
  *                  @arg  PCC_LPTMR0_INDEX                         64
  *                  @arg  PCC_TSI_INDEX                            69
  *                  @arg  PCC_PORTA_INDEX                          73
  *                  @arg  PCC_PORTB_INDEX                          74
  *                  @arg  PCC_PORTC_INDEX                          75
  *                  @arg  PCC_PORTD_INDEX                          76
  *                  @arg  PCC_PORTE_INDEX                          77
  *                  @arg  PCC_PWT_INDEX                            86
  *                  @arg  PCC_EWM_INDEX                            97
  *                  @arg  PCC_LPI2C0_INDEX                         102
  *                  @arg  PCC_LPUART0_INDEX                        106
  *                  @arg  PCC_LPUART1_INDEX                        107
  *                  @arg  PCC_LPUART2_INDEX                        108
  *                  @arg  PCC_CMP0_INDEX                           115
  * @param __CLKSOURCE__ specifies the clock source
  *         This parameter can be one of these values
  *                  @arg   0b000..Clock is off.
  *                  @arg   0b001..Clock option 1
  *                  @arg   0b010..Clock option 2
  *                  @arg   0b011..Clock option 3
  *                  @arg   0b100..Clock option 4
  *                  @arg   0b101..Clock option 5
  *                  @arg   0b110..Clock option 6
  *                  @arg   0b111..Clock option 7

  * @{
  */
#define __HAL_PCC_PCS_CLK_CONFIG(__PERIPH__, __CLKSOURCE__) \
                    (PCC->CLKCFG[__PERIPH__] = (PCC->CLKCFG[__PERIPH__] & ~(PCC_CLKCFG_PCS_MASK)) \
                                             | (PCC_CLKCFG_PCS(__CLKSOURCE__)))

#define __HAL_PCC_PCS_GET_CLK(__PERIPH__) \
                    ((uint32_t)(PCC->CLKCFG[__PERIPH__] & PCC_CLKCFG_PCS_MASK) >> PCC_CLKCFG_PCS_SHIFT)
/**
  * @}
  */

/** @defgroup SCG_Check_Valid_Clock
  * @brief  Check if clock is valid and enabled
  * @{
  */
#define __HAL_SCG_SOSC_CHECK_VALID() \
                    ((SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) != SCG_SOSCCSR_SOSCVLD(SCG_SOSCCSR_SOSCVLD_INVALID))

#define __HAL_SCG_SIRC_CHECK_VALID() \
                    ((SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK) != SCG_SIRCCSR_SIRCVLD(SCG_SIRCCSR_SIRCVLD_INVALID))

#define __HAL_SCG_FIRC_CHECK_VALID() \
                    ((SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) != SCG_FIRCCSR_FIRCVLD(SCG_FIRCCSR_FIRCVLD_INVALID))

#define __HAL_SCG_LPFLL_CHECK_VALID() \
                    ((SCG->LPFLLCSR & SCG_LPFLLCSR_LPFLLVLD_MASK) != SCG_LPFLLCSR_LPFLLVLD(SCG_LPFLLCSR_LPFLLVLD_INVALID))
/**
  * @}
  */

/** @defgroup SCG_SIRC_Range
  * @brief  Check the range of SIRC Clock
  * @param __RANGE__ specifies the range of SIRC
  *         This parameter can be one of these values
  *                  @arg   0b0..Slow IRC low range clock (2 MHz)
  *                  @arg   0b1..Slow IRC high range clock (8 MHz )
  * @{
  */
#define __HAL_SCG_SIRC_CHECK_RANGE(__RANGE__) \
                    ((SCG->SIRCCFG & SCG_SIRCCFG_RANGE_MASK) == SCG_SIRCCFG_RANGE(__RANGE__))
/**
  * @}
  */

/** @defgroup SCG_Divider_Config
  * @brief  Config or get the clock divider
  * @param __CLKDIV__ specifies the clock divider ratio of clock
  *         This parameter can be one of these values
  *                  @arg   0b000..Output disabled
  *                  @arg   0b001..Divide by 1
  *                  @arg   0b010..Divide by 2
  *                  @arg   0b011..Divide by 4
  *                  @arg   0b100..Divide by 8
  *                  @arg   0b101..Divide by 16
  *                  @arg   0b110..Divide by 32
  *                  @arg   0b111..Divide by 64
  * @{
  */
#define __HAL_SCG_SOSC_DIVIDER_CONFIG(__CLKDIV__) \
                    (SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV2(__CLKDIV__))

#define __HAL_SCG_SIRC_DIVIDER_CONFIG(__CLKDIV__) \
                    (SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV2(__CLKDIV__))

#define __HAL_SCG_FIRC_DIVIDER_CONFIG(__CLKDIV__) \
                    (SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV2(__CLKDIV__))

#define __HAL_SCG_LPFLL_DIVIDER_CONFIG(__CLKDIV__) \
                    (SCG->LPFLLDIV = SCG_LPFLLDIV_LPFLLDIV2(__CLKDIV__))

#define __HAL_SCG_SOSC_GET_DIVIDER() \
                    ((SCG->SOSCDIV & SCG_SOSCDIV_SOSCDIV2_MASK) >> SCG_SOSCDIV_SOSCDIV2_SHIFT)

#define __HAL_SCG_SIRC_GET_DIVIDER() \
                    ((SCG->SIRCDIV & SCG_SIRCDIV_SIRCDIV2_MASK) >> SCG_SIRCDIV_SIRCDIV2_SHIFT)

#define __HAL_SCG_FIRC_GET_DIVIDER() \
                    ((SCG->FIRCDIV & SCG_FIRCDIV_FIRCDIV2_MASK) >> SCG_FIRCDIV_FIRCDIV2_SHIFT)

#define __HAL_SCG_LPFLL_GET_DIVIDER() \
                    ((SCG->LPFLLDIV & SCG_LPFLLDIV_LPFLLDIV2_MASK) >> SCG_LPFLLDIV_LPFLLDIV2_SHIFT)
/**
  * @}
  */

/**
  * @}
  */
/******************************************************************************
 * EXPORTED VARIABLES
 ******************************************************************************/

/******************************************************************************
 * EXPORTED FUNCTIONS PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************/

#endif /* __MKE16Z4_HAL_CLOCK */

/******************************************************************************
 * End of file
 ******************************************************************************/
