#ifndef __MKE16Z4_HAL_PORT_GPIO
#define __MKE16Z4_HAL_PORT_GPIO

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "MKE16Z4.h"
/******************************************************************************
 * EXPORTED TYPEDEF
 ******************************************************************************/

/**
 * @brief
 */


/******************************************************************************
 * EXPORTED CONSTANTS
 ******************************************************************************/

/** @defgroup
 * @{
 */

/**
 * @}
 */

/******************************************************************************
 * EXPORTED MACROS
 ******************************************************************************/
/** @defgroup PORT_GPIO_Exported_Macros PORT and GPIO Exported Macros
  * @{
  */

/** @defgroup PORT_PullUp_PullDown_Config
  * @brief  Config the mode of Pull up pull down resistors
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0U
  *                  @arg  1U
  *                  @arg  .
  *                  @arg  .
  *                  @arg  .
  *                  @arg  29U
  *                  @arg  30U
  *                  @arg  31U
  * @param __PUPUD__ specifies the pull up pull down mode
  *        This parameter can be one of the following values:
  *                  @arg   0b00..No pull up pull down
  *                  @arg   0b10..Pull down
  *                  @arg   0b11..Pull up
  * @{
  */
#define __HAL_PORT_PUPD_CONFIG(__INSTANCE__, __PIN__, __PUPD__) \
                    (__INSTANCE__->PCR[__PIN__] = (__INSTANCE__->PCR[__PIN__] & ~(PORT_PCR_PE_MASK | PORT_PCR_PS_MASK)) \
                                                | (__PUPD__ << PORT_PCR_PS_SHIFT))
/**
  * @}
  */

/** @defgroup PORT_Filter_Config
  * @brief  Config the mode of passive filter
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0U
  *                  @arg  1U
  *                  @arg  .
  *                  @arg  .
  *                  @arg  .
  *                  @arg  29U
  *                  @arg  30U
  *                  @arg  31U
  * @param __FILTER__ specifies the filter mode
  *        This parameter can be one of the following values:
  *                  @arg   0b0..Passive filter disable
  *                  @arg   0b1..Passive filter enable
  * @{
  */
#define __HAL_PORT_FILTER_CONFIG(__INSTANCE__, __PIN__, __FILTER__) \
                    (__INSTANCE__->PCR[__PIN__] = (__INSTANCE__->PCR[__PIN__] & ~(PORT_PCR_PFE_MASK)) \
                                                | (PORT_PCR_PFE(__FILTER__)))
/**
  * @}
  */

/** @defgroup PORT_Drive_Config
  * @brief  Config the mode of drive strength
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0U
  *                  @arg  1U
  *                  @arg  .
  *                  @arg  .
  *                  @arg  .
  *                  @arg  29U
  *                  @arg  30U
  *                  @arg  31U
  * @param __DRIVE__ specifies the filter mode
  *        This parameter can be one of the following values:
  *                  @arg   0b0..Low drive strength
  *                  @arg   0b1..High drive strength
  * @{
  */
#define __HAL_PORT_DRIVE_CONFIG(__INSTANCE__, __PIN__, __DRIVE__) \
                    (__INSTANCE__->PCR[__PIN__] = (__INSTANCE__->PCR[__PIN__] & ~(PORT_PCR_DSE_MASK)) \
                                                | (PORT_PCR_DSE(__DRIVE__)))
/**
  * @}
  */

/** @defgroup PORT_Mux_Config
  * @brief  Config the muxing modes
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0U
  *                  @arg  1U
  *                  @arg  .
  *                  @arg  .
  *                  @arg  .
  *                  @arg  29U
  *                  @arg  30U
  *                  @arg  31U
  * @param __MUX__ specifies the muxing configuration
  *        This parameter can be one of the following values:
  *                  @arg  0b000..Pin disabled (Alternative 0) (analog).
  *                  @arg  0b001..Alternative 1 (GPIO).
  *                  @arg  0b010..Alternative 2 (chip-specific).
  *                  @arg  0b011..Alternative 3 (chip-specific).
  *                  @arg  0b100..Alternative 4 (chip-specific).
  *                  @arg  0b101..Alternative 5 (chip-specific).
  *                  @arg  0b110..Alternative 6 (chip-specific).
  *                  @arg  0b111..Alternative 7 (chip-specific).
  * @{
  */
#define __HAL_PORT_MUX_CONFIG(__INSTANCE__, __PIN__, __MUX__) \
                    (__INSTANCE__->PCR[__PIN__] = (__INSTANCE__->PCR[__PIN__] & ~(PORT_PCR_MUX_MASK)) \
                                                | (PORT_PCR_MUX(__MUX__)))
