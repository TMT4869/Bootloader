#ifndef __MKE16Z4_HAL_LPUART_
#define __MKE16Z4_HAL_LPUART_

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

/******************************************************************************
 * EXPORTED MACROS
 ******************************************************************************/

/** @defgroup LPUART_Enable_Macros
  * @brief  Enable or disable Tx and Rx
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @{
  */
#define __HAL_LPUART_DISABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL &= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK))

#define __HAL_LPUART_RX_ENABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL |= LPUART_CTRL_RE_MASK)

#define __HAL_LPUART_RX_DISABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL &= (LPUART_CTRL_RE_MASK))

#define __HAL_LPUART_TX_ENABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL |= LPUART_CTRL_TE_MASK)

#define __HAL_LPUART_TX_DISABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL &= (LPUART_CTRL_TE_MASK))
/**
  * @}
  */

/** @defgroup LPUART_DataBit_Config
  * @brief  Config the number of data bit (default is 8)
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @{
  */
#define __HAL_LPUART_10BIT_DATA_ENABLE(__INSTANCE__) \
                    (__INSTANCE__->BAUD |= LPUART_BAUD_M10_MASK)

#define __HAL_LPUART_10BIT_DATA_DISABLE(__INSTANCE__) \
                    (__INSTANCE__->BAUD &= ~(LPUART_BAUD_M10_MASK))

#define __HAL_LPUART_9BIT_DATA_ENABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL |= LPUART_CTRL_M_MASK)

#define __HAL_LPUART_9BIT_DATA_DISABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL &= ~LPUART_CTRL_M_MASK)

#define __HAL_LPUART_7BIT_DATA_ENABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL |= LPUART_CTRL_M7_MASK)

#define __HAL_LPUART_7BIT_DATA_DISABLE(__INSTANCE__) \
                    (__INSTANCE__->CTRL &= ~LPUART_CTRL_M7_MASK)

/**
  * @}
  */

/** @defgroup LPUART_StopBits_Config
  * @brief  Config the stop bits of LPUART
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @param __STOPBITS__ specifies the stopbits
  *         This parameter can be one of the following values:
  *                  @arg  0b0..One stop bit.
  *                  @arg  0b1..Two stop bits.
  * @{
  */
#define __HAL_LPUART_STOPBITS_CONFIG(__INSTANCE__, __STOPBITS__) \
                    (__INSTANCE__->BAUD = (__INSTANCE__->BAUD & ~(LPUART_BAUD_SBNS_MASK)) \
                                        | LPUART_BAUD_SBNS(__STOPBITS__))
/**
  * @}
  */

/** @defgroup LPUART_Oversampling_Config
  * @brief  Config the Oversampling ratio of LPUART
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @param __OSR__ specifies the oversampling ratio
  *         This parameter can be one of the following values:
  *                  @arg  0b00000..Writing 0 to this field will result in an oversampling ratio of 16
  *                  @arg  0b00001..Reserved
  *                  @arg  0b00010..Reserved
  *                  @arg  0b00011..Oversampling ratio of 4, requires BOTHEDGE to be set.
  *                  @arg  0b00100..Oversampling ratio of 5, requires BOTHEDGE to be set.
  *                  @arg  0b00101..Oversampling ratio of 6, requires BOTHEDGE to be set.
  *                  @arg  0b00110..Oversampling ratio of 7, requires BOTHEDGE to be set.
  *                  @arg  0b00111..Oversampling ratio of 8.
  *                  @arg  0b01000..Oversampling ratio of 9.
  *                  @arg  0b01001..Oversampling ratio of 10.
  *                  @arg  0b01010..Oversampling ratio of 11.
  *                  @arg  0b01011..Oversampling ratio of 12.
  *                  @arg  0b01100..Oversampling ratio of 13.
  *                  @arg  0b01101..Oversampling ratio of 14.
  *                  @arg  0b01110..Oversampling ratio of 15.
  *                  @arg  0b01111..Oversampling ratio of 16.
  *                  @arg  0b10000..Oversampling ratio of 17.
  *                  @arg  0b10001..Oversampling ratio of 18.
  *                  @arg  0b10010..Oversampling ratio of 19.
  *                  @arg  0b10011..Oversampling ratio of 20.
  *                  @arg  0b10100..Oversampling ratio of 21.
  *                  @arg  0b10101..Oversampling ratio of 22.
  *                  @arg  0b10110..Oversampling ratio of 23.
  *                  @arg  0b10111..Oversampling ratio of 24.
  *                  @arg  0b11000..Oversampling ratio of 25.
  *                  @arg  0b11001..Oversampling ratio of 26.
  *                  @arg  0b11010..Oversampling ratio of 27.
  *                  @arg  0b11011..Oversampling ratio of 28.
  *                  @arg  0b11100..Oversampling ratio of 29.
  *                  @arg  0b11101..Oversampling ratio of 30.
  *                  @arg  0b11110..Oversampling ratio of 31.
  *                  @arg  0b11111..Oversampling ratio of 32.
  * @{
  */
