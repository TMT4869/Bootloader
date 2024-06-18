/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_hal_lpuart.h"
#include "mke16z4_lpuart.h"
#include "mke16z4_clock.h"
#include "mke16z4_core.h"
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
/* LPUARTx arrays */
static LPUART_Type *const lpuartPtrs[LPUART_TOTAL_INSTANCES] = LPUART_BASE_PTRS;
/* LPUART IRQ numbers */
static const IRQn_Type LPUART_IRQn[LPUART_TOTAL_INSTANCES] = {LPUART0_IRQn, LPUART1_IRQn, LPUART2_IRQn};
/* Data mask for each word length type ( 6bit to 10bit )*/
static const uint16_t wordLengthMask[5U] = {0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF};
/* LPUART Handles (one for each instance) */
static LPUART_HandleTypeDef *LPUART_DriverHandles[LPUART_TOTAL_INSTANCES];
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 ******************************************************************************/
static int16_t LPUSART_InstanceToPCC(LPUART_Type *LPUARTx);
static void LPUART_BaudRateMatchConfig(LPUART_Type *LPUARTx, uint32_t BaudRate);
static DRV_StatusTypeDef LPUART_WaitForFlag(LPUART_Type *LPUARTx,\
                                         uint32_t UART_FLAG,\
                                         FlagStatus flagStatus,\
                                         const uint32_t timeLimit);
/* Process Transmitting and Receiving */
static DRV_StatusTypeDef LPUART_Receive_Interrupt(LPUART_HandleTypeDef *LPUART_Handle);
static DRV_StatusTypeDef LPUART_Transmit_Interrupt(LPUART_HandleTypeDef *LPUART_Handle);
static DRV_StatusTypeDef LPUART_TransmitComplete_Interrupt(LPUART_HandleTypeDef *LPUART_Handle);
/* Default IRQ handler for LPUART and function pointers */
static void LPUART_Default_IRQHandler(void);
/* Function pointers */
static void (*pISR_LPUART0_Callback)(void) = LPUART_Default_IRQHandler;
static void (*pISR_LPUART1_Callback)(void) = LPUART_Default_IRQHandler;
static void (*pISR_LPUART2_Callback)(void) = LPUART_Default_IRQHandler;
/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/**
 * @brief Initialize LPUART Peripheral
 * @param LPUART_Handle:LPUART Handle
 * @param Priority: interrupt priority
 * @retval None
 */
