#ifndef __MKE16Z4_PORT_GPIO
#define __MKE16Z4_PORT_GPIO

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_def.h"

/******************************************************************************
 * EXPORT TYPEDEF
 ******************************************************************************/
/**
  * @brief PORT Init structure definition
  */
typedef struct
{
    uint32_t Pin;       /*!< Specifies the PORT pins to be configured.
                           This parameter can be any value of @ref PORT_pins_define */

    uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                           This parameter can be a value of @ref PORT_mode_define */

    uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                           This parameter can be a value of @ref PORT_pull_define */

    uint32_t Drive;      /*!< Specifies the Drive strength for the selected pins.
                           This parameter can be a value of @ref PORT_drive_define */

    uint32_t Filter;      /*!< Specifies the Passive Input Filter for the selected pins.
                           This parameter can be a value of @ref PORT_filter_define */

    uint32_t Interrupt;  /*!< Specifies the interrupt mode for the selected pins.
                            This parameter can be a value of @ref PORT_interrupt_define */

    uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins.
                            This parameter can be a value of @ref PORT_Alternate_function_selection */
}PORT_InitTypeDef;

/**
  * @brief  PORT Bit SET and Bit RESET enumeration
  */
typedef enum
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
}GPIO_PinState;

#define IS_GPIO_PIN_ACTION(ACTION) (((ACTION) == GPIO_PIN_RESET) || \
                                    ((ACTION) == GPIO_PIN_SET))
/******************************************************************************
 * EXPORT CONSTANTS
 ******************************************************************************/

/** @defgroup PORT_Instance_define PORT Instances define
  * @{
  */
#define DRIVER_PORTA                     0x00000000U
#define DRIVER_PORTB                     0x00000001U
#define DRIVER_PORTC                     0x00000002U
#define DRIVER_PORTD                     0x00000003U
#define DRIVER_PORTE                     0x00000004U

#define IS_PORT_ALL_INSTANCE(PERIPH)       (((PERIPH) == DRIVER_PORTA) || \
                                            ((PERIPH) == DRIVER_PORTB) || \
                                            ((PERIPH) == DRIVER_PORTC) || \
                                            ((PERIPH) == DRIVER_PORTD) || \
                                            ((PERIPH) == DRIVER_PORTE))
/**
  * @}
  */

/** @defgroup GPIO_Instance_define GPIO Instances define
  * @{
  */
#define DRIVER_GPIOA                     0x00000000U
#define DRIVER_GPIOB                     0x00000001U
#define DRIVER_GPIOC                     0x00000002U
#define DRIVER_GPIOD                     0x00000003U
#define DRIVER_GPIOE                     0x00000004U

#define IS_GPIO_ALL_INSTANCE(PERIPH)       (((PERIPH) == DRIVER_GPIOA) || \
                                            ((PERIPH) == DRIVER_GPIOB) || \
                                            ((PERIPH) == DRIVER_GPIOC) || \
                                            ((PERIPH) == DRIVER_GPIOD) || \
                                            ((PERIPH) == DRIVER_GPIOE))
/**
  * @}
  */

/** @defgroup PORT_pins_define PORT pins define
  * @{
  */
