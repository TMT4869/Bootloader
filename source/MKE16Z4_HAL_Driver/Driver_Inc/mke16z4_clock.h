#ifndef __MKE16Z4_CLOCK
#define __MKE16Z4_CLOCK

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

/** @defgroup PCC_Peripheral_define
  * @{
  */
#define PCC_PERIPH_FLASH             (32U)
#define PCC_PERIPH_MSCAN0            (36U)
#define PCC_PERIPH_LPSPI0            (44U)
#define PCC_PERIPH_CRC               (50U)
#define PCC_PERIPH_PDB0              (54U)
#define PCC_PERIPH_LPIT0             (55U)
#define PCC_PERIPH_FLEXTMR0          (56U)
#define PCC_PERIPH_FLEXTMR1          (57U)
#define PCC_PERIPH_ADC0              (59U)
#define PCC_PERIPH_RTC               (61U)
#define PCC_PERIPH_LPTMR0            (64U)
#define PCC_PERIPH_TSI               (69U)
#define PCC_PERIPH_PORTA             (73U)
#define PCC_PERIPH_PORTB             (74U)
#define PCC_PERIPH_PORTC             (75U)
#define PCC_PERIPH_PORTD             (76U)
#define PCC_PERIPH_PORTE             (77U)
#define PCC_PERIPH_PWT               (86U)
#define PCC_PERIPH_EWM               (97U)
#define PCC_PERIPH_LPI2C0            (102U)
#define PCC_PERIPH_LPUART0           (106U)
#define PCC_PERIPH_LPUART1           (107U)
#define PCC_PERIPH_LPUART2           (108U)
#define PCC_PERIPH_CMP0              (115U)

#define IS_PCC_PERIPH(PERIPH)  (((PERIPH) == PCC_PERIPH_ADC0) || \
                                ((PERIPH) == PCC_PERIPH_CMP0) || \
                                ((PERIPH) == PCC_PERIPH_CRC) || \
                                ((PERIPH) == PCC_PERIPH_EWM) || \
                                ((PERIPH) == PCC_PERIPH_FLASH) || \
                                ((PERIPH) == PCC_PERIPH_FLEXTMR0) || \
                                ((PERIPH) == PCC_PERIPH_FLEXTMR1) || \
                                ((PERIPH) == PCC_PERIPH_LPI2C0) || \
                                ((PERIPH) == PCC_PERIPH_LPIT0) || \
                                ((PERIPH) == PCC_PERIPH_LPSPI0) || \
                                ((PERIPH) == PCC_PERIPH_LPTMR0) || \
                                ((PERIPH) == PCC_PERIPH_LPUART0) || \
                                ((PERIPH) == PCC_PERIPH_LPUART1) || \
                                ((PERIPH) == PCC_PERIPH_LPUART2) || \
                                ((PERIPH) == PCC_PERIPH_MSCAN0) || \
                                ((PERIPH) == PCC_PERIPH_PDB0) || \
                                ((PERIPH) == PCC_PERIPH_PORTA) || \
                                ((PERIPH) == PCC_PERIPH_PORTB) || \
                                ((PERIPH) == PCC_PERIPH_PORTC) || \
                                ((PERIPH) == PCC_PERIPH_PORTD) || \
                                ((PERIPH) == PCC_PERIPH_PORTE) || \
                                ((PERIPH) == PCC_PERIPH_PWT) || \
                                ((PERIPH) == PCC_PERIPH_RTC) || \
                                ((PERIPH) == PCC_PERIPH_TSI))

#define IS_PCC_ASYNC_PERIPH(PERIPH)    (((PERIPH) == PCC_PERIPH_ADC0) || \
                                        ((PERIPH) == PCC_PERIPH_LPI2C0) || \
                                        ((PERIPH) == PCC_PERIPH_LPIT0) || \
                                        ((PERIPH) == PCC_PERIPH_LPSPI0) || \
                                        ((PERIPH) == PCC_PERIPH_LPTMR0) || \
                                        ((PERIPH) == PCC_PERIPH_LPUART0) || \
                                        ((PERIPH) == PCC_PERIPH_LPUART1) || \
                                        ((PERIPH) == PCC_PERIPH_LPUART2))
/**
  * @}
  */

