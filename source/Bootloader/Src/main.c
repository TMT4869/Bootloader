/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_port_gpio.h"
#include "mke16z4_core.h"
#include "mke16z4_lpuart.h"
#include "mke16z4_clock.h"
#include "mke16z4_flash.h"
#include "emb_utility.h"
#include "emb_queue.h"
#include "emb_srec.h"
#include <stdbool.h>
#include <stdio.h>
/******************************************************************************
 * PRIVATE TYPEDEF
 ******************************************************************************/
/**
 * @brief Reset_Handler typedef
*/
typedef void (*pResetHandler_t)(void);

/******************************************************************************
 * PRIVATE CONSTANTS
 ******************************************************************************/

/** @defgroup User app configurations
  * @{
  */
#define USER_APP_START_ADDRESS          (0x0000A000)
/**
 * @}
 */


/** @defgroup UART Pins, Ports and NVIC Configuration
 * @{
 */
#define UART_TX_PIN                     PORT_PIN_1
#define UART_RX_PIN                     PORT_PIN_0

#define UART_TX_RX_PIN_PORT             DRIVER_PORTB
#define UART_TX_RX_PIN_CLK              PCC_PERIPH_PORTB

#define UART_SREC_PRIORITY              NVIC_PRIORITY_0

#define UART_TIMEOUT                    (400U)
/**
 * @}
 */

/** @defgroup Button Pins, Ports, Clock source
 * @{
 */
#define BUTTON_0_PIN                GPIO_PIN_2
#define BUTTON_0_PORT               DRIVER_GPIOD
#define BUTTON_0_CLK                PCC_PERIPH_PORTD

#define BUTTON_PRESSED              GPIO_PIN_RESET
#define BUTTON_RELEASED             GPIO_PIN_SET
/**
 * @}
 */

/** @defgroup SREC Buffer
 * @{
 */
#define SREC_BUFFER_LINES               (4U)
#define SREC_LINE_MAX_BYTES             (1024U)
/**
 * @}
 */

/** @defgroup SREC parsing states
 * @{
 */
#define SREC_STOPPED                   (0U)
#define SREC_READY                     (1U)
#define SREC_RECEIVING_DATA            (2U)

#define SREC_GET_BYTECOUNT             (0U)
#define SREC_GET_DATA                  (1U)
/**
 * @}
 */

/** @defgroup Bootloader program states
 * @{
 */
#define BOOT_FLASHING                   (0U)
#define BOOT_IDLE                       (1U)
#define BOOT_MENU                       (2U)
/**
 * @}
 */
/******************************************************************************
 * PRIVATE MACROS
 ******************************************************************************/
/* Send a string by UART */
#define PRINT_MESSAGE(STR) \
                LPUART_Transmit_Polling(&handleUART, (uint8_t*)(STR), strlen(STR), UART_TIMEOUT)

/******************************************************************************
 * PRIVATE VARIABLES
 ******************************************************************************/
/* UART Handle for transmitting and receiving */
static LPUART_HandleTypeDef handleUART;
/* Bootloader state */
static uint8_t bootState = BOOT_MENU;
/* SREC's Queue struct */
static Queue_TypeDef srecQueue;
/* SREC buffer for lines */
static uint8_t srecBuffer[SREC_BUFFER_LINES * SREC_LINE_MAX_BYTES];
/* SREC Data field struct */
static SREC_Data_TypeDef srecData;
/* SREC Parsing states and error status */
static volatile uint8_t  parsingState = SREC_READY;
static uint8_t           parsingField = SREC_GET_BYTECOUNT;
static SREC_ErrorTypeDef srecError    = SREC_ERROR_NONE;
/* Misc */
static uint8_t errorMessage[50U];              /* Error message string */
static uint32_t timeoutTicks = 0x00;           /* To calculate the timeout of UART */
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 ******************************************************************************/
/* Initialize  */
static void BL_Button_Init(void);
static void BL_UART_Init(void);
/* Bootloader program */
static void BL_JumpToUserApp(void);
static void BL_Download_Program(void);
/* Interrupt Handler */
static void BL_SysTick_Handler(void);
/* Callbacks */
static void BL_UART_Received_Handling(LPUART_HandleTypeDef *LPUART_Handle);
/* Misc */
/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/**
 * @brief  Main function of program
 * @param  None
 * @return None
 */