#define PORT_PIN_0                  ((uint32_t)0x00000001)  /* Pin 0 selected    */
#define PORT_PIN_1                  ((uint32_t)0x00000002)  /* Pin 1 selected    */
#define PORT_PIN_2                  ((uint32_t)0x00000004)  /* Pin 2 selected    */
#define PORT_PIN_3                  ((uint32_t)0x00000008)  /* Pin 3 selected    */
#define PORT_PIN_4                  ((uint32_t)0x00000010)  /* Pin 4 selected    */
#define PORT_PIN_5                  ((uint32_t)0x00000020)  /* Pin 5 selected    */
#define PORT_PIN_6                  ((uint32_t)0x00000040)  /* Pin 6 selected    */
#define PORT_PIN_7                  ((uint32_t)0x00000080)  /* Pin 7 selected    */
#define PORT_PIN_8                  ((uint32_t)0x00000100)  /* Pin 8 selected    */
#define PORT_PIN_9                  ((uint32_t)0x00000200)  /* Pin 9 selected    */
#define PORT_PIN_10                 ((uint32_t)0x00000400)  /* Pin 10 selected   */
#define PORT_PIN_11                 ((uint32_t)0x00000800)  /* Pin 11 selected   */
#define PORT_PIN_12                 ((uint32_t)0x00001000)  /* Pin 12 selected   */
#define PORT_PIN_13                 ((uint32_t)0x00002000)  /* Pin 13 selected   */
#define PORT_PIN_14                 ((uint32_t)0x00004000)  /* Pin 14 selected   */
#define PORT_PIN_15                 ((uint32_t)0x00008000)  /* Pin 15 selected   */
#define PORT_PIN_16                 ((uint32_t)0x00010000)  /* Pin 16 selected    */
#define PORT_PIN_17                 ((uint32_t)0x00020000)  /* Pin 17 selected    */
#define PORT_PIN_18                 ((uint32_t)0x00040000)  /* Pin 18 selected    */
#define PORT_PIN_19                 ((uint32_t)0x00080000)  /* Pin 19 selected    */
#define PORT_PIN_20                 ((uint32_t)0x00100000)  /* Pin 20 selected    */
#define PORT_PIN_21                 ((uint32_t)0x00200000)  /* Pin 21 selected    */
#define PORT_PIN_22                 ((uint32_t)0x00400000)  /* Pin 22 selected    */
#define PORT_PIN_23                 ((uint32_t)0x00800000)  /* Pin 23 selected    */
#define PORT_PIN_24                 ((uint32_t)0x01000000)  /* Pin 24 selected    */
#define PORT_PIN_25                 ((uint32_t)0x02000000)  /* Pin 25 selected    */
#define PORT_PIN_26                 ((uint32_t)0x04000000)  /* Pin 26 selected   */
#define PORT_PIN_27                 ((uint32_t)0x08000000)  /* Pin 27 selected   */
#define PORT_PIN_28                 ((uint32_t)0x10000000)  /* Pin 28 selected   */
#define PORT_PIN_29                 ((uint32_t)0x20000000)  /* Pin 29 selected   */
#define PORT_PIN_30                 ((uint32_t)0x40000000)  /* Pin 30 selected   */
#define PORT_PIN_31                 ((uint32_t)0x80000000)  /* Pin 31 selected   */
#define PORT_PIN_All                ((uint32_t)0xFFFFFFFF)  /* All pins selected */

#define PORT_PIN_MASK                0xFFFFFFFFU /* PIN mask for assert test */

#define IS_PORT_PIN(PIN)           (((PIN) & PORT_PIN_MASK ) != (uint32_t)0x00)
/**
  * @}
  */

/** @defgroup GPIO_pins_define GPIO pins define
  * @{
  */
#define GPIO_PIN_0                  ((uint32_t)0x00000001)  /* Pin 0 selected    */
#define GPIO_PIN_1                  ((uint32_t)0x00000002)  /* Pin 1 selected    */
#define GPIO_PIN_2                  ((uint32_t)0x00000004)  /* Pin 2 selected    */
#define GPIO_PIN_3                  ((uint32_t)0x00000008)  /* Pin 3 selected    */
#define GPIO_PIN_4                  ((uint32_t)0x00000010)  /* Pin 4 selected    */
#define GPIO_PIN_5                  ((uint32_t)0x00000020)  /* Pin 5 selected    */
#define GPIO_PIN_6                  ((uint32_t)0x00000040)  /* Pin 6 selected    */
#define GPIO_PIN_7                  ((uint32_t)0x00000080)  /* Pin 7 selected    */
#define GPIO_PIN_8                  ((uint32_t)0x00000100)  /* Pin 8 selected    */
#define GPIO_PIN_9                  ((uint32_t)0x00000200)  /* Pin 9 selected    */
#define GPIO_PIN_10                 ((uint32_t)0x00000400)  /* Pin 10 selected   */
#define GPIO_PIN_11                 ((uint32_t)0x00000800)  /* Pin 11 selected   */
#define GPIO_PIN_12                 ((uint32_t)0x00001000)  /* Pin 12 selected   */
#define GPIO_PIN_13                 ((uint32_t)0x00002000)  /* Pin 13 selected   */
#define GPIO_PIN_14                 ((uint32_t)0x00004000)  /* Pin 14 selected   */
#define GPIO_PIN_15                 ((uint32_t)0x00008000)  /* Pin 15 selected   */
#define GPIO_PIN_16                 ((uint32_t)0x00010000)  /* Pin 16 selected    */
#define GPIO_PIN_17                 ((uint32_t)0x00020000)  /* Pin 17 selected    */
#define GPIO_PIN_18                 ((uint32_t)0x00040000)  /* Pin 18 selected    */
#define GPIO_PIN_19                 ((uint32_t)0x00080000)  /* Pin 19 selected    */
#define GPIO_PIN_20                 ((uint32_t)0x00100000)  /* Pin 20 selected    */
#define GPIO_PIN_21                 ((uint32_t)0x00200000)  /* Pin 21 selected    */
#define GPIO_PIN_22                 ((uint32_t)0x00400000)  /* Pin 22 selected    */
#define GPIO_PIN_23                 ((uint32_t)0x00800000)  /* Pin 23 selected    */
#define GPIO_PIN_24                 ((uint32_t)0x01000000)  /* Pin 24 selected    */
#define GPIO_PIN_25                 ((uint32_t)0x02000000)  /* Pin 25 selected    */
#define GPIO_PIN_26                 ((uint32_t)0x04000000)  /* Pin 26 selected   */
#define GPIO_PIN_27                 ((uint32_t)0x08000000)  /* Pin 27 selected   */
#define GPIO_PIN_28                 ((uint32_t)0x10000000)  /* Pin 28 selected   */
#define GPIO_PIN_29                 ((uint32_t)0x20000000)  /* Pin 29 selected   */
#define GPIO_PIN_30                 ((uint32_t)0x40000000)  /* Pin 30 selected   */
#define GPIO_PIN_31                 ((uint32_t)0x80000000)  /* Pin 31 selected   */
#define GPIO_PIN_All                ((uint32_t)0xFFFFFFFF)  /* All pins selected */

