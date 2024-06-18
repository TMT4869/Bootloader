/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "emb_srec.h"
#include <stdbool.h>
/******************************************************************************
 * PRIVATE TYPEDEF
 ******************************************************************************/

/******************************************************************************
 * PRIVATE CONSTANTS
 ******************************************************************************/
/** @defgroup Record type
 * @{
 */
#define S0                                  ('0')
#define S1                                  ('1')
#define S2                                  ('2')
#define S3                                  ('3')
#define S5                                  ('5')
#define S6                                  ('6')
#define S7                                  ('7')
#define S8                                  ('8')
#define S9                                  ('9')
/**
 * @}
 */

/** @defgroup Number of digits for each record type and fields
 * @{
 */

#define S0_ADDRESSBYTES                   (2U)
#define S1_ADDRESSBYTES                   (2U)
#define S2_ADDRESSBYTES                   (3U)
#define S3_ADDRESSBYTES                   (4U)
#define S5_ADDRESSBYTES                   (2U)
#define S6_ADDRESSBYTES                   (3U)
#define S7_ADDRESSBYTES                   (4U)
#define S8_ADDRESSBYTES                   (3U)
#define S9_ADDRESSBYTES                   (2U)

#define BYTECOUNT_BYTES                   (1U)
#define ONE_BYTE                          (1U)

/**
 * @}
 */

/******************************************************************************
 * PRIVATE MACROS
 ******************************************************************************/
/* Check if a character is a hex digits */
#define IS_HEX(CH)                        ((((CH) <= 'F') && ((CH) >= 'A'))\
                                        || (((CH) <= '9') && ((CH) >= '0')))
/* Convert from a digit character to a digit value */
#define ASCII_TO_NUM(CH)                 ((CH) - '0')
/* Convert from a hex character to hex value */
#define ASCII_TO_HEX(CH)                 ((CH) - '0' - ((CH) >= 'A') * 7U)
/******************************************************************************
 * PRIVATE VARIABLES
 ******************************************************************************/
static bool     S0_Started        = false;
static uint8_t  S_DataTypeUsed    = false;
static uint8_t  S_CountUsed       = false;
static uint8_t  S_TerminateUsed   = false;
static uint32_t totalLines        = 0x00;
static uint32_t dataLines         = 0x00;
/* Address bytes */
static uint8_t addressField_HexDigits[10] =  /* Hex digits required for each record type */
           {S0_ADDRESSBYTES,\
            S1_ADDRESSBYTES,\
            S2_ADDRESSBYTES,\
            S3_ADDRESSBYTES,\
                          0,\
            S5_ADDRESSBYTES,\
            S6_ADDRESSBYTES,\
            S7_ADDRESSBYTES,\
            S8_ADDRESSBYTES,\
            S9_ADDRESSBYTES};
/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 ******************************************************************************/
static SREC_ErrorTypeDef SREC_ConvertToHex_BigEndian(const uint8_t *hexString,\
                                                     uint8_t *hexBuffer,\
                                                     const uint32_t bytes);

static SREC_ErrorTypeDef SREC_ConvertToHex_LittleEndian(const uint8_t *hexString,\
                                                        uint8_t *hexBuffer,\
                                                        const uint32_t bytes);
/******************************************************************************
 * End of file
 ******************************************************************************/

/**
 * @brief  Initialize all variables
 * @param  None
 * @retval None
 */
void SREC_Init(void)
{
    S0_Started        = false;
    S_DataTypeUsed    = false;
    S_CountUsed       = false;
    S_TerminateUsed   = false;
    totalLines        = 0x00;
    dataLines         = 0x00;
}

/**
 * @brief  Get the current or previous parsed line
 * @param  None
 * @retval Parsed line
 */
uint32_t SREC_GetParsingLine(void)
{
    return totalLines;
}

/**
 * @brief  Parse an SREC line
 * @param  lineBuffer: SREC line string
 * @param  pDataField: SREC struct to store the data parsed
 * @return Error status
 */
