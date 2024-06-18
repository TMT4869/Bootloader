/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_hal_port_gpio.h"
#include "mke16z4_port_gpio.h"
#include "mke16z4_clock.h"
#include <assert.h>
/******************************************************************************
 * PRIVATE TYPEDEF
 ******************************************************************************/

/******************************************************************************
 * PRIVATE CONSTANTS
 ******************************************************************************/
#define PORT_PINS               (32U)           /*!< Number of pins each port */
#define PORT_INSTANCES          (5U)            /*!< Number of ports          */
/******************************************************************************
 * PRIVATE MACROS
 ******************************************************************************/

/******************************************************************************
 * PRIVATE VARIABLES
 ******************************************************************************/
/* Base address of each ports */
static PORT_Type *const portPtrs[PORT_INSTANCES] = PORT_BASE_PTRS;
static GPIO_Type *const gpioPtrs[PORT_INSTANCES] = GPIO_BASE_PTRS;
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 ******************************************************************************/
static void PORT_Default_IRQHandler(void);
/* Callback function pointer */
static void (*pISR_PORTAE_Callback)(void)  = PORT_Default_IRQHandler;
static void (*pISR_PORTBCD_Callback)(void) = PORT_Default_IRQHandler;
/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/**
  * @brief  Initializes the PORTx peripheral according to the specified parameters in the PORT_Init.
  * @param  PORTx: where x can be (A..E) to select the PORT peripherals
  * @param  PORT_Init: pointer to a PORT_InitTypeDef structure that contains
  *         the configuration information for the specified PORT peripheral.
  * @retval None
  */
void PORT_Init(const uint32_t DRIVER_PORTx , const PORT_InitTypeDef *const PORT_Init)
{
    uint32_t pinPos = 0x00;
    uint32_t pinCurrent = 0x00;
    uint32_t pinMask = 0x00;
    PORT_Type *PORTx = NULL;
    GPIO_Type *GPIOx = NULL;
    /* Check the parameters */
    assert(IS_PORT_ALL_INSTANCE(DRIVER_PORTx));
    assert(IS_PORT_PIN(PORT_Init->Pin));
    assert(IS_PORT_MODE(PORT_Init->Mode));
    assert(IS_PORT_INTRPT(PORT_Init->Interrupt));
    assert(IS_PORT_ALT(PORT_Init->Alternate));
    /* PORT and GPIO Type */
    PORTx = portPtrs[DRIVER_PORTx];
    GPIOx = gpioPtrs[DRIVER_PORTx];
    for (pinPos = 0x0; pinPos < PORT_PINS; pinPos++)
    {
        pinMask = BIT_POS(pinPos);
        pinCurrent = (uint32_t)PORT_Init->Pin & pinMask;
        if (pinCurrent == pinMask)
        {
            /*--------------------- GPIO Mode Configuration ------------------------*/
            if (PORT_Init->Mode == PORT_MODE_INPUT)
            {
                /* Pull-up Pull-down configuration */
                assert(IS_PORT_PUPD_(PORT_Init->Pull));
                __HAL_PORT_PUPD_CONFIG(PORTx, pinPos, PORT_Init->Pull);
                /* Passive Filter configuration */
                assert(IS_PORT_FILTER(PORT_Init->Filter));
                __HAL_PORT_FILTER_CONFIG(PORTx, pinPos, PORT_Init->Filter);
            }
            else if (PORT_Init->Mode == PORT_MODE_OUTPUT)
            {
                /* Drive strenth configuration */
                assert(IS_PORT_DRIVE(PORT_Init->Drive));
                __HAL_PORT_DRIVE_CONFIG(PORTx, pinPos, PORT_Init->Drive);
            }
            /* PORT Mode configuration */
            __HAL_PORT_MUX_CONFIG(PORTx, pinPos, PORT_Init->Alternate);
            /* Interrupt configuration */
            __HAL_PORT_INTRPT_CONFIG(PORTx, pinPos, PORT_Init->Interrupt);
            /* Clear ISF */
            __HAL_PORT_CLEAR_INTRPT_PENDING(PORTx, pinCurrent);
            /*----------------- GPIO Drirection Configuration --------------------*/
            if ((PORT_Init->Mode == PORT_MODE_INPUT) || (PORT_Init->Mode == PORT_MODE_OUTPUT))
            {
                __HAL_GPIO_MODE_CONFIG(GPIOx, pinPos, PORT_Init->Mode);
            }
        }
    }
}

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx where x can be (A..E) to select the GPIO peripheral
  * @param  GPIO_Pin specifies the port bit to read.
  *         This parameter can be GPIO_PIN_x where x can be (0..31).
  * @retval The input port pin value.
  */