#define GPIO_PIN_MASK                0xFFFFFFFFU /* PIN mask for assert test */

#define IS_GPIO_PIN(PIN)           (((PIN) & GPIO_PIN_MASK ) != (uint32_t)0x00)
/**
  * @}
  */

/** @defgroup PORT_mode_define PORT mode define
  * @{
  */
#define  PORT_MODE_INPUT             0x00000000       /*!< Input Mode              */
#define  PORT_MODE_OUTPUT            0x00000001       /*!< Output Mode             */
#define  PORT_MODE_AF                0x00000003       /*!< Alternate Function Mode */
#define  PORT_MODE_AN                0x00000004       /*!< Analog or disabled mode */

#define IS_PORT_MODE(MODE) (((MODE) == PORT_MODE_INPUT)  || \
                            ((MODE) == PORT_MODE_OUTPUT) || \
                            ((MODE) == PORT_MODE_AF) || \
                            ((MODE) == PORT_MODE_AN))
/**
  * @}
  */

/** @defgroup PORT_pull_define PORT pull define
 * @brief PORT Pull-Up or Pull-Down Activation
 * @{
 */
#define  PORT_PUPD_NOPULL   0x00000000U   /*!< No Pull-up or Pull-down activation */
#define  PORT_PUPD_DOWN     0x00000002U   /*!< Pull-down activation               */
#define  PORT_PUPD_UP       0x00000003U   /*!< Pull-up activation                 */

#define IS_PORT_PUPD_(PUPD) (((PUPD) == PORT_PUPD_NOPULL) || \
                            ((PUPD) == PORT_PUPD_UP) || \
                            ((PUPD) == PORT_PUPD_DOWN))
/**
  * @}
  */

/** @defgroup PORT_drive_define PORT drive define
 * @brief PORT Drive Strength Activation
 * @{
 */
#define  PORT_DRIVE_LOW         0x00000000U   /*!< Low drive strength enabled          */
#define  PORT_DRIVE_HIGH        0x00000001U   /*!< High drive strength enabled         */

#define IS_PORT_DRIVE(DRIVE) (((DRIVE) == PORT_DRIVE_HIGH) || ((DRIVE) == PORT_DRIVE_LOW))
/**
  * @}
  */

/** @defgroup PORT_filter_define PORT filter define
 * @brief PORT Passive Input Filter Activation
 * @{
 */
#define  PORT_FILTER_DISABLED         0x00000000U   /*!< Passive input filter disabled */
#define  PORT_FILTER_ENABLED          0x00000001U   /*!< Passive input filter enabled  */