void LPUART_Init(LPUART_HandleTypeDef *LPUART_Handle, uint32_t Priority)
{
    uint32_t uartClock   = 0x00;
    uint32_t tmpOvrsampl = 0x00;
    uint32_t tmpDivisor  = 0x00;
    LPUART_Type* LPUARTx = NULL;
    /* Busy state (configuring), TxState is also used for global state of UART */
    LPUART_Handle->TxState = UART_STATE_BUSY;
    /* Check parameters */
    assert(IS_UART_ALL_INSTANCE(LPUART_Handle->DRIVER_LPUARTx));
    assert(IS_UART_MODE(LPUART_Handle->Init.Mode));
    assert(IS_UART_PARITY(LPUART_Handle->Init.Parity));
    assert(IS_UART_STOPBITS(LPUART_Handle->Init.StopBits));
    assert(IS_UART_OVERSAMPLING(LPUART_Handle->Init.OverSampling));
    assert(IS_USART_BAUDRATE(LPUART_Handle->Init.BaudRate));
    assert(IS_UART_WORD_LENGTH(LPUART_Handle->Init.WordLength));
    assert(IS_NVIC_PRIORITY(Priority));
    /* Get instance */
    LPUARTx = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    /* Disable transmitter and receiver */
    __HAL_LPUART_DISABLE(LPUARTx);
    /* Configure oversampling, baud clock divisor, stop bits and M10 */
        /* 1. M10 (10 data bits) */
    __HAL_LPUART_10BIT_DATA_DISABLE(LPUARTx);
    if (LPUART_Handle->Init.WordLength == UART_WORDLENGTH_10B)
    {
        __HAL_LPUART_10BIT_DATA_ENABLE(LPUARTx);
    }
        /* 2. Stop bits (SBNS) */
    __HAL_LPUART_STOPBITS_CONFIG(LPUARTx, LPUART_Handle->Init.StopBits);
    if (LPUART_Handle->Init.OverSampling == UART_OVERSAMPLING_AUTO)
    {
        LPUART_BaudRateMatchConfig(LPUARTx, LPUART_Handle->Init.BaudRate);
    }
    else
    {
        /* 3. Oversampling ratio (OSR) */
        __HAL_LPUART_OVERSAMPLING_CONFIG(LPUARTx, LPUART_Handle->Init.OverSampling);
        __HAL_LPUART_SAMPLING_BOTHEDGE_DISABLE(LPUARTx);
        /* 4. Baud clock divisor (SBR) */
            /* a. Get asyncrhonous clock of baud rate clock */
        uartClock = PCC_GetAsyncClock(LPUSART_InstanceToPCC(LPUARTx));
            /* b. Oversampling ratio */
        tmpOvrsampl = LPUART_Handle->Init.OverSampling + 1U;
        if ((tmpOvrsampl > UART_OVERSAMPLING_4) && (tmpOvrsampl < UART_OVERSAMPLING_9))
        {
            __HAL_LPUART_SAMPLING_BOTHEDGE_ENABLE(LPUARTx);
        }
            /* c. Modulus divier */
        tmpDivisor = (uint32_t)uartClock /(LPUART_Handle->Init.BaudRate * tmpOvrsampl);
            /* d. Choose the closer value */
        if (tmpDivisor == 0)
        {
            tmpDivisor = 1U;
        }
        else if ((uartClock - tmpDivisor * tmpOvrsampl * LPUART_Handle->Init.BaudRate) \
          > ((tmpDivisor + 1U) * tmpOvrsampl * LPUART_Handle->Init.BaudRate - uartClock))
        {
            tmpDivisor = tmpDivisor + 1U;
        }
        else
        {
            /* Avoid MISRA C violation */
        }
            /* d .Config SBR */
        __HAL_LPUART_BAUDRATE_DIVISOR_CONFIG(LPUARTx, tmpDivisor);
    }
    /* Configure parity, word length, loop mode */
        /* 1. Word length */
    if (LPUART_Handle->Init.WordLength == UART_WORDLENGTH_9B)
    {
        __HAL_LPUART_7BIT_DATA_DISABLE(LPUARTx);
        __HAL_LPUART_9BIT_DATA_ENABLE(LPUARTx);
    }
    else if (LPUART_Handle->Init.WordLength == UART_WORDLENGTH_7B)
    {
        __HAL_LPUART_9BIT_DATA_DISABLE(LPUARTx);
        __HAL_LPUART_7BIT_DATA_ENABLE(LPUARTx);
    }
    else /* Default 8bit data or 10bit data */
    {
        __HAL_LPUART_9BIT_DATA_DISABLE(LPUARTx);
        __HAL_LPUART_7BIT_DATA_DISABLE(LPUARTx);
    }
        /* 2. Parity */
    __HAL_LPUART_PARITY_CONFIG(LPUARTx, LPUART_Handle->Init.Parity);
        /* 3. Loop mode config */
    if ((LPUART_Handle->Init.Mode == UART_MODE_LOOPBACK) || \
        (LPUART_Handle->Init.Mode == UART_MODE_HALFDUPLEX))
    {
        __HAL_LPUART_LOOPMODE_CONFIG(LPUARTx,LPUART_Handle->Init.Mode);
    }
    /* Enable Tx or Rx or both */
    if (LPUART_Handle->Init.Mode & UART_MODE_RX)
    {
        __HAL_LPUART_RX_ENABLE(LPUARTx);
    }
    if (LPUART_Handle->Init.Mode & UART_MODE_TX)
    {
        __HAL_LPUART_TX_ENABLE(LPUARTx);
    }
    /* Initialize function pointers */
    LPUART_Handle->ErrorCallback = NULL;
    LPUART_Handle->RxFinishCallback = NULL;
    LPUART_Handle->TxFinishCallback = NULL;
    /* Make state to ready (can receive or send data) */
    LPUART_Handle->TxState = UART_STATE_READY;
    LPUART_Handle->RxState = UART_STATE_READY;
    /* NVIC config */
    NVIC_SetPriority(LPUART_IRQn[LPUART_Handle->DRIVER_LPUARTx], Priority);
    NVIC_DisableIRQ(LPUART_IRQn[LPUART_Handle->DRIVER_LPUARTx]);
    NVIC_EnableIRQ(LPUART_IRQn[LPUART_Handle->DRIVER_LPUARTx]);
    NVIC_ClearPendingIRQ(LPUART_IRQn[LPUART_Handle->DRIVER_LPUARTx]);
    /* Assign user handle to driver handle */
    LPUART_DriverHandles[LPUART_Handle->DRIVER_LPUARTx] = LPUART_Handle;
}