SREC_ErrorTypeDef SREC_ParseLine(const uint8_t *lineBuffer, SREC_Data_TypeDef *pDataField)
{
    SREC_ErrorTypeDef errorStatus     = SREC_ERROR_NONE;
    uint8_t           byteCount       = 0x00;            /* S-Record line's byte count */
    uint8_t           S_CurrentRecord = 0x00;            /* Current S Record */
    uint8_t           addressBytes    = 0x00;            /* Address field bytes */
    uint8_t           sumBytes        = 0x00;            /* Sum of all data bytes */
    uint8_t           i               = 0x00;            /* Loop variable */
    /* Reset data field */
    pDataField->Address   = 0x00;
    pDataField->DataBytes = 0x00;
    /* Check for S character */
    if (lineBuffer[0U] != 'S')
    {
        errorStatus = SREC_ERROR_INVALID_CHARACTER;
    }
    /* Record type */
    if (errorStatus == SREC_ERROR_NONE)
    {
        S_CurrentRecord = lineBuffer[1U];
        switch (S_CurrentRecord)
        {
            case '0':
                if (S0_Started)
                {
                    errorStatus = SREC_ERROR_DUPLICARE_HEADER;
                }
                S0_Started = true;
                break;
            case '1':
            case '2':
            case '3':
                if (!S0_Started)
                {
                    errorStatus = SREC_ERROR_NO_HEADER;
                }
                else if (S_CountUsed || S_TerminateUsed)
                {
                    errorStatus = SREC_ERROR_DATA_SENT;
                }
                else if (S_DataTypeUsed && (S_DataTypeUsed != S_CurrentRecord))
                {
                    errorStatus = SREC_ERROR_CONFLICTED_DATA_TYPE;
                }
                else
                {
                    S_DataTypeUsed = S_CurrentRecord;
                }
                /* Increase line parsed */
                dataLines++;
                break;
            case '5':
            case '6':
                if (!S_DataTypeUsed)
                {
                    errorStatus = SREC_ERROR_NO_DATA_SENT;
                }
                else if (S_CountUsed || S_TerminateUsed)
                {
                    errorStatus = SREC_ERROR_COUNTED_OR_TERMINATED;
                }
                else
                {
                    S_CountUsed  = S_CurrentRecord;
                }
                break;
            case '7':
            case '8':
            case '9':
                if (!S_DataTypeUsed)
                {
                    errorStatus = SREC_ERROR_NO_DATA_SENT;
                }
                else if (S_TerminateUsed)
                {
                    errorStatus = SREC_ERROR_COUNTED_OR_TERMINATED;
                }
                else if ((ASCII_TO_NUM(S_CurrentRecord) + ASCII_TO_NUM(S_DataTypeUsed)) != 10U)
                {
                    errorStatus = SREC_ERROR_UNMATCHED_TERMINATE_RECORD;
                }
                else
                {
                    S_TerminateUsed = S_CurrentRecord;
                }
                break;
            default:
                errorStatus = SREC_ERROR_INVALID_RECORD_TYPE;
                break;
        }
    }
    /* Get byte count */
    if (errorStatus == SREC_ERROR_NONE)
    {
        errorStatus = SREC_ConvertToHex_BigEndian(&lineBuffer[2U],\
                                                 &byteCount,\
                                                 BYTECOUNT_BYTES);
        sumBytes += byteCount;
        addressBytes = addressField_HexDigits[ASCII_TO_NUM(S_CurrentRecord)];
        if (((S_CurrentRecord >= S5) && (byteCount != (addressBytes + 1U))) \
        ||  ((S_CurrentRecord <  S5) && (byteCount <  (addressBytes + 1U))))
        {
            errorStatus = SREC_ERROR_INVALID_BYTE_COUNT;
        }
    }
    /* Get address or line counts */
    if (errorStatus == SREC_ERROR_NONE)
    {
        errorStatus = SREC_ConvertToHex_BigEndian(&lineBuffer[4U],\
                                                  (uint8_t*)&pDataField->Address,
                                                  addressBytes);
        for (i = 0; i < addressBytes; i++)
        {
            sumBytes += ((uint8_t*)&pDataField->Address)[i];
        }
        if ((S_CurrentRecord == S0) && (pDataField->Address != 0x0000))
        {
            errorStatus = SREC_ERROR_INVALID_HEADER_ADDRESS;
        }
        else if (((S_CurrentRecord == S5) || (S_CurrentRecord == S6)) \
             && (pDataField->Address != dataLines))
        {
            errorStatus = SREC_ERROR_WRONG_LINE_COUNT;
        }
        else
        {
            /* Avoid MISRA C violation */
        }
    }
    /* Get data bytes, and calculate checksum */
    pDataField->DataBytes = byteCount - addressBytes - 1U;
    for (i = 0; (i < (pDataField->DataBytes + 1U)) && (errorStatus == SREC_ERROR_NONE); i++)
    {
        errorStatus = SREC_ConvertToHex_LittleEndian(&lineBuffer[4U + (addressBytes + i) * 2U],\
                                                    &pDataField->Data[i],
                                                    ONE_BYTE);
        sumBytes += pDataField->Data[i];
    }
    /* Checksum */
    if ((errorStatus == SREC_ERROR_NONE) &&  (sumBytes != 0xFF))
    {
        errorStatus = SREC_ERROR_WRONG_CHECKSUM;
    }
    /* End of line */
    if (errorStatus == SREC_ERROR_NONE)
    {
        if (!(((lineBuffer[4U + byteCount * 2U] == '\r') && (lineBuffer[5U + byteCount * 2U] == '\n'))\
        ||  (lineBuffer[4U + byteCount * 2U] != '\n')))
        {
            errorStatus = SREC_ERROR_INVALID_CHARACTER;
        }
    }
    /* S0 doesn't have data field */
    if (S_CurrentRecord == S0)
    {
        pDataField->DataBytes = 0;
    }
    /* Increase line parsed */
    totalLines++;
    return errorStatus;
}

