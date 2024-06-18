#ifndef __MKE16Z4_HAL_FLASH
#define __MKE16Z4_HAL_FLASH

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
#define __HAL_FLASH_WRITE_COMMAND(__COMMAND__) \
                    (FTFA->FCCOB0 = (FTFA->FCCOB0 & (~0xFF)) | ((__COMMAND__) & 0xFF))

#define __HAL_FLASH_GET_FLAG(__FLAG__) \
                    ((FTFA->FSTAT & (__FLAG__)) != 0x00)

#define __HAL_FLASH_CLEAR_FLAG(__FLAG__) \
                    (FTFA->FSTAT = __FLAG__)

#define __HAL_FLASH_COMMAND_LAUNCH() \
                    (__HAL_FLASH_CLEAR_FLAG(0x80))

#define __HAL_FLASH_WRITE_ADDRESS_BIT0_BIT7(__ADDR__) \
                    (FTFA->FCCOB3 = (FTFA->FCCOB0 & (~0xFF)) | ((__ADDR__) & 0xFF))

#define __HAL_FLASH_WRITE_ADDRESS_BIT8_BIT15(__ADDR__) \
                    (FTFA->FCCOB2 = (FTFA->FCCOB0 & (~0xFF)) | ((__ADDR__) & 0xFF))

#define __HAL_FLASH_WRITE_ADDRESS_BIT16_BIT23(__ADDR__) \
                    (FTFA->FCCOB1 = (FTFA->FCCOB0 & (~0xFF)) | ((__ADDR__) & 0xFF))

#define __HAL_FLASH_WRITE_BYTE_0(__BYTE__) \
                    (FTFA->FCCOB4 = (FTFA->FCCOB0 & (~0xFF)) | ((__BYTE__) & 0xFF))

#define __HAL_FLASH_WRITE_BYTE_1(__BYTE__) \
                    (FTFA->FCCOB5 = (FTFA->FCCOB0 & (~0xFF)) | ((__BYTE__) & 0xFF))

#define __HAL_FLASH_WRITE_BYTE_2(__BYTE__) \
                    (FTFA->FCCOB6 = (FTFA->FCCOB0 & (~0xFF)) | ((__BYTE__) & 0xFF))

#define __HAL_FLASH_WRITE_BYTE_3(__BYTE__) \
                    (FTFA->FCCOB7 = (FTFA->FCCOB0 & (~0xFF)) | ((__BYTE__) & 0xFF))

#define __HAL_FLASH_WRITE_BYTE_4(__BYTE__) \
                    (FTFA->FCCOB8 = (FTFA->FCCOB0 & (~0xFF)) | ((__BYTE__) & 0xFF))

#define __HAL_FLASH_WRITE_BYTE_5(__BYTE__) \
                    (FTFA->FCCOB9 = (FTFA->FCCOB0 & (~0xFF)) | ((__BYTE__) & 0xFF))

#define __HAL_FLASH_WRITE_BYTE_6(__BYTE__) \
                    (FTFA->FCCOBA = (FTFA->FCCOB0 & (~0xFF)) | ((__BYTE__) & 0xFF))

#define __HAL_FLASH_WRITE_BYTE_7(__BYTE__) \
                    (FTFA->FCCOBB = (FTFA->FCCOB0 & (~0xFF)) | ((__BYTE__) & 0xFF))

/******************************************************************************
 * EXPORTED VARIABLES
 ******************************************************************************/

/******************************************************************************
 * EXPORTED FUNCTIONS PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************/

#endif /* __MKE16Z4_HAL_FLASH */
/******************************************************************************
 * End of file
 ******************************************************************************/