/**
 * @brief  Start transmit with LPUART by interrupts
 * @param  LPUART_Handle: Handle
 * @param  pData:pointer to data
 * @param  DataSize: data size
 * @retval None
 */
DRV_StatusTypeDef LPUART_Start_Transmit_Interrupt (LPUART_HandleTypeDef *LPUART_Handle,\
                                      const uint8_t *pData,\
                                      const uint32_t DataSize)
{
    LPUART_Type *LPUARTx = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    DRV_StatusTypeDef status = DRV_ERROR_NONE;
    if ((pData == NULL) || (DataSize == 0))
    {
        status = DRV_ERROR;
    }
    else if (LPUART_Handle->TxState == UART_STATE_READY)
    {
        /* Set state to busy */
        LPUART_Handle->TxState  = UART_STATE_BUSY_TX;
        /* Configure the transmition data */
        LPUART_Handle->TxBuffer = pData;
        LPUART_Handle->TxCount  = DataSize;
        /* Enable interrupt */
        __HAL_LPUART_INTRPT_ENABLE(LPUARTx, UART_INTRPT_TXEMPTY);
        /* Configuration finished */
        status = DRV_ERROR_NONE;
    }
    else
    {
        status = DRV_BUSY;
    }
    return status;
}

/**
 * @brief  Start receiving with LPUART
 * @param  LPUART_Handle: Handle
 * @param  pData:pointer to data
 * @param  DataSize: data size
 * @retval None
 */
DRV_StatusTypeDef LPUART_Start_Receive_Interrupt(LPUART_HandleTypeDef *LPUART_Handle,\
                                          uint8_t *pData,\
                                          const uint32_t DataSize)
{
    LPUART_Type *LPUARTx = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    DRV_StatusTypeDef status = DRV_ERROR_NONE;
    if ((pData == NULL) || (DataSize == 0))
    {
        status = DRV_ERROR;
    }
    else if (LPUART_Handle->RxState == UART_STATE_READY)
    {
        /* Set state to busy */
        LPUART_Handle->RxState = UART_STATE_BUSY_RX;
        /* Configure the transmition data */
        LPUART_Handle->RxBuffer = pData;
        LPUART_Handle->RxCount = DataSize;
        /* Enable error interrupts */
        __HAL_LPUART_INTRPT_ENABLE(LPUARTx, UART_INTRPT_FRAMEERR | \
                                        UART_INTRPT_OVERRUN | \
                                        UART_INTRPT_NOISEERR | \
                                        UART_INTRPT_PARITYERR);
        /* Enable interrupt to start receving data */
        __HAL_LPUART_INTRPT_ENABLE(LPUARTx, UART_INTRPT_RXFULL);
        /* Configuration finished */
        status = DRV_ERROR_NONE;
    }
    else
    {
        status = DRV_BUSY;
    }
    return status;
}

/**
 * @brief  Abort or stop waiting to receive data from UART
 * @param  LPUART_Handle: Handle
 * @retval None
 */
void LPUART_AbortReceive_Interrupt(LPUART_HandleTypeDef *LPUART_Handle)
{
    LPUART_Type *LPUARTx = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    /* Disable error interrupts and RDRF interrupt */
        /* Errors */
    __HAL_LPUART_INTRPT_DISABLE(LPUARTx, UART_INTRPT_FRAMEERR | UART_INTRPT_OVERRUN | UART_INTRPT_NOISEERR | UART_INTRPT_PARITYERR);
        /* Receive Buffer Full */
    __HAL_LPUART_INTRPT_DISABLE(LPUARTx, UART_INTRPT_RXFULL);
    /* Reset counter */
    LPUART_Handle->RxCount = 0x00;
    /* Set state to ready */
    LPUART_Handle->RxState = UART_STATE_READY;
}

/**
 * @brief  Start transmitting data from LPUART by polling
 */
