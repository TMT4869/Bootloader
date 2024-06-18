#ifndef EMB_SREC_H_
#define EMB_SREC_H_

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <stdint.h>
/******************************************************************************
 * EXPORTED TYPEDEF
 ******************************************************************************/

/**
 * @brief Type of possible errors while parsing SREC
 */
typedef enum
{
    SREC_ERROR_NONE,
    SREC_ERROR_INVALID_CHARACTER,                /* NOT A HEX, 'S', \R, \N OR AT THE WRONG POSITIONS*/
    SREC_ERROR_NO_HEADER,                        /* S1 -> S9 FOUND WITHOUT S0 */
    SREC_ERROR_DUPLICARE_HEADER,                 /* S0 FOUND AGAIN */
    SREC_ERROR_DATA_SENT,                        /* S1 -> S3 FOUND WHEN S5 -> S9 FOUND */
    SREC_ERROR_NO_DATA_SENT,                     /* S5 -> S9 FOUND WHEN S1 -> S3 HAVEN'T */
    SREC_ERROR_CONFLICTED_DATA_TYPE,             /* DIFFRENT TYPES OF RECORD (S1,S2,S3)*/
    SREC_ERROR_WRONG_LINE_COUNT,                 /* S5 OR S6 COUNTED WRONG */
    SREC_ERROR_COUNTED_OR_TERMINATED,            /* FOUND S5 -> S9 AGAIN  */
    SREC_ERROR_UNMATCHED_TERMINATE_RECORD,       /* S1 MUST GO WITH S9 AND SO ON */
    SREC_ERROR_INVALID_RECORD_TYPE,              /* NOT A VALID RECORD TYPE S0 -> S9 (NOT S4 )*/
    SREC_ERROR_INVALID_BYTE_COUNT,               /* S0 -> S3 MUST REACHED A MIN, S5 -> S9 CAN ONLY HAVE ONE VALUE */
    SREC_ERROR_INVALID_HEADER_ADDRESS,           /* S0 MUST HAVE 0000 AS ADDRESS */
    SREC_ERROR_WRONG_CHECKSUM,                   /* WRONG CHECK SUM */
    SREC_ERROR_UNTERMINATED_FILE                 /* S7 -> S9 must be the last record type with finished checksum */
} SREC_ErrorTypeDef;

/**
 * @brief SREC Struct to hold the data parsed
 */
typedef struct __SREC_Fields
{
    uint32_t Address;
    uint8_t  DataBytes;
    uint8_t  Data[255U];
}SREC_Data_TypeDef;

/******************************************************************************
 * EXPORTED CONSTANTS
 ******************************************************************************/

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
 * @brief  Initialize all variables
 */
void SREC_Init(void);

/**
 * @brief  Get the current or previous parsed line
 */
uint32_t SREC_GetParsingLine(void);

/**
 * @brief  Parse an SREC line
 */
SREC_ErrorTypeDef SREC_ParseLine(const uint8_t *lineBuffer, SREC_Data_TypeDef *pDataField);

/**
 * @brief  Check if SREC file end with a terminate record
 */
SREC_ErrorTypeDef SREC_EndOfFileCheck(void);

#endif /* EMB_SREC_H_ */
/******************************************************************************
 * End of file
 ******************************************************************************/