int main(void)
{
    /* Initialize peripherals ( UART, GPIO(for button), LED, SysTick (for timeout) )*/
    BL_Button_Init();
    BL_UART_Init();
    SysTick_Init(SYSTICK_1000HZ_FREQ, NVIC_PRIORITY_1); /* UART is priority 0 */
    SysTick_RegisterCallback(BL_SysTick_Handler);
    /* Check if button is pressed*/
    if ((GPIO_ReadPin(BUTTON_0_PORT, BUTTON_0_PIN) == BUTTON_RELEASED))
    {
        /* Jump to user app */
        PRINT_MESSAGE("Left button is not pressed, executing user app\r\n");
        BL_JumpToUserApp();
    }
    while(true)
    {
        switch (bootState)
        {
            case BOOT_MENU:
                /* Welcom message, running bootloader program */
                PRINT_MESSAGE("Left button pressed, running bootloader\r\n"
                            "*****************************************************\r\n"
                            "*                  BOOTLOADER PROGRAM               *\r\n"
                            "*              FOR NXP FREEDOM KE16 BOARD           *\r\n"
                            "*****************************************************\r\n"
                            "V1.0.0                             Author: Leo Newens\r\n"
                            "-----------------------------------------------------\r\n"
                            "UART Configuration and Recommendations:              \r\n"
                            "    | Baud rate      : 115200                        \r\n"
                            "    | Parity         : No parity                     \r\n"
                            "    | Stop bits      : One stop bit                  \r\n"
                            "    | Word length    : 8 bits                        \r\n"
                            "    | Transmit delay : >= 1 msec/line (recommended)  \r\n");
                bootState = BOOT_FLASHING;
            case BOOT_FLASHING:
                BL_Download_Program();
                bootState = BOOT_IDLE;
                break;
            case BOOT_IDLE:
                break;
            default:
                break;
        }
    }
}

/**
 * @brief  Initialize button
 * @param  None
 * @retval None
 */
void BL_Button_Init(void)
{
    PORT_InitTypeDef PORT_Button_Init;
    /* Enable clock  */
    PCC_PeriphClockSrcSelect(BUTTON_0_CLK, PCC_CLOCKSOURCE_FIRC);
    SCG_SetCLOCKDIV2(SCG_CLOCKSOURCE_FIRC, SCG_DIVIDE_1);
    PCC_ClockGateControl(BUTTON_0_CLK, ENABLE);
    /* Configuration */
    PORT_Button_Init.Pin       = BUTTON_0_PIN;
    PORT_Button_Init.Mode      = PORT_MODE_INPUT;
    PORT_Button_Init.Filter    = PORT_FILTER_DISABLED;
    PORT_Button_Init.Drive     = PORT_DRIVE_LOW;
    PORT_Button_Init.Pull      = PORT_PUPD_NOPULL;
    PORT_Button_Init.Interrupt = PORT_INTRPT_DISABLED;
    PORT_Button_Init.Alternate = PORT_ALT_1;
    PORT_Init(BUTTON_0_PORT, &PORT_Button_Init);
}

/**
 * @brief  Initialize UART Peripheral
 * @param  None
 * @retval None
 */
void BL_UART_Init(void)
{
    PORT_InitTypeDef PORT_UART_Init;
    /* Enable clock for Tx Rx pins */
    PCC_ClockGateControl(UART_TX_RX_PIN_CLK, ENABLE);
    /* Configuration */
    PORT_UART_Init.Pin       = UART_TX_PIN | UART_RX_PIN;
    PORT_UART_Init.Mode      = PORT_MODE_AF;
    PORT_UART_Init.Filter    = PORT_FILTER_DISABLED;
    PORT_UART_Init.Pull      = PORT_PUPD_NOPULL;
    PORT_UART_Init.Interrupt = PORT_INTRPT_DISABLED;
    PORT_UART_Init.Alternate = PORT_ALT_2;
    PORT_Init(UART_TX_RX_PIN_PORT, &PORT_UART_Init);
    /* Enable clock for baud rate generator */
    PCC_PeriphClockSrcSelect(PCC_PERIPH_LPUART0, PCC_CLOCKSOURCE_FIRC);
    SCG_SetCLOCKDIV2(SCG_CLOCKSOURCE_FIRC, SCG_DIVIDE_1);
    PCC_ClockGateControl(PCC_PERIPH_LPUART0, ENABLE);
    /* Configuration */
    handleUART.DRIVER_LPUARTx    = DRIVER_LPUART0;
    handleUART.Init.BaudRate     = 115200;
    handleUART.Init.OverSampling = UART_OVERSAMPLING_AUTO;
    handleUART.Init.Mode         = UART_MODE_TX_RX;
    handleUART.Init.Parity       = UART_PARITY_NO;
    handleUART.Init.StopBits     = UART_STOPBITS_1;
    handleUART.Init.WordLength   = UART_WORDLENGTH_8B;
    LPUART_Init(&handleUART, UART_SREC_PRIORITY);
}