DRV_StatusTypeDef LPUART_Transmit_Polling(LPUART_HandleTypeDef *LPUART_Handle,\
                                          const uint8_t *pData, \
                                          const uint32_t DataSize, \
                                          const uint32_t TimeLimit)
{
    LPUART_Type      *LPUARTx  = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    DRV_StatusTypeDef status   = DRV_ERROR_NONE;
    uint16_t          tempData = 0x00;
    uint16_t dataMask = wordLengthMask[LPUART_Handle->Init.WordLength - (LPUART_Handle->Init.Parity != UART_PARITY_NO)];
    if ((pData == NULL) || (DataSize == 0))
    {
        status = DRV_ERROR;
    }
    else if (LPUART_Handle->TxState == UART_STATE_READY)
    {
        /* Set state to busy */
        LPUART_Handle->TxState = UART_STATE_BUSY_TX;
        /* Configure the transmition data */
        LPUART_Handle->TxBuffer = pData;
        LPUART_Handle->TxCount = DataSize;
        while ((LPUART_Handle->TxCount-- != 0) && (status == DRV_ERROR_NONE))
        {
            if (LPUART_WaitForFlag(LPUARTx, UART_FLAG_TXBUFFEREMPTY, SET, TimeLimit) == DRV_TIMEOUT)
            {
                status = DRV_TIMEOUT;
            }
            else
            {
                if ((LPUART_Handle->Init.WordLength == UART_WORDLENGTH_10B)
                || (LPUART_Handle->Init.WordLength == UART_WORDLENGTH_9B) && (LPUART_Handle->Init.Parity == UART_PARITY_NO))
                {
                    tempData = ((uint16_t)(*LPUART_Handle->TxBuffer)) | (uint16_t) (*(LPUART_Handle->TxBuffer + 1U) << 8U);
                    __HAL_LPUART_WRITE_DATA_BUFFER(LPUARTx, tempData & dataMask);
                    LPUART_Handle->TxBuffer += 2U;
                }
                else
                {
                    __HAL_LPUART_WRITE_DATA_BUFFER(LPUARTx, (*LPUART_Handle->TxBuffer) & (uint8_t)dataMask);
                    LPUART_Handle->TxBuffer += 1U;
                }
            }
        }
        /* Wait for Tranmission Complete Flag (TCIE) */
        if (LPUART_WaitForFlag(LPUARTx, UART_FLAG_TXCOMPLETE, SET, TimeLimit) == DRV_TIMEOUT)
        {
            status = DRV_TIMEOUT;
        }
        /* Release Tx */
        LPUART_Handle->TxState = UART_STATE_READY;
    }
    else
    {
        status = DRV_BUSY;
    }
    return status;
}

/**
 * @brief Start receiving data from LPUART by polling
 */
DRV_StatusTypeDef LPUART_Receive_Polling(LPUART_HandleTypeDef *LPUART_Handle, \
                                         uint8_t *pData, \
                                         const uint32_t DataSize, \
                                         const uint32_t TimeLimit)
{
    LPUART_Type      *LPUARTx      = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    DRV_StatusTypeDef status       = DRV_ERROR_NONE;
    uint8_t          *pTempData    = NULL;
    uint16_t          receivedData = 0x00;
    uint16_t dataMask = wordLengthMask[LPUART_Handle->Init.WordLength - (LPUART_Handle->Init.Parity != UART_PARITY_NO)];
    if ((pData == NULL) || (DataSize == 0))
    {
        status = DRV_ERROR;
    }
    else if (LPUART_Handle->RxState == UART_STATE_READY)
    {
        /* Set state to busy */
        LPUART_Handle->RxState = UART_STATE_BUSY_RX;
        /* Configure the transmition data */
        LPUART_Handle->RxBuffer = pData;
        LPUART_Handle->RxCount  = DataSize;
        while ((LPUART_Handle->RxCount-- != 0) && (status == DRV_ERROR_NONE))
        {
            if (LPUART_WaitForFlag(LPUARTx, UART_FLAG_RXBUFFERFULL, SET, TimeLimit) == DRV_TIMEOUT)
            {
                status = DRV_TIMEOUT;
            }
            else
            {
                /*  Check data size and read data into buffer */
                if ((LPUART_Handle->Init.WordLength == UART_WORDLENGTH_10B)
                || (LPUART_Handle->Init.WordLength == UART_WORDLENGTH_9B) && (LPUART_Handle->Init.Parity == UART_PARITY_NO))
                {
                    receivedData = (uint16_t)(__HAL_LPUART_READ_DATA_BUFFER(LPUARTx) & dataMask);
                    /* Save first byte of data */
                    *pTempData = (uint8_t)receivedData;
                    /* Save remaning bits */
                    *(pTempData + 1U) = (uint8_t)(receivedData >> 8U);
                    LPUART_Handle->RxBuffer += 2U;
                }
                else
                {
                    *pTempData = (uint8_t)(__HAL_LPUART_READ_DATA_BUFFER(LPUARTx) & dataMask);
                    LPUART_Handle->RxBuffer += 1U;
                }
            }
        }
        LPUART_Handle->RxState = UART_STATE_READY;
    }
    else
    {
        status = DRV_BUSY;
    }
    return status;
}