#define __HAL_LPUART_OVERSAMPLING_CONFIG(__INSTANCE__, __OSR__) \
                    (__INSTANCE__->BAUD = (__INSTANCE__->BAUD & ~(LPUART_BAUD_OSR_MASK)) \
                                        | LPUART_BAUD_OSR(__OSR__))
/**
  * @}
  */

/** @defgroup LPUART_ModuloDivisor_Config
  * @brief  Config the divisor from baud rate generator of LPUART
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @param __DIV__ specifies the divisor
  *         This parameter can be from 1 to 8191
  * @{
  */
#define __HAL_LPUART_BAUDRATE_DIVISOR_CONFIG(__INSTANCE__, __DIV__) \
                    (__INSTANCE__->BAUD = (__INSTANCE__->BAUD & ~(LPUART_BAUD_SBR_MASK)) \
                                        | LPUART_BAUD_SBR(__DIV__))
/**
  * @}
  */

/** @defgroup LPUART_ModuloDivisor_Config
  * @brief  Config the divisor from baud rate generator of LPUART
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @param __DIV__ specifies the divisor
  *         This parameter can be from 1 to 8191
  * @{
  */
#define __HAL_LPUART_SAMPLING_BOTHEDGE_ENABLE(__INSTANCE__) \
                    (__INSTANCE__->BAUD |= LPUART_BAUD_BOTHEDGE_MASK)

#define __HAL_LPUART_SAMPLING_BOTHEDGE_DISABLE(__INSTANCE__) \
                    (__INSTANCE__->BAUD &= ~(LPUART_BAUD_BOTHEDGE_MASK))
/**
  * @}
  */

/** @defgroup LPUART_Parity_Config
  * @brief  Config the parity type of LPUART
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @param __PARITY__ specifies parity type
  *         This parameter can be one of the following values:
  *                  @arg  0b00..No parity.
  *                  @arg  0b10..Even parity.
  *                  @arg  0b11..Odd parity.
  * @{
  */
#define __HAL_LPUART_PARITY_CONFIG(__INSTANCE__, __PARITY__) \
                    (__INSTANCE__->CTRL = (__INSTANCE__->CTRL & ~(LPUART_CTRL_PE_MASK | LPUART_CTRL_PT_MASK)) \
                                        | (__PARITY__ << LPUART_CTRL_PT_SHIFT))
/**
  * @}
  */

/** @defgroup LPUART_Loops_Config
  * @brief  Config the Loops mode of LPUART
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @param __MODE__ specifies the loops mode
  *         This parameter can be one of the following values:
  *                  @arg  0x80..Loopback
  *                  @arg  0xA0..Halfduplex
  * @{
  */
#define __HAL_LPUART_LOOPMODE_CONFIG(__INSTANCE__, __MODE__) \
                    (__INSTANCE__->CTRL = (__INSTANCE__->CTRL & ~(LPUART_CTRL_LOOPS_MASK | LPUART_CTRL_RSRC_MASK)) \
                                        | (uint32_t)(__MODE__))