/** @defgroup PCC_ClockSource_define
  * @{
  */
#define PCC_CLOCKSOURCE_Off              0x00U
#define PCC_CLOCKSOURCE_SOSC             0x01U
#define PCC_CLOCKSOURCE_SIRC             0x02U
#define PCC_CLOCKSOURCE_FIRC             0x03U
#define PCC_CLOCKSOURCE_LPFLL            0x05U

#define IS_PCC_PERIPH_CLOCK_SOURCE(CLK)    (((CLK) == PCC_CLOCKSOURCE_Off) || \
                                            ((CLK) == PCC_CLOCKSOURCE_FIRC) || \
                                            ((CLK) == PCC_CLOCKSOURCE_SIRC) || \
                                            ((CLK) == PCC_CLOCKSOURCE_SOSC) || \
                                            ((CLK) == PCC_CLOCKSOURCE_LPFLL))
/**
  * @}
  */

/** @defgroup SCG_ClockSource_define
  * @{
  */
#define SCG_CLOCKSOURCE_SOSC             0x00U
#define SCG_CLOCKSOURCE_SIRC             0x01U
#define SCG_CLOCKSOURCE_FIRC             0x02U
#define SCG_CLOCKSOURCE_LPFLL            0x03U

#define IS_SCG_CLOCK_SOURCE(CLK)   (((CLK) == SCG_CLOCKSOURCE_FIRC) || \
                                    ((CLK) == SCG_CLOCKSOURCE_SIRC) || \
                                    ((CLK) == SCG_CLOCKSOURCE_SOSC) || \
                                    ((CLK) == SCG_CLOCKSOURCE_LPFLL))
/**
  * @}
  */

/** @defgroup SCG_Divider_define
  * @{
  */
#define SCG_OUTPUT_DISABLED               0x00000000U
#define SCG_DIVIDE_1                      0x00000001U
#define SCG_DIVIDE_2                      0x00000002U
#define SCG_DIVIDE_4                      0x00000003U
#define SCG_DIVIDE_8                      0x00000004U
#define SCG_DIVIDE_16                     0x00000005U
#define SCG_DIVIDE_32                     0x00000006U
#define SCG_DIVIDE_64                     0x00000007U

#define IS_SCG_DIVIDER(RATIO)  (((RATIO) == SCG_OUTPUT_DISABLED) || \
                                ((RATIO) == SCG_DIVIDE_1) || \
                                ((RATIO) == SCG_DIVIDE_2) || \
                                ((RATIO) == SCG_DIVIDE_4) || \
                                ((RATIO) == SCG_DIVIDE_8) || \
                                ((RATIO) == SCG_DIVIDE_16) || \
                                ((RATIO) == SCG_DIVIDE_32) || \
                                ((RATIO) == SCG_DIVIDE_64))
 /**
  * @}
  */

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
/*  Functions to enable the bus clock to peripheral and choose the clock source */

/**
 * @brief  Enable or disable clock to PCC output peripherals
*/
void PCC_ClockGateControl(const uint32_t PCC_Periph, const FunctionalState NewState);

/**
 * @brief  Select clock source for a peripheral of PCC
*/
void PCC_PeriphClockSrcSelect(const uint32_t PCC_Periph, const uint32_t ClockSource);

/**
 * @brief  Get the asynchronous clock frequency of the specified peripheral
 */
uint32_t PCC_GetAsyncClock(const uint32_t PCC_Periph);

/*  Functions to read clock sources and set the DIV2 clocks *********************/

/**
 * @brief  Get CORE_CLK and SYS_CLK
 */
uint32_t SCG_GetCoreSysClock(void);

/**
 * @brief  Get the frequency of the specified clock source
 */
uint32_t SCG_GetCLOCK(const uint32_t ClockSource);

/**
 * @brief  Set the divider for the specified clock source
 */
void SCG_SetCLOCKDIV2(const uint32_t ClockSource, const uint32_t ClockDivider);

/**
 * @brief  Get the frequency of the specified DIV2 clocks
 */
uint32_t SCG_GetCLOCKDIV2(const uint32_t ClockSource);

#endif /* __MKE16Z4_CLOCK */

/******************************************************************************
 * End of file
 ******************************************************************************/