static DRV_StatusTypeDef LPUART_WaitForFlag(LPUART_Type *LPUARTx,\
                                         uint32_t UART_FLAG,\
                                         FlagStatus flagStatus,\
                                         const uint32_t timeLimit)
{
    DRV_StatusTypeDef drvStatus = DRV_ERROR_NONE;
    uint32_t startTicks;
    /* SysTick is not yet initialized */
    if (SysTick_GetFrequency() == 0)
    {
        SysTick_Init(SYSTICK_1000HZ_FREQ, NVIC_PRIORITY_0);
    }

    startTicks = SysTick_GetTick();
    while (((FlagStatus)(__HAL_LPUART_GET_FLAG(LPUARTx, UART_FLAG)) != flagStatus) \
        && (drvStatus == DRV_ERROR_NONE))
    {
        if (SysTick_GetTick() > (startTicks + timeLimit))
        {
            drvStatus = DRV_TIMEOUT;
        }
    }
    return drvStatus;
}

/**
 * @brief  Enable or disable the interrupt for a UART Flag
 * @param  LPUART_Handle: LPUART Handle
 * @param  UART_INTRPT: UART Interrupts
 * @param  NewState: ENABLE or DISABLE
 * @retval None
 */
void LPUART_InterruptEnablingConfig(LPUART_HandleTypeDef *LPUART_Handle,\
                                    uint16_t UART_INTRPT,\
                                    FunctionalState NewState)
{
    LPUART_Type *LPUARTx = NULL;
    /* Check parameters */
    assert(IS_UART_ALL_INSTANCE(LPUART_Handle->DRIVER_LPUARTx));
    assert(IS_UART_INTRPT(UART_INTRPT));
    assert(IS_FUNCTIONAL_STATE(NewState));
    /* Enable or disable interrupt for flags */
    LPUARTx = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    __HAL_LPUART_INTRPT_DISABLE(LPUARTx, UART_INTRPT);
    if (NewState != DISABLE)
    {
        __HAL_LPUART_INTRPT_ENABLE(LPUARTx, UART_INTRPT);
    }
}

/**
 * @brief  Process in ISR for handle transmition and reception of UART with interrupt
 * @param  LPUART_Handle: LPUART Handle
 * @retval None
 */
void LPUART_IRQHandler(LPUART_HandleTypeDef *LPUART_Handle)
{
    LPUART_Type *LPUARTx = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    uint32_t flagStatus  = LPUARTx->STAT;
    uint32_t ITenabled   = LPUARTx->CTRL;
    /* Check for errors */
    if ((flagStatus & (UART_FLAG_PARITYERR | UART_FLAG_FRAMEERR | UART_FLAG_NOISEERR | UART_FLAG_OVERRUN)) == RESET)
    {
        /* Receiver buffer full */
        if ((flagStatus & UART_FLAG_RXBUFFERFULL) && (ITenabled & UART_INTRPT_RXFULL))
        {
            LPUART_Receive_Interrupt(LPUART_Handle);
        }
        /* Transmitter buffer empty */
        if ((flagStatus & UART_FLAG_TXBUFFEREMPTY) && (ITenabled & UART_INTRPT_TXEMPTY))
        {
            LPUART_Transmit_Interrupt(LPUART_Handle);
        }
        /* Transmit completion */
        if ((flagStatus & UART_FLAG_TXCOMPLETE) && (ITenabled & UART_INTRPT_TXCOMPLETE))
        {
            LPUART_TransmitComplete_Interrupt(LPUART_Handle);
        }
    }
    else /* If some errors occur */
    {
        /* Frame error */
        if ((flagStatus & UART_FLAG_FRAMEERR) && (ITenabled & UART_INTRPT_FRAMEERR))
        {
            __HAL_LPUART_CLEAR_FLAG(LPUARTx, UART_FLAG_FRAMEERR);
            LPUART_Handle->ErrorName |= UART_ERROR_FRAME;
        }
        /* Noise detected */
        if ((flagStatus & UART_FLAG_NOISEERR) && (ITenabled & UART_INTRPT_NOISEERR))
        {
            __HAL_LPUART_CLEAR_FLAG(LPUARTx, UART_FLAG_NOISEERR);
            LPUART_Handle->ErrorName |= UART_ERROR_NOISE;
        }
        /* Parity error */
        if ((flagStatus & UART_FLAG_PARITYERR) && (ITenabled & UART_INTRPT_PARITYERR))
        {
            __HAL_LPUART_CLEAR_FLAG(LPUARTx, UART_FLAG_PARITYERR);
            LPUART_Handle->ErrorName |= UART_ERROR_PARITY;
        }
        /* Overrun error */
        if ((flagStatus & UART_FLAG_OVERRUN) && (ITenabled & UART_INTRPT_OVERRUN))
        {
            __HAL_LPUART_CLEAR_FLAG(LPUARTx, UART_FLAG_OVERRUN);
            LPUART_Handle->ErrorName |= UART_ERROR_OVERRUN;
        }
        /* Error call back */
        if (LPUART_Handle->ErrorCallback != NULL)
        {
            LPUART_Handle->ErrorCallback(LPUART_Handle);
        }
        LPUART_Handle = UART_ERROR_NONE;
    }
}