/**
  * @}
  */

/** @defgroup PORT_Interrupt_Config
  * @brief  Config the interrupt mode
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0U
  *                  @arg  1U
  *                  @arg  .
  *                  @arg  .
  *                  @arg  .
  *                  @arg  29U
  *                  @arg  30U
  *                  @arg  31U
  * @param __IT__ specifies the interrupt configuration
  *        This parameter can be one of the following values:
  *                  @arg  0b0000..Interrupt Status Flag (ISF) is disabled.
  *                  @arg  0b0100..Reserved.
  *                  @arg  0b0101..Reserved.
  *                  @arg  0b0110..Reserved.
  *                  @arg  0b0111..Reserved.
  *                  @arg  0b1000..ISF flag and Interrupt when logic 0.
  *                  @arg  0b1001..ISF flag and Interrupt on rising-edge.
  *                  @arg  0b1010..ISF flag and Interrupt on falling-edge.
  *                  @arg  0b1011..ISF flag and Interrupt on either edge.
  *                  @arg  0b1100..ISF flag and Interrupt when logic 1.
  *                  @arg  0b1101..Reserved.
  *                  @arg  0b1110..Reserved.
  *                  @arg  0b1111..Reserved.
  * @{
  */
#define __HAL_PORT_INTRPT_CONFIG(__INSTANCE__, __PIN__, __IT__) \
                    (__INSTANCE__->PCR[__PIN__] = (__INSTANCE__->PCR[__PIN__] & ~(PORT_PCR_IRQC_MASK)) \
                                                | (PORT_PCR_IRQC(__IT__)))
/**
  * @}
  */

/** @defgroup PORT_PullUp_PullDown_Config
  * @brief  Config the mode of Pull up pull down resistors
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0U
  *                  @arg  1U
  *                  @arg  .
  *                  @arg  .
  *                  @arg  .
  *                  @arg  29U
  *                  @arg  30U
  *                  @arg  31U
  * @param __FILTER__ specifies the filter mode
  *        This parameter can be one of the following values:
  *                  @arg   0b0..Passive filter disable
  *                  @arg   0b1..Passive filter enable
  * @{
  */

/** @defgroup PORT_IT_Control
  * @brief  Get or clear the IT pending
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0b00000000000000000000000000000001..Pin 1
  *                  @arg  0b00000000000000000000000000000010..Pin 2
  *                  @arg  0b00000000000000000000000000000100..Pin 3
  *                  @arg  0b00000000000000000000000000001000..Pin 4
  *                  @arg  0b00000000000000000000000000010000..Pin 5
  *                  @arg  0b00000000000000000000000000100000..Pin 6
  *                  @arg  0b00000000000000000000000001000000..Pin 7
  *                  @arg  0b00000000000000000000000010000000..Pin 8
  *                  @arg  0b00000000000000000000000100000000..Pin 9
  *                  @arg  0b00000000000000000000001000000000..Pin 10
  *                  @arg  0b00000000000000000000010000000000..Pin 11
  *                  @arg  0b00000000000000000000100000000000..Pin 12
  *                  @arg  0b00000000000000000001000000000000..Pin 13
  *                  @arg  0b00000000000000000010000000000000..Pin 14
  *                  @arg  0b00000000000000000100000000000000..Pin 15
  *                  @arg  0b00000000000000001000000000000000..Pin 16
  *                  @arg  0b00000000000000010000000000000000..Pin 17
  *                  @arg  0b00000000000000100000000000000000..Pin 18
  *                  @arg  0b00000000000001000000000000000000..Pin 19
  *                  @arg  0b00000000000010000000000000000000..Pin 20
  *                  @arg  0b00000000000100000000000000000000..Pin 21
  *                  @arg  0b00000000001000000000000000000000..Pin 22
  *                  @arg  0b00000000010000000000000000000000..Pin 23
  *                  @arg  0b00000000100000000000000000000000..Pin 24
  *                  @arg  0b00000001000000000000000000000000..Pin 25
  *                  @arg  0b00000010000000000000000000000000..Pin 26
  *                  @arg  0b00000100000000000000000000000000..Pin 27
  *                  @arg  0b00001000000000000000000000000000..Pin 28
  *                  @arg  0b00010000000000000000000000000000..Pin 29
  *                  @arg  0b00100000000000000000000000000000..Pin 30
  *                  @arg  0b01000000000000000000000000000000..Pin 31
  *                  @arg  0b10000000000000000000000000000000..Pin 32
  * @{
  */
