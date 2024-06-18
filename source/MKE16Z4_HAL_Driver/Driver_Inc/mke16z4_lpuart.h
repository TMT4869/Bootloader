#ifndef __MKE16Z4_LPUART
#define __MKE16Z4_LPUART

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_def.h"
/******************************************************************************
 * EXPORTED TYPEDEF
 ******************************************************************************/

/**
  * @brief UART Init Structure definition
  */
typedef struct
{
    uint32_t OverSampling;              /*!< Specifies whether the Oversampling ratio for the receiver.
                                            This parameter can be a value of @ref UART_Over_Sampling */

    uint32_t BaudRate;                  /*!< This member configures the UART communication baud rate.
                                            The baud rate is computed using the following formula:
                                            - ModuloDivisor = ((AsyncClock) / ((OSR+1) * BaudRate))
                                            Where OSR is the Oversampling ratio configuration bit in the BAUD register. */

    uint32_t WordLength;                /*!< Specifies the number of data bits transmitted or received in a frame.
                                            This parameter can be a value of @ref UART_Word_Length */

    uint32_t StopBits;                  /*!< Specifies the number of stop bits transmitted.
                                            This parameter can be a value of @ref UART_Stop_Bits */

    uint32_t Parity;                    /*!< Specifies the parity mode.
                                            This parameter can be a value of @ref UART_PARITY
                                            @note When parity is enabled, the computed parity is inserted
                                                    at the MSB position of the transmitted data */

    uint32_t Mode;                      /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                                            This parameter can be a value of @ref UART_Mode */

}LPUART_InitTypeDef;

/**
  * @brief UART State structures definition
  * @note  UART State value is a combination of 2 different substates: State and RxState.
  */
typedef enum
{
    UART_STATE_RESET             = 0x00U,    /*!< Peripheral is not yet Initialized */
    UART_STATE_READY             = 0x40U,    /*!< Peripheral Initialized and ready for use */
    UART_STATE_BUSY              = 0x41U,    /*!< an internal process is ongoing */
    UART_STATE_BUSY_TX           = 0x42U,    /*!< Data Transmission process is ongoing*/
    UART_STATE_BUSY_RX           = 0x48U,    /*!< Data Reception process is ongoing */
    UART_STATE_ERROR             = 0xF0U     /*!< Error */
} LPUART_StateTypeDef;


/**
  * @brief  UART handle Structure definition
  */
typedef struct __LPUART_HandleTypeDef
{
    uint32_t                     DRIVER_LPUARTx; /*!< UART registers base address        */

    LPUART_InitTypeDef           Init;           /*!< UART communication parameters      */

    const uint8_t               *TxBuffer;       /*!< Pointer to UART Tx transfer Buffer */

    volatile uint32_t            TxCount;        /*!< UART Tx Transfer Counter           */

    uint8_t                     *RxBuffer;       /*!< Pointer to UART Rx transfer Buffer */

    volatile uint32_t            RxCount;        /*!< UART Rx Transfer Counter           */

    volatile LPUART_StateTypeDef TxState;          /*!< UART state information and also related to Tx operations.
                                                    This parameter can be a value of @ref UART_StateTypeDef */

    volatile LPUART_StateTypeDef RxState;        /*!< UART state information related to Rx operations.
                                                    This parameter can be a value of @ref UART_StateTypeDef */

    volatile uint32_t            ErrorName;      /*!< UART Error code                    */

    void (* ErrorCallback)(struct __LPUART_HandleTypeDef *LPUART_Handle);
    void (* RxFinishCallback)(struct __LPUART_HandleTypeDef *LPUART_Handle);
    void (* TxFinishCallback)(struct __LPUART_HandleTypeDef *LPUART_Handle);
}LPUART_HandleTypeDef;
/******************************************************************************
 * EXPORTED CONSTANTS
 ******************************************************************************/

/** @defgroup UART_Error_Code UART Error Code
  * @{
  */
#define UART_ERROR_NONE                  0x00000000U   /*!< No error            */
#define UART_ERROR_PARITY                0x00000001U   /*!< Parity error        */
#define UART_ERROR_NOISE                 0x00000002U   /*!< Noise error         */
#define UART_ERROR_FRAME                 0x00000004U   /*!< Frame error         */
#define UART_ERROR_OVERRUN               0x00000008U   /*!< Overrun error       */
/**
  * @}
  */

/** @defgroup UART_Instance UART Instance
  * @{
  */
#define DRIVER_LPUART0                  0x00000000U
#define DRIVER_LPUART1                  0x00000001U
#define DRIVER_LPUART2                  0x00000002U