/**
 * @brief  Get the flag status of UART
 * @param  LPUART_Handle: Handle of LPUART
 * @param  UART_FLAG: UART Flag
 * @retval None
 */
FlagStatus LPUART_GetFlagStatus(LPUART_HandleTypeDef *LPUART_Handle, uint32_t UART_FLAG)
{
    FlagStatus status = RESET;
    LPUART_Type *LPUARTx = NULL;
    /* Check parameters */
    assert(IS_UART_ALL_INSTANCE(LPUART_Handle->DRIVER_LPUARTx));
    assert(IS_UART_FLAG(UART_FLAG));
    /* Read flag status */
    LPUARTx = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    if (__HAL_LPUART_GET_FLAG(LPUARTx, UART_FLAG) != RESET)
    {
        status = SET;
    }
    return status;
}

/**
 * @brief  Clear the flag of UART
 * @param  LPUART_Handle: Handle of LPUART
 * @param  UART_FLAG: UART Flag
 * @retval None
 */
void LPUART_ClearFlag(LPUART_HandleTypeDef *LPUART_Handle, uint32_t UART_FLAG)
{
    LPUART_Type *LPUARTx = NULL;
    /* Check parameters */
    assert(IS_UART_ALL_INSTANCE(LPUART_Handle->DRIVER_LPUARTx));
    assert(IS_UART_FLAG(UART_FLAG));
    LPUARTx = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    __HAL_LPUART_CLEAR_FLAG(LPUARTx, UART_FLAG);
    if ((UART_FLAG & UART_FLAG_RXBUFFERFULL) != RESET)
    {
        UNUSED(__HAL_LPUART_READ_DATA_BUFFER(LPUARTx));
    }
}

/**
 * @brief  Convert LPUART Instance to a PCC Peripheral Define to config clock
 * @param  LPUARTx: LPAURT Instance
 * @retval PCC Peripheral
 */
static int16_t LPUSART_InstanceToPCC(LPUART_Type *LPUARTx)
{
    int16_t PCCx = 0x00;
    if (LPUARTx == LPUART0)
    {
        PCCx = PCC_PERIPH_LPUART0;
    }
    else if (LPUARTx == LPUART1)
    {
        PCCx = PCC_PERIPH_LPUART1;
    }
    else if (LPUARTx == LPUART2)
    {
        PCCx = PCC_PERIPH_LPUART2;
    }
    else
    {
        /* Avoid MISRA C violation */
    }
    return PCCx;
}

/**
 * @brief  Config the LPUART Instance to the closest match of baud rate
 * @param  LPUARTx: LPUART Instance
 * @param  BaudRate
 * @retval None
 */