/**
 * @brief  Initialize all peripherals used for bootloader program
 * @param  None
 * @retval None
 */
void BL_DeInit(void)
{
    /* Deinit UART */
    PCC_ClockGateControl(PCC_PERIPH_LPUART0, DISABLE);
    PCC_PeriphClockSrcSelect(PCC_PERIPH_LPUART0, PCC_CLOCKSOURCE_Off);
    /* Deinit Button */
    PCC_ClockGateControl(BUTTON_0_CLK, ENABLE);
    PCC_PeriphClockSrcSelect(BUTTON_0_CLK, PCC_CLOCKSOURCE_Off);
    /* Deinit FIRC output */
    SCG_SetCLOCKDIV2(SCG_CLOCKSOURCE_FIRC, SCG_OUTPUT_DISABLED);
    /* Deinit SysTick */
    SysTick_DeInit();
}

/**
 * @brief  Move control to user app
 * @param  None
 * @retval None
 */
void BL_JumpToUserApp(void)
{
    /* MSP of user app */
    uint32_t appMSP;
    /* 1. Fetch the MSP */
    PRINT_MESSAGE("Initializing Main Stack Pointer...\r\n");
    appMSP = *((uint32_t*)(TO_POINTER(USER_APP_START_ADDRESS)));
    ARM_SetMSP(appMSP);
    /* 2. Fetch the address of user app reset handler */
    PRINT_MESSAGE("Fetch user app's Reset Handler...\r\n");

    /* Reset Handler address */
    uint32_t appResetHandler_Address = 0x00;

    /* Function pointer to user app reset handler */
    pResetHandler_t appResetHandler;

    appResetHandler_Address = *((uint32_t*)(TO_POINTER(USER_APP_START_ADDRESS + 4U)));

    appResetHandler = (pResetHandler_t)(TO_POINTER(appResetHandler_Address));

    /* 3. Jump to reset handler of the user application */
    PRINT_MESSAGE("Jump to user app\r\n");

    /* a. Deinit used peripherals */
    BL_DeInit();

    /* b. Change the vector table address */
    SCB_VectorTableOffSetConfig(USER_APP_START_ADDRESS);
    /* c. Jump */
    appResetHandler();
}

/**
 * @brief  Parsing SREC file and write to flash
 * @param  None
 * @retval None
 */
static void BL_Download_Program(void)
{
    uint16_t flashNextSector = 0x0000;
    srecError    = SREC_ERROR_NONE;
    parsingState = SREC_READY;
    PRINT_MESSAGE("=====================================================\r\n"
                  "Please send a SREC file to start                     \r\n"
                  "=====================================================\r\n");
    /* Initialize queue and SREC parameters */
    Queue_Init(&srecQueue, srecBuffer, SREC_LINE_MAX_BYTES, SREC_BUFFER_LINES);
    SREC_Init();
    /* Callback function after finish receiving process of UART */
    handleUART.RxFinishCallback = BL_UART_Received_Handling;
    /* Start the reception of SREC file (first 4 bytes, S-Record type and byte count)*/
    parsingField = SREC_GET_BYTECOUNT;
    LPUART_Start_Receive_Interrupt(&handleUART, Queue_GetBackBuffer(&srecQueue), 4U);
    /* Parse SREC file */
    while (parsingState)
    {
        /* Check if queue is empty */
        if (!Queue_IsEmpty(&srecQueue))
        {
            srecError = SREC_ParseLine(Queue_PeekFront(&srecQueue), &srecData);
            Queue_FrontPopIndex(&srecQueue, 1U);
            /* Errors occured */
            if (srecError != SREC_ERROR_NONE)
            {
                parsingState = SREC_STOPPED;
                LPUART_AbortReceive_Interrupt(&handleUART);
                sprintf((char*)errorMessage, "Error in parsing SREC file, at line %d\r\n", SREC_GetParsingLine());
                PRINT_MESSAGE(errorMessage);
            }
            else if ((srecData.DataBytes > 0) && (srecData.Address >= USER_APP_START_ADDRESS))
            {
                /* We would want to just erase the secotr where we write to it so that mulitple program
                 * can be written on FLASH */
                if (srecData.Address >= flashNextSector)
                {
                    FLASH_EraseSectors(srecData.Address, srecData.Address + srecData.DataBytes);
                    flashNextSector = (FLASH_GetSector(srecData.Address + srecData.DataBytes) + 1U) \
                                     * FLASH_SECTOR_SIZE;
                }
                FLASH_WriteData_Polling(srecData.Address, srecData.DataBytes, srecData.Data);
            }
            else
            {
                /* Avoid MISRA C violation */
            }
        }
    }
    /* Parsing successful*/
    if (!srecError)
    {
        PRINT_MESSAGE("Parsing success, program is load at 0x0000A000\r\n"
                      "Press reset button to execute user app        \r\n");
    }
}