#define LPUART_TOTAL_INSTANCES          (0x03U)

#define IS_UART_ALL_INSTANCE(PERIPH)   (((PERIPH) == DRIVER_LPUART0) || \
                                        ((PERIPH) == DRIVER_LPUART1) || \
                                        ((PERIPH) == DRIVER_LPUART2))
/**
  * @}
  */

/** @defgroup UART_Word_Length UART Word Length
  * @{
  */
#define UART_WORDLENGTH_7B                  (0x01U)
#define UART_WORDLENGTH_8B                  (0x02U)
#define UART_WORDLENGTH_9B                  (0x03U)
#define UART_WORDLENGTH_10B                 (0x04U)

#define IS_UART_WORD_LENGTH(LENGTH)    (((LENGTH) == UART_WORDLENGTH_7B) || \
                                        ((LENGTH) == UART_WORDLENGTH_8B) || \
                                        ((LENGTH) == UART_WORDLENGTH_9B) || \
                                        ((LENGTH) == UART_WORDLENGTH_10B))
/**
  * @}
  */

/** @defgroup UART_Stop_Bits UART Number of Stop Bits
  * @{
  */
#define UART_STOPBITS_1                     (0x00000000U)
#define UART_STOPBITS_2                     (0x00000001U)

#define IS_UART_STOPBITS(STOPBITS)  (((STOPBITS) == UART_STOPBITS_1) || \
                                     ((STOPBITS) == UART_STOPBITS_2))
/**
  * @}
  */

/** @defgroup UART_PARITY UART Parity
  * @{
  */
#define UART_PARITY_NO                      (0x00000000U)
#define UART_PARITY_EVEN                    (0x00000002U)
#define UART_PARITY_ODD                     (0x00000003U)

#define IS_UART_PARITY(PARITY)     (((PARITY) == UART_PARITY_NO) || \
                                    ((PARITY) == UART_PARITY_EVEN) || \
                                    ((PARITY) == UART_PARITY_ODD))
/**
  * @}
  */
/** @defgroup UART_Mode UART Transfer Mode
  * @{
  */
#define UART_MODE_RX                        (0x40000U)
#define UART_MODE_TX                        (0x80000U)
#define UART_MODE_TX_RX                     (UART_MODE_RX | UART_MODE_TX)
#define UART_MODE_LOOPBACK                  (0x80U)
#define UART_MODE_HALFDUPLEX                (0xA0U)

#define IS_UART_MODE(MODE)    (((MODE) == UART_MODE_RX) || \
                                ((MODE) == UART_MODE_TX) || \
                                ((MODE) == UART_MODE_TX_RX) || \
                                ((MODE) == UART_MODE_LOOPBACK) || \
                                ((MODE) == UART_MODE_HALFDUPLEX))
/**
  * @}
  */

/** @defgroup UART_Over_Sampling UART Over Sampling
  * @{
  */
#define UART_OVERSAMPLING_AUTO                  (0xFFFFFFFFU)
#define UART_OVERSAMPLING_DEFAULT               (0x00000000U)
#define UART_OVERSAMPLING_4                     (0x00000003U)
#define UART_OVERSAMPLING_5                     (0x00000004U)
#define UART_OVERSAMPLING_6                     (0x00000005U)
#define UART_OVERSAMPLING_7                     (0x00000006U)
#define UART_OVERSAMPLING_8                     (0x00000007U)
#define UART_OVERSAMPLING_9                     (0x00000008U)
#define UART_OVERSAMPLING_10                    (0x00000009U)
#define UART_OVERSAMPLING_11                    (0x0000000AU)
#define UART_OVERSAMPLING_12                    (0x0000000BU)
#define UART_OVERSAMPLING_13                    (0x0000000CU)
#define UART_OVERSAMPLING_14                    (0x0000000DU)
#define UART_OVERSAMPLING_15                    (0x0000000EU)
#define UART_OVERSAMPLING_16                    (0x0000000FU)
#define UART_OVERSAMPLING_17                    (0x00000010U)
#define UART_OVERSAMPLING_18                    (0x00000011U)
#define UART_OVERSAMPLING_19                    (0x00000012U)
#define UART_OVERSAMPLING_20                    (0x00000013U)
#define UART_OVERSAMPLING_21                    (0x00000014U)
#define UART_OVERSAMPLING_22                    (0x00000015U)
#define UART_OVERSAMPLING_23                    (0x00000016U)
#define UART_OVERSAMPLING_24                    (0x00000017U)
#define UART_OVERSAMPLING_25                    (0x00000018U)
#define UART_OVERSAMPLING_26                    (0x00000019U)
#define UART_OVERSAMPLING_27                    (0x0000001AU)
#define UART_OVERSAMPLING_28                    (0x0000001BU)
#define UART_OVERSAMPLING_29                    (0x0000001CU)
#define UART_OVERSAMPLING_30                    (0x0000001DU)
#define UART_OVERSAMPLING_31                    (0x0000001EU)
#define UART_OVERSAMPLING_32                    (0x0000001FU)