static void LPUART_BaudRateMatchConfig(LPUART_Type *LPUARTx, uint32_t BaudRate)
{
    uint32_t uartClock    = 0x00;
    uint32_t tmpOvrsampl  = 0x00;
    uint32_t tmpDivisor   = 0x00;
    uint32_t tmpModulo    = 0x00;
    uint32_t bestModulo   = 0xFFFFFFFF;
    uint32_t bestDivisor  = 0x00;
    uint32_t bestOvrsampl = 0x00;
    uint8_t i = 0;
    /* Get baud rate clock of LPUART */
    uartClock = PCC_GetAsyncClock(LPUSART_InstanceToPCC(LPUARTx));
    /* Find the best match config to baud rate */
    for (i = UART_OVERSAMPLING_4; i <= UART_OVERSAMPLING_32; i++)
    {
        tmpOvrsampl = i + 1U;
        tmpModulo   = uartClock % (tmpOvrsampl * BaudRate);
        tmpDivisor  = (uartClock - tmpModulo) / (tmpOvrsampl * BaudRate);
        /* (tmpOvrsampl * BaudRate) is bigger than uartClock */
        if (tmpModulo == uartClock)
        {
            tmpDivisor = 1U;
        }
        if (tmpModulo < bestModulo)
        {
            bestOvrsampl = i;
            bestModulo   = tmpModulo;
            bestDivisor  = tmpDivisor;
        }
    }
    /* Config */
    __HAL_LPUART_OVERSAMPLING_CONFIG(LPUARTx, bestOvrsampl);
    if ((bestOvrsampl >= UART_OVERSAMPLING_4) && (bestOvrsampl <= UART_OVERSAMPLING_7))
    {
        __HAL_LPUART_SAMPLING_BOTHEDGE_ENABLE(LPUARTx);
    }
    __HAL_LPUART_BAUDRATE_DIVISOR_CONFIG(LPUARTx, bestDivisor);
}

/**
 * @brief  Process the reception of UART
 * @param  LPUART_Handle: Handler of LPUART
 * @retval Error status
 */
static DRV_StatusTypeDef LPUART_Receive_Interrupt(LPUART_HandleTypeDef *LPUART_Handle)
{
    DRV_StatusTypeDef status       = DRV_ERROR_NONE;
    uint8_t          *pTempData    = NULL;
    uint16_t          receivedData = 0x00;
    LPUART_Type      *LPUARTx      = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    uint16_t dataMask = wordLengthMask[LPUART_Handle->Init.WordLength - (LPUART_Handle->Init.Parity != UART_PARITY_NO)];
    pTempData = LPUART_Handle->RxBuffer;
    if (LPUART_Handle->RxState == UART_STATE_BUSY_RX)
    {
        /*  Check data size and read data into buffer */
        if ((LPUART_Handle->Init.WordLength == UART_WORDLENGTH_10B)
         || (LPUART_Handle->Init.WordLength == UART_WORDLENGTH_9B) && (LPUART_Handle->Init.Parity == UART_PARITY_NO))
        {
            receivedData = (uint16_t)(__HAL_LPUART_READ_DATA_BUFFER(LPUARTx) & dataMask);
            /* Save first byte of data */
            *pTempData = (uint8_t)receivedData;
            /* Save remaning bits */
            *(pTempData + 1U) = (uint8_t)(receivedData >> 8U);
            LPUART_Handle->RxBuffer += 2U;
        }
        else
        {
            *pTempData = (uint8_t)(__HAL_LPUART_READ_DATA_BUFFER(LPUARTx) & dataMask);
            LPUART_Handle->RxBuffer += 1U;
        }
        /* Finish read a data */
        LPUART_Handle->RxCount--;
        /* Check if received enough required data and end the receiving process */
        if (LPUART_Handle->RxCount == 0)
        {
            /* Disable interrupts (receive buffer full and errors)*/
            __HAL_LPUART_INTRPT_DISABLE(LPUARTx, UART_INTRPT_RXFULL);
            __HAL_LPUART_INTRPT_DISABLE(LPUARTx, UART_INTRPT_OVERRUN | UART_INTRPT_PARITYERR | UART_INTRPT_FRAMEERR | UART_INTRPT_NOISEERR);
            /* Release the receiver Rx */
            LPUART_Handle->RxState = UART_STATE_READY;
            /* Callback if function pointer is assigned */
            if (LPUART_Handle->RxFinishCallback != NULL)
            {
                LPUART_Handle->RxFinishCallback(LPUART_Handle);
            }
        }
    }
    else /* Not request receiving but still call this function */
    {
        status = DRV_ERROR;
    }
    return status;
}

/**
 * @brief  Process the transmittion of UART
 * @param  LPUART_Handle: Handler of LPUART
 * @retval Error status
 */