#define __HAL_PORT_GET_INTRPT(__INSTANCE__, __PIN__) \
                    (__INSTANCE__->ISFR & __PIN__)

#define __HAL_PORT_CLEAR_INTRPT_PENDING(__INSTANCE__, __PIN__) \
                    (__INSTANCE__->ISFR = __PIN__)
/**
  * @}
  */

/** @defgroup GPIO_Mode_Config
  * @brief  Config the mode of GPIO
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0U
  *                  @arg  1U
  *                  @arg  .
  *                  @arg  .
  *                  @arg  .
  *                  @arg  29U
  *                  @arg  30U
  *                  @arg  31U
  * @param __MODE__ specifies the I/O mode
  *        This parameter can be one of the following values:
  *                  @arg   0b0..Input
  *                  @arg   0b1..Output
  * @{
  */
#define __HAL_GPIO_MODE_CONFIG(__INSTANCE__, __PIN__, __MODE__) \
                    (__INSTANCE__->PDDR = (__INSTANCE__->PDDR & ~(1U << __PIN__)) \
                                        | (uint32_t)(__MODE__ << __PIN__))
/**
  * @}
  */

/** @defgroup GPIO_Pin_Control
  * @brief  Get, set, clear and toggle pin
  * @param  __INSTANCE__ specifies the instance of PORT
  *         This parameter can be one of the following values:
  *                  @arg  PORTA
  *                  @arg  PORTB
  *                  @arg  PORTC
  *                  @arg  PORTD
  *                  @arg  PORTE
  * @param __PIN__ specifies the pin of PORT
  *        This parameter can be one of the following values:
  *                  @arg  0b00000000000000000000000000000001..Pin 1
  *                  @arg  0b00000000000000000000000000000010..Pin 2
  *                  @arg  0b00000000000000000000000000000100..Pin 3
  *                  @arg  0b00000000000000000000000000001000..Pin 4
  *                  @arg  0b00000000000000000000000000010000..Pin 5
  *                  @arg  0b00000000000000000000000000100000..Pin 6
  *                  @arg  0b00000000000000000000000001000000..Pin 7
  *                  @arg  0b00000000000000000000000010000000..Pin 8
  *                  @arg  0b00000000000000000000000100000000..Pin 9
  *                  @arg  0b00000000000000000000001000000000..Pin 10
  *                  @arg  0b00000000000000000000010000000000..Pin 11
  *                  @arg  0b00000000000000000000100000000000..Pin 12
  *                  @arg  0b00000000000000000001000000000000..Pin 13
  *                  @arg  0b00000000000000000010000000000000..Pin 14
  *                  @arg  0b00000000000000000100000000000000..Pin 15
  *                  @arg  0b00000000000000001000000000000000..Pin 16
  *                  @arg  0b00000000000000010000000000000000..Pin 17
  *                  @arg  0b00000000000000100000000000000000..Pin 18
  *                  @arg  0b00000000000001000000000000000000..Pin 19
  *                  @arg  0b00000000000010000000000000000000..Pin 20
  *                  @arg  0b00000000000100000000000000000000..Pin 21
  *                  @arg  0b00000000001000000000000000000000..Pin 22
  *                  @arg  0b00000000010000000000000000000000..Pin 23
  *                  @arg  0b00000000100000000000000000000000..Pin 24
  *                  @arg  0b00000001000000000000000000000000..Pin 25
  *                  @arg  0b00000010000000000000000000000000..Pin 26
  *                  @arg  0b00000100000000000000000000000000..Pin 27
  *                  @arg  0b00001000000000000000000000000000..Pin 28
  *                  @arg  0b00010000000000000000000000000000..Pin 29
  *                  @arg  0b00100000000000000000000000000000..Pin 30
  *                  @arg  0b01000000000000000000000000000000..Pin 31
  *                  @arg  0b10000000000000000000000000000000..Pin 32
  * @{
  */
#define __HAL_GPIO_GET_STATE(__INSTANCE__, __PIN__) \
                    (__INSTANCE__->PDIR & __PIN__)

#define __HAL_GPIO_SET_PIN(__INSTANCE__, __PIN__) \
                    (__INSTANCE__->PSOR = __PIN__)

#define __HAL_GPIO_CLEAR_PIN(__INSTANCE__, __PIN__) \
                    (__INSTANCE__->PCOR = __PIN__)

#define __HAL_GPIO_TOGGLE_PIN(__INSTANCE__, __PIN__) \
                    (__INSTANCE__->PTOR = __PIN__)
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

#endif /* __MKE16Z4_HAL_PORT_GPIO */

/******************************************************************************
 * End of file
 ******************************************************************************/