/**
 * @brief  Check if SREC file end with a terminate record
 * @param  None
 * @return Error status
 */
SREC_ErrorTypeDef SREC_EndOfFileCheck(void)
{
    SREC_ErrorTypeDef errorStatus = SREC_ERROR_NONE;
    if (!S_TerminateUsed)
    {
        errorStatus = SREC_ERROR_UNTERMINATED_FILE;
    }
    return errorStatus;
}

/**
 * @brief  Convert from a hex string to bytes (big endian)
 * @param  hexString: Hex string
 * @param  hexBuffer: Buffer to store the bytes converted
 * @param  bytes: Number of bytes to convert
 * @return Error status
 */
static SREC_ErrorTypeDef SREC_ConvertToHex_BigEndian(const uint8_t *hexString,\
                                                     uint8_t *hexBuffer,\
                                                     const uint32_t bytes)
{
    SREC_ErrorTypeDef errorStatus = SREC_ERROR_NONE;
    uint32_t i = 0x00;
    for (i = 0; (i < bytes) && (errorStatus == SREC_ERROR_NONE); i++)
    {
        if(IS_HEX(hexString[i * 2U]) && IS_HEX(hexString[i * 2U + 1U]))
        {
            hexBuffer[bytes - 1U - i] = (ASCII_TO_HEX(hexString[i * 2U]) << 4U) \
                                       + ASCII_TO_HEX(hexString[i * 2U + 1U]);
        }
        else
        {
            errorStatus = SREC_ERROR_INVALID_CHARACTER;
        }
    }
    return errorStatus;
}

/**
 * @brief  Convert from a hex string to bytes (little endian)
 * @param  hexString: Hex string
 * @param  hexBuffer: Buffer to store the bytes converted
 * @param  bytes: Number of bytes to convert
 * @return Error status
 */
static SREC_ErrorTypeDef SREC_ConvertToHex_LittleEndian(const uint8_t *hexString,\
                                                        uint8_t *hexBuffer,\
                                                        const uint32_t bytes)
{
    SREC_ErrorTypeDef errorStatus = SREC_ERROR_NONE;
    uint32_t i = 0x00;
    for (i = 0; (i < bytes) && (errorStatus == SREC_ERROR_NONE); i++)
    {
        if(IS_HEX(hexString[i * 2U]) && IS_HEX(hexString[i * 2U + 1U]))
        {
            hexBuffer[i] = (ASCII_TO_HEX(hexString[i * 2U]) << 4U) \
                          + ASCII_TO_HEX(hexString[i * 2U + 1U]);
        }
        else
        {
            errorStatus = SREC_ERROR_INVALID_CHARACTER;
        }
    }
    return errorStatus;
}

/******************************************************************************
 * End of file
 ******************************************************************************/