#define IS_PORT_FILTER(FILTER) (((FILTER) == PORT_FILTER_ENABLED) || \
                                ((FILTER) == PORT_FILTER_DISABLED))
/**
  * @}
  */

/** @defgroup PORT_interrupt_define PORT interrput define
 * @brief PORT Interrupt mode
 * @{
 */
#define  PORT_INTRPT_DISABLED          0x00000000U   /*!< Interrupt disabled        */
#define  PORT_INTRPT_LEVEL_0           0x00000008U   /*!< Interrupt when logic 0    */
#define  PORT_INTRPT_RISING            0x00000009U   /*!< Interrupt on rising edge  */
#define  PORT_INTRPT_FALLING           0x0000000AU   /*!< Interrupt on falling edge */
#define  PORT_INTRPT_RISING_FALLING    0x0000000BU   /*!< Interrupt on both edge    */
#define  PORT_INTRPT_LEVEL_1           0x0000000CU   /*!< Interrupt when logic 1    */


#define IS_PORT_INTRPT(INTRPT) (((INTRPT) == PORT_INTRPT_DISABLED) || ((INTRPT) == PORT_INTRPT_LEVEL_0) || \
                               ((INTRPT) == PORT_INTRPT_LEVEL_1) || ((INTRPT) == PORT_INTRPT_RISING) || \
                               ((INTRPT) == PORT_INTRPT_FALLING) || ((INTRPT) == PORT_INTRPT_RISING_FALLING))
/**
  * @}
  */

/** @defgroup PORT_Alternate_function_selection PORT Alternate Function
 * @brief PORT Alternate function selection
 * @{
 */
#define  PORT_ALT_0           0x00000000U   /*!< Alternate function 0       */
#define  PORT_ALT_1           0x00000001U   /*!< Alternate function 1       */
#define  PORT_ALT_2           0x00000002U   /*!< Alternate function 2       */
#define  PORT_ALT_3           0x00000003U   /*!< Alternate function 3       */
#define  PORT_ALT_4           0x00000004U   /*!< Alternate function 4       */
#define  PORT_ALT_5           0x00000005U   /*!< Alternate function 5       */
#define  PORT_ALT_6           0x00000006U   /*!< Alternate function 6       */
#define  PORT_ALT_7           0x00000007U   /*!< Alternate function 7       */

#define IS_PORT_ALT(ALT) (((ALT) == PORT_ALT_0) || \
                          ((ALT) == PORT_ALT_1) || \
                          ((ALT) == PORT_ALT_2) || \
                          ((ALT) == PORT_ALT_3) || \
                          ((ALT) == PORT_ALT_4) || \
                          ((ALT) == PORT_ALT_5) || \
                          ((ALT) == PORT_ALT_6) || \
                          ((ALT) == PORT_ALT_7))
/**
  * @}
  */
/******************************************************************************
 * EXPORT MACROS
 ******************************************************************************/

/******************************************************************************
 * EXPORT VARIABLES
 ******************************************************************************/

/******************************************************************************
 * EXPORT FUNCTIONS PROTOTYPES
 ******************************************************************************/

/* Initialization and de-initialization functions *****************************/
/**
 * @brief  Initializes the PORTx peripheral according to the specified parameters in the PORT_Init.
 */
void PORT_Init(const uint32_t DRIVER_PORTx , const PORT_InitTypeDef *const PORT_Init);

/* IO operation functions *****************************************************/
/**
 * @brief  Reads the specified input port pin.
 */
GPIO_PinState GPIO_ReadPin (const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin);

/**
  * @brief  Sets or clears the selected data port bit.
  */
void GPIO_WritePin(const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin, const GPIO_PinState PinState);

/**
  * @brief  Toggles the selected data port bit.
  */
void GPIO_TogglePin(const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin);

/**
 * @brief  Get Interrupt flag of a pin
 */
ITStatus GPIO_GetIT(const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin);

/**
 * @brief  Clear Interrupt flag of a pin
 */
void GPIO_ClearITPending(const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin);

/**
 * @brief  Register callback function for interrupt
 */
void PORTAE_RegisterCallback(void (*callbackFunc)(void));

/**
 * @brief  Register callback function for interrupt
 */
void PORTBCD_RegisterCallback(void (*callbackFunc)(void));

#endif /* ___MKE16Z4_PORT_GPIO */

/******************************************************************************
 * End of file
 ******************************************************************************/