static DRV_StatusTypeDef LPUART_Transmit_Interrupt(LPUART_HandleTypeDef *LPUART_Handle)
{
    DRV_StatusTypeDef status   = DRV_ERROR_NONE;
    uint16_t          tempData = 0x00;
    LPUART_Type      *LPUARTx  = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    /* Data mask is the word length (minus 1 if use parity) */
    uint16_t dataMask = wordLengthMask[LPUART_Handle->Init.WordLength - (LPUART_Handle->Init.Parity != UART_PARITY_NO)];
    if (LPUART_Handle->TxState == UART_STATE_BUSY_TX)
    {
        /*  Check data size and read data into buffer */
        /* 10 Bit data and 9 Bit data (no parity) need more than 8 Bit*/
        if ((LPUART_Handle->Init.WordLength == UART_WORDLENGTH_10B)
         || (LPUART_Handle->Init.WordLength == UART_WORDLENGTH_9B) && (LPUART_Handle->Init.Parity == UART_PARITY_NO))
        {
            tempData = ((uint16_t)(*LPUART_Handle->TxBuffer)) | (uint16_t) (*(LPUART_Handle->TxBuffer + 1U) << 8U);
            __HAL_LPUART_WRITE_DATA_BUFFER(LPUARTx, tempData & dataMask);
            LPUART_Handle->TxBuffer += 2U;
        }
        else
        {
            __HAL_LPUART_WRITE_DATA_BUFFER(LPUARTx, (*LPUART_Handle->TxBuffer) & (uint8_t)dataMask);
            LPUART_Handle->TxBuffer += 1U;
        }
        /* Finish reading a data */
        LPUART_Handle->TxCount--;
        /* Check if received enough required data and end the receiving process */
        if (LPUART_Handle->TxCount == 0)
        {
            /* Disable interrupt of transmit buffer empty */
            __HAL_LPUART_INTRPT_DISABLE(LPUARTx, UART_INTRPT_TXEMPTY);
            /* Enable Tranmission Complete interrupt */
            __HAL_LPUART_INTRPT_ENABLE(LPUARTx, UART_INTRPT_TXCOMPLETE);
        }
    }
    else /* Not request receiving but still call this function */
    {
        status = DRV_ERROR;
    }
    return status;
}

/**
 * @brief  Tranmit Completion, release the transmitter and call the callback function
 * @param  LPUART_Handle: Handler of LPUART
 * @retval Status
 */
static DRV_StatusTypeDef LPUART_TransmitComplete_Interrupt(LPUART_HandleTypeDef *LPUART_Handle)
{
    LPUART_Type *LPUARTx   = lpuartPtrs[LPUART_Handle->DRIVER_LPUARTx];
    /* Disable Tranmission Complete interrupt */
    __HAL_LPUART_INTRPT_DISABLE(LPUARTx, UART_INTRPT_TXCOMPLETE);
    /* Release Tx */
    LPUART_Handle->TxState = UART_STATE_READY;
    /* Callback after finishing transmition */
    if (LPUART_Handle->TxFinishCallback != NULL)
    {
        LPUART_Handle->TxFinishCallback( LPUART_Handle);
    }
    return DRV_ERROR_NONE;
}

/**
 * @brief  Register callback function for LPUART0 interrupt handlers
 * @param  callbackFunc: Function pointer of the callback function
 * @retval None
 */
void LPUART0_RegisterCallback(void (*callbackFunc)(void))
{
    pISR_LPUART0_Callback = callbackFunc;
}

/**
 * @brief  Register callback function for LPUART1 interrupt handlers
 * @param  callbackFunc: Function pointer of the callback function
 * @retval None
 */
void LPUART1_RegisterCallback(void (*callbackFunc)(void))
{
    pISR_LPUART1_Callback = callbackFunc;
}

/**
 * @brief  Register callback function for LPUART2 interrupt handlers
 * @param  callbackFunc: Function pointer of the callback function
 * @retval None
 */
void LPUART2_RegisterCallback(void (*callbackFunc)(void))
{
    pISR_LPUART2_Callback = callbackFunc;
}

/**
 * @brief  Empty function as default handler
 * @param  None
 * @retval None
 */
static void LPUART_Default_IRQHandler(void)
{
    /* Empty function body */
}

/**
 * @brief  LPUART0 Handler from Vector table
 * @param  None
 * @retval None
 */
void LPUART0_IRQHandler(void)
{
    LPUART_IRQHandler(LPUART_DriverHandles[0U]);
    pISR_LPUART0_Callback();
}

/**
 * @brief  LPUART1 Handler from Vector table
 * @param  None
 * @retval None
 */
void LPUART1_IRQHandler(void)
{
    LPUART_IRQHandler(LPUART_DriverHandles[1U]);
    pISR_LPUART1_Callback();
}

/**
 * @brief LPUART2 Handler from Vector table
 * @param  None
 * @retval None
 */
void LPUART2_IRQHandler(void)
{
    LPUART_IRQHandler(LPUART_DriverHandles[2U]);
    pISR_LPUART2_Callback();
}

/******************************************************************************
 * END OF FILE
 ******************************************************************************/