#define IS_UART_OVERSAMPLING(OS)       (((OS) == UART_OVERSAMPLING_DEFAULT) || \
                                        ((OS) == UART_OVERSAMPLING_AUTO) || \
                                        ((OS) == UART_OVERSAMPLING_4) || \
                                        ((OS) == UART_OVERSAMPLING_5) || \
                                        ((OS) == UART_OVERSAMPLING_6) || \
                                        ((OS) == UART_OVERSAMPLING_7) || \
                                        ((OS) == UART_OVERSAMPLING_8) || \
                                        ((OS) == UART_OVERSAMPLING_9) || \
                                        ((OS) == UART_OVERSAMPLING_10) || \
                                        ((OS) == UART_OVERSAMPLING_11) || \
                                        ((OS) == UART_OVERSAMPLING_12) || \
                                        ((OS) == UART_OVERSAMPLING_13) || \
                                        ((OS) == UART_OVERSAMPLING_14) || \
                                        ((OS) == UART_OVERSAMPLING_15) || \
                                        ((OS) == UART_OVERSAMPLING_16) || \
                                        ((OS) == UART_OVERSAMPLING_17) || \
                                        ((OS) == UART_OVERSAMPLING_18) || \
                                        ((OS) == UART_OVERSAMPLING_19) || \
                                        ((OS) == UART_OVERSAMPLING_20) || \
                                        ((OS) == UART_OVERSAMPLING_21) || \
                                        ((OS) == UART_OVERSAMPLING_22) || \
                                        ((OS) == UART_OVERSAMPLING_23) || \
                                        ((OS) == UART_OVERSAMPLING_24) || \
                                        ((OS) == UART_OVERSAMPLING_25) || \
                                        ((OS) == UART_OVERSAMPLING_26) || \
                                        ((OS) == UART_OVERSAMPLING_27) || \
                                        ((OS) == UART_OVERSAMPLING_28) || \
                                        ((OS) == UART_OVERSAMPLING_29) || \
                                        ((OS) == UART_OVERSAMPLING_30) || \
                                        ((OS) == UART_OVERSAMPLING_31) || \
                                        ((OS) == UART_OVERSAMPLING_32))

#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 12000000U))
/**
  * @}
  */

/** @defgroup UART_Flags   UART FLags
  *        Elements values convention: 0xXXXXXXXX
  *           - 0xXXXXXXXX  : Flag mask in the STAT register
  * @{
  */
#define UART_FLAG_MATCHADDR2            (0x00004000U)
#define UART_FLAG_MATCHADDR1            (0x00008000U)
#define UART_FLAG_PARITYERR             (0x00010000U)
#define UART_FLAG_FRAMEERR              (0x00020000U)
#define UART_FLAG_NOISEERR              (0x00040000U)
#define UART_FLAG_OVERRUN               (0x00080000U)
#define UART_FLAG_IDLE                  (0x00100000U)
#define UART_FLAG_RXBUFFERFULL          (0x00200000U)
#define UART_FLAG_TXBUFFEREMPTY         (0x00400000U)
#define UART_FLAG_TXCOMPLETE            (0x00800000U)
#define UART_FLAG_RAF                   (0x01000000U)
#define UART_FLAG_RXEDGIF               (0x40000000U)
#define UART_FLAG_LBKDIF                (0x80000000U)

#define IS_UART_FLAG(FLAG)    (((FLAG) & UART_FLAG_MATCHADDR1) || ((FLAG) & UART_FLAG_MATCHADDR2) || \
                                ((FLAG) & UART_FLAG_PARITYERR) || ((FLAG) & UART_FLAG_FRAMEERR) || \
                                ((FLAG) & UART_FLAG_NOISEERR) || ((FLAG) & UART_FLAG_OVERRUN) || \
                                ((FLAG) & UART_FLAG_IDLE) || ((FLAG) & UART_FLAG_RXBUFFERFULL) || \
                                ((FLAG) & UART_FLAG_TXBUFFEREMPTY) || ((FLAG) & UART_FLAG_TXCOMPLETE) || \
                                ((FLAG) & UART_FLAG_RAF) || ((FLAG) & UART_FLAG_LBKDIF) || \
                                ((FLAG) & UART_FLAG_RXEDGIF))