/**
  * @}
  */

/** @defgroup LPUART_Interrupt_Config
  * @brief  Config the interrupts of LPUART
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @param __INTERRUPT__ specifies the interrupt
  *         This parameter can be one of the following values:
  *                  @arg  0x00004000U..UART_IT_MA2IE
  *                  @arg  0x00008000U..UART_IT_MA1IE
  *                  @arg  0x01000000U..UART_IT_PEIE
  *                  @arg  0x02000000U..UART_IT_FEIE
  *                  @arg  0x04000000U..UART_IT_NEIE
  *                  @arg  0x08000000U..UART_IT_ORIE
  *                  @arg  0x00100000U..UART_IT_ILIE
  *                  @arg  0x00200000U..UART_IT_RIE
  *                  @arg  0x00400000U..UART_IT_TIE
  *                  @arg  0x00800000U..UART_IT_TCIE
  * @{
  */
#define __HAL_LPUART_INTRPT_ENABLE(__INSTANCE__, __INTERRUPT__) \
                    (__INSTANCE__->CTRL = (__INSTANCE__->CTRL & ~(__INTERRUPT__)) \
                                        | (__INTERRUPT__))

#define __HAL_LPUART_INTRPT_DISABLE(__INSTANCE__, __INTERRUPT__) \
                    (__INSTANCE__->CTRL &= ~(__INTERRUPT__))
/**
  * @}
  */

/** @defgroup LPUART_Flag_Control
  * @brief  Get and clear the flag of LPUART
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @param __FLAG__ specifies the flag
  *         This parameter can be one of the following values:
  *                  @arg  0x00004000U..UART_FLAG_MA2IF
  *                  @arg  0x00008000U..UART_FLAG_MA1IF
  *                  @arg  0x00010000U..UART_FLAG_PF
  *                  @arg  0x00020000U..UART_FLAG_FE
  *                  @arg  0x00040000U..UART_FLAG_NF
  *                  @arg  0x00080000U..UART_FLAG_OR
  *                  @arg  0x00100000U..UART_FLAG_IDLE
  *                  @arg  0x40000000U..UART_FLAG_RXEDGIF
  *                  @arg  0x80000000U..UART_FLAG_LBKDIF
  *                  Cannot write one to clear flags
  *                  @arg  0x00200000U..UART_FLAG_RDRF
  *                  @arg  0x00400000U..UART_FLAG_TDRE
  *                  @arg  0x00800000U..UART_FLAG_TC
  *                  @arg  0x01000000U..UART_FLAG_RAF
  * @{
  */
#define __HAL_LPUART_GET_FLAG(__INSTANCE__, __FLAG__) \
                    ((__INSTANCE__->STAT & __FLAG__) == (__FLAG__))

#define __HAL_LPUART_CLEAR_FLAG(__INSTANCE__, __FLAG__) \
                    (__INSTANCE__->STAT |= (__FLAG__))
/**
  * @}
  */

/** @defgroup LPUART_Data_RW
  * @brief  Read and write to data buffer
  * @param  __INSTANCE__ specifies the instance of LPUART
  *         This parameter can be one of the following values:
  *                  @arg  LPUART0
  *                  @arg  LPUART1
  *                  @arg  LPUART2
  * @{
  */
#define __HAL_LPUART_READ_DATA_BUFFER(__INSTANCE__) \
                    (__INSTANCE__->DATA & 0x03FF)

#define __HAL_LPUART_WRITE_DATA_BUFFER(__INSTANCE__, __DATA__) \
                    (__INSTANCE__->DATA = (__INSTANCE__->DATA & ~(0x03FF)) \
                                        | (__DATA__ & (0x03FF)))
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

#endif /* __MKE16Z4_HAL_LPUART_ */


/******************************************************************************
 * End of file
 ******************************************************************************/