GPIO_PinState GPIO_ReadPin (const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin)
{
    GPIO_PinState bitstatus = GPIO_PIN_RESET;
    GPIO_Type *GPIOx = NULL;
    /* Check the parameters */
    assert(IS_GPIO_PIN(GPIO_Pin));
    assert(IS_GPIO_ALL_INSTANCE(DRIVER_GPIOx));
    /* Read bit state */
    GPIOx = gpioPtrs[DRIVER_GPIOx];
    if(__HAL_GPIO_GET_STATE(GPIOx, GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        bitstatus = GPIO_PIN_SET;
    }
    else
    {
        bitstatus = GPIO_PIN_RESET;
    }
    return bitstatus;
}

/**
  * @brief  Sets or clears the selected data port bit.
  *
  * @note   This function uses GPIOx_PSOR and PIOx_PCOR register to allow atomic read/modify
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  *
  * @param  GPIOx where x can be (A..E) to select the GPIO peripheral
  * @param  GPIO_Pin specifies the port bit to be written.
  *          This parameter can be one of GPIO_PIN_x where x can be (0..31).
  * @param  PinState specifies the value to be written to the selected bit.
  *          This parameter can be one of the GPIO_PinState enum values:
  *            @arg GPIO_PIN_RESET: to clear the port pin
  *            @arg GPIO_PIN_SET: to set the port pin
  * @retval None
  */
void GPIO_WritePin(const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin, const GPIO_PinState PinState)
{
    GPIO_Type *GPIOx = NULL;
    /* Check the parameters */
    assert(IS_GPIO_ALL_INSTANCE(DRIVER_GPIOx));
    assert(IS_GPIO_PIN(GPIO_Pin));
    assert(IS_GPIO_PIN_ACTION(PinState));
    /*Set pin */
    GPIOx = gpioPtrs[DRIVER_GPIOx];
    if(PinState != GPIO_PIN_RESET)
    {
        __HAL_GPIO_SET_PIN(GPIOx, GPIO_Pin);
    }
    else
    {
        __HAL_GPIO_CLEAR_PIN(GPIOx, GPIO_Pin);
    }
}

/**
  * @brief  Toggles the specified GPIO pins.
  * @param  GPIOx Where x can be (A..E) to select the GPIO peripheral
  * @param  GPIO_Pin Specifies the pins to be toggled.
  * @retval None
  */
void GPIO_TogglePin(const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin)
{
    GPIO_Type *GPIOx = NULL;
    /* Check the parameters */
    assert(IS_GPIO_ALL_INSTANCE(DRIVER_GPIOx));
    assert(IS_GPIO_PIN(GPIO_Pin));
    GPIOx = gpioPtrs[DRIVER_GPIOx];
    __HAL_GPIO_TOGGLE_PIN(GPIOx, GPIO_Pin);
}

/**
  * @brief  Check whether the interrupt pending bit of a pin is set or not
  * @param  GPIOx Where x can be (A..E) to select the GPIO peripheral
  * @param  GPIO_Pin Specifies the pins to be read.
  * @retval Interrupt status
  */
ITStatus GPIO_GetIT(const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin)
{
    PORT_Type *PORTx     = NULL;
    ITStatus   status    = RESET;
    /* Check the parameters */
    assert(IS_GPIO_ALL_INSTANCE(DRIVER_GPIOx));
    /* Calculate the index */
    PORTx = portPtrs[DRIVER_GPIOx];
    /* Get status */
    if (__HAL_PORT_GET_INTRPT(PORTx, GPIO_Pin) != (uint32_t)RESET)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }
    return status;
}

/**
  * @brief  Clear the interrupt pending bit of a pin
  * @param  GPIOx Where x can be (A..E) to select the GPIO peripheral
  * @param  GPIO_Pin Specifies the pins to have its interrupt pending bit cleared.
  * @retval None
  */
void GPIO_ClearITPending(const uint32_t DRIVER_GPIOx , const uint32_t GPIO_Pin)
{
    PORT_Type *PORTx     = NULL;
    /* Check the parameters */
    assert(IS_GPIO_ALL_INSTANCE(DRIVER_GPIOx));
    assert(IS_GPIO_PIN(GPIO_Pin));
    /* Calculate the index */
    PORTx = portPtrs[DRIVER_GPIOx];
    /* Clear pending bit */
    __HAL_PORT_CLEAR_INTRPT_PENDING(PORTx, GPIO_Pin);
}

/**
 * @brief  Register callback function for interrupt handlers
 * @param callbackFunc Function pointer of the callback function
 */
void PORTAE_RegisterCallback(void (*callbackFunc)(void))
{
    pISR_PORTAE_Callback = callbackFunc;
}

/**
 * @brief  Register callback function for interrupt handlers
 * @param callbackFunc Function pointer of the callback function
 */
void PORTBCD_RegisterCallback(void (*callbackFunc)(void))
{
    pISR_PORTBCD_Callback = callbackFunc;
}

/**
 * @brief  Empty function as default handler
 * @param  None
 * @retval None
 */
static void PORT_Default_IRQHandler(void)
{
    /* Empty function */
}

/**
 * @brief  Port A and E Handler
 * @param  None
 * @retval None
 */
void PORTAE_IRQHandler (void)
{
    pISR_PORTAE_Callback();
}

/**
 * @brief  Port B to D Handler
 * @param  None
 * @retval None
 */
void PORTBCD_IRQHandler(void)
{
    pISR_PORTBCD_Callback();
}

/******************************************************************************
 * End of file
 ******************************************************************************/
