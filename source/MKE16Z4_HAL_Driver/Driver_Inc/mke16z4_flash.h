#ifndef __MKE16Z4_FLASH
#define __MKE16Z4_FLASH

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_def.h"
/******************************************************************************
 * EXPORTED TYPEDEF
 ******************************************************************************/

/******************************************************************************
 * EXPORTED CONSTANTS
 ******************************************************************************/

/** @defgroup Flash memory information
 * @{
 */
#define FLASH_SECTOR_SIZE               (0x400U)
#define FLASH_START_ADDR                (0x00000000U)
#define FLASH_END_ADDR                  (0x0000FFFFU)

#define FLASH_CONFIG_ADDRESS            (0x400U)
/**
 * @}
 */

/** @defgroup Flash Flags
 * @{
 */
#define FLASH_FLAG_COMMAND_COMPLETE     (0x80U)
#define FLASH_FLAG_ACCESS_ERROR         (0x20U)
#define FLASH_FLAG_PROTECT_VIOLATE      (0x10U)
#define FLASH_FLAG_READ_COLLISION       (0x40U)
/**
 * @}
 */
/******************************************************************************
 * EXPORTED MACROS
 ******************************************************************************/
#define IS_LONGWORD_ALIGNED(ADDR)               ((ADDR & 0x03) == 0x00)

#define IS_MULTIPLE_OF_LONGWORD(BYTES)          (BYTES == ((BYTES >> 2U) << 2U))

#define IS_VALID_FLASH_ADDRESS(ADDR)            (ADDR <= FLASH_END_ADDR)
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
 * @brief  Write a block of data to FLASH
*/
void FLASH_WriteData_Polling(const uint32_t StartAddress,\
                             const uint32_t WriteSize,\
                             const uint8_t *pData);

/**
 * @brief  Erase the sectors from a starting to an ending address
*/
void FLASH_EraseSectors(const uint32_t StartAddress,\
                        const uint32_t EndAddress);

/**
 * @brief  Erase a sector from an address
*/
void FLASH_EraseSector(const uint32_t Address);

/**
 * @brief  Write a Write a longword (4bytes) to FLASH
*/
void FLASH_WriteLongWord_Polling(const uint32_t Address,\
                                 const uint8_t *pData);

/**
 * @brief  Get the sector number of given address
 */
uint8_t FLASH_GetSector(const uint32_t Address);

#endif /* __MKE16Z4_FLASH */

/******************************************************************************
 * End of file
 ******************************************************************************/