/**
 * @brief  Callback function for after receiving required bytes
 * @param  LPUART_Handle
 * @retval None
 */
static void BL_UART_Received_Handling(LPUART_HandleTypeDef *LPUART_Handle)
{
    uint8_t        byteCount = 0x00;
    const uint8_t *tmpLine   = NULL;
    /* Reset timeout time (bytes were received)*/
    timeoutTicks = 0;
    parsingState = SREC_RECEIVING_DATA;
    /* First 4 bytes is to get the byte count, then start receive remaining bytes */
    if (parsingField == SREC_GET_BYTECOUNT)
    {
        /* Byte count starts at the third digit of a SREC line */
        tmpLine = Queue_PeekBack(&srecQueue);
        if (strtohex(&tmpLine[2], &byteCount))
        {
            /* Get remaining bytes and possible \r and \n */
            parsingField = SREC_GET_DATA;
            LPUART_Start_Receive_Interrupt(LPUART_Handle, LPUART_Handle->RxBuffer, byteCount * 2U + 2U);
        }
        else
        {
            parsingState = SREC_STOPPED;
            srecError = SREC_ERROR_INVALID_CHARACTER;
            LPUART_AbortReceive_Interrupt(LPUART_Handle);
            sprintf((char*)errorMessage, "Error in parsing SREC file, at line %d\r\n", SREC_GetParsingLine());
            PRINT_MESSAGE(errorMessage);
        }
    }
    else if (parsingField == SREC_GET_DATA)
    {
        /* Finish read a line */
        Queue_BackPushIndex(&srecQueue, 1U);
        /* Start to get next line */
        parsingField = SREC_GET_BYTECOUNT;
        if (Queue_IsFull(&srecQueue))
        {
            parsingState = SREC_STOPPED;
            LPUART_AbortReceive_Interrupt(LPUART_Handle);
            PRINT_MESSAGE("FIFO Buffer for parsing SREC lines is full,\r\n"
                        "try again or increase the transmit delay time\r\n");
        }
        else
        {
            LPUART_Start_Receive_Interrupt(LPUART_Handle, Queue_GetBackBuffer(&srecQueue), 4U);
        }
    }
}

/**
 * @brief  SysTick callback functions (used for calculate UART's timeout)
 * @param  None
 * @retval None
 */
static void BL_SysTick_Handler(void)
{
    timeoutTicks++;
    if ((parsingState == SREC_RECEIVING_DATA) && (timeoutTicks > UART_TIMEOUT))
    {
        /* Timeout, abort the UART reception and stop parsing */
        parsingState = SREC_STOPPED;
        LPUART_AbortReceive_Interrupt(&handleUART);
        /* Check if SREC file end with a valid terminate record */
        if ((srecError == SREC_ERROR_NONE) && (SREC_EndOfFileCheck() != SREC_ERROR_NONE))
        {
            srecError = SREC_ERROR_UNTERMINATED_FILE;
            PRINT_MESSAGE("Error in parsing SREC file, not terminated by S7/S8/S9\r\n");
        }
    }
}

/******************************************************************************
 * End of file
 ******************************************************************************/