/**
  * @}
  */

/** @defgroup UART_Interrupt_definition  UART Interrupt Definitions
  * @{
  */
#define UART_INTRPT_MATCHADD2                 (0x00004000U)
#define UART_INTRPT_MATCHADD1                 (0x00008000U)
#define UART_INTRPT_PARITYERR                 (0x01000000U)
#define UART_INTRPT_FRAMEERR                  (0x02000000U)
#define UART_INTRPT_NOISEERR                  (0x04000000U)
#define UART_INTRPT_OVERRUN                   (0x08000000U)
#define UART_INTRPT_IDLE                      (0x00100000U)
#define UART_INTRPT_RXFULL                    (0x00200000U)
#define UART_INTRPT_TXCOMPLETE                (0x00400000U)
#define UART_INTRPT_TXEMPTY                   (0x00800000U)

#define IS_UART_INTRPT(INTRPT)      (((INTRPT) == UART_INTRPT_MATCHADD1) || ((INTRPT) == UART_INTRPT_MATCHADD2) || \
                                ((INTRPT) == UART_INTRPT_PARITYERR) || ((INTRPT) == UART_INTRPT_FRAMEERR) || \
                                ((INTRPT) == UART_INTRPT_NOISEERR) || ((INTRPT) == UART_INTRPT_OVERRUN) || \
                                ((INTRPT) == UART_INTRPT_IDLE) || ((INTRPT) == UART_INTRPT_RXFULL) || \
                                ((INTRPT) == UART_INTRPT_TXEMPTY) || ((INTRPT) == UART_INTRPT_TXCOMPLETE))
/**
  * @}
  */

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

/**
 * @brief  Initialize a LPUART Handle
 */
void LPUART_Init(LPUART_HandleTypeDef *LPUART_Handle, uint32_t Priority);

/* Transmitting and receiving data functions **********************************/

/**
 * @brief  Start transmitting data from LPUART by interrupt
 */
DRV_StatusTypeDef LPUART_Start_Transmit_Interrupt(LPUART_HandleTypeDef *LPUART_Handle,\
                                          const uint8_t *pData,\
                                          const uint32_t DataSize);

/**
 * @brief Start receiving data from LPUART by interrupt
 */
DRV_StatusTypeDef LPUART_Start_Receive_Interrupt(LPUART_HandleTypeDef *LPUART_Handle,\
                                          uint8_t *pData,\
                                          const uint32_t DataSize);

/**
 * @brief  End the receiving process with interrupt
 */
void LPUART_AbortReceive_Interrupt(LPUART_HandleTypeDef *LPUART_Handle);

/**
 * @brief  Start transmitting data from LPUART by polling
 */
DRV_StatusTypeDef LPUART_Transmit_Polling(LPUART_HandleTypeDef *LPUART_Handle,\
                                          const uint8_t *pData,\
                                          const uint32_t DataSize,\
                                          const uint32_t TimeLimit);

/**
 * @brief Start receiving data from LPUART by polling
 */
DRV_StatusTypeDef LPUART_Receive_Polling(LPUART_HandleTypeDef *LPUART_Handle,\
                                         uint8_t *pData,\
                                         const uint32_t DataSize,\
                                         const uint32_t TimeLimit);

/* Interrupts and flags management functions **********************************/

/**
 * @brief  Enable or disable interrupt of LPUART
 */
void LPUART_InterruptEnablingConfig(LPUART_HandleTypeDef *LPUART_Handle,\
                                    uint16_t UART_INTRPT,\
                                    FunctionalState NewState);

/**
 * @brief  Get flag (SET or RESET) of LPUART
 */
FlagStatus LPUART_GetFlagStatus(LPUART_HandleTypeDef *LPUART_Handle, uint32_t UART_FLAG);

/**
 * @brief  Clear flags for LPUART
 */
void LPUART_ClearFlag(LPUART_HandleTypeDef *LPUART_Handle, uint32_t UART_FLAG);

/* Callback functions for ISR **************************************************/
/**
 * @brief  Register callback function for interrupt
 */
void LPUART0_RegisterCallback(void (*callbackFunc)(void));

/**
 * @brief  Register callback function for interrupt
 */
void LPUART1_RegisterCallback(void (*callbackFunc)(void));

/**
 * @brief  Register callback function for interrupt
 */
void LPUART2_RegisterCallback(void (*callbackFunc)(void));

#endif /* __MKE16Z4_LPUART */

/******************************************************************************
 * End of file
 ******************************************************************************/
