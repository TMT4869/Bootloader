/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include "mke16z4_flash.h"
#include "mke16z4_hal_flash.h"
#include <assert.h>
/******************************************************************************
 * PRIVATE TYPEDEF
 ******************************************************************************/

/******************************************************************************
 * PRIVATE CONSTANTS
 ******************************************************************************/
#define FLASH_COMMAND_ERASE                     ((uint8_t)0x09)
#define FLASH_COMMAND_PROGRAM_LONGWORD          ((uint8_t)0x06)
/******************************************************************************
 * PRIVATE MACROS
 ******************************************************************************/

/******************************************************************************
 * PRIVATE VARIABLES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

/**
 * @brief  Write a block of data to FLASH
 * @param  StartAddress: starting address of write the first byte
 * @param  WriteSize: size in bytes
 * @param  pData: pointer to data
 * @retval None
 */
void FLASH_WriteData_Polling(const uint32_t StartAddress,\
                             const uint32_t WriteSize,\
                             const uint8_t *pData)
{
    assert(IS_LONGWORD_ALIGNED(StartAddress));
    assert(pData != NULL);
    uint32_t words = (WriteSize >> 2U) + ((WriteSize & 0x03) != 0x00);
    uint32_t i;
    for (i = 0; i < words; i++)
    {
        FLASH_WriteLongWord_Polling(StartAddress + 4U * i, pData + 4U * i);
    }
}

/**
 * @brief  Erase the sectors from a starting to an ending address
 * @param  StartAddress
 * @param  EndAddress
 * @return None
 */
void FLASH_EraseSectors(const uint32_t StartAddress,\
                        const uint32_t EndAddress)
{
    assert(IS_LONGWORD_ALIGNED(StartAddress));
    assert(IS_LONGWORD_ALIGNED(EndAddress));
    uint8_t startSector = FLASH_GetSector(StartAddress);
    uint8_t endSector   = FLASH_GetSector(EndAddress);
    uint8_t i;
    for (i = 0; i < (startSector - endSector + 1U); i++)
    {
        FLASH_EraseSector(FLASH_SECTOR_SIZE * (startSector + i));
    }
}

/**
 * @brief  Get the sector of an address
 * @param  Address
 * @retval None
 */
uint8_t FLASH_GetSector(const uint32_t Address)
{
    assert(IS_VALID_FLASH_ADDRESS(Address));
    uint8_t sector = 0x00;
    sector = (uint8_t)(Address / FLASH_SECTOR_SIZE);
    return sector;
}

/**
 * @brief  Erase a sector from an address
 * @param  Address
 * @param  pData: pointer to data
 * @retval None
 */
void FLASH_EraseSector(const uint32_t Address)
{
    assert(IS_LONGWORD_ALIGNED(Address));
    /* Wait for previous command to be completed */
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_COMMAND_COMPLETE) == RESET);
    /* Check previous error and clear */
    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_ACCESS_ERROR | FLASH_FLAG_PROTECT_VIOLATE) != RESET)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ACCESS_ERROR | FLASH_FLAG_PROTECT_VIOLATE);
    }
    /* Write to FCCOB command */
    __HAL_FLASH_WRITE_COMMAND(FLASH_COMMAND_ERASE);
    /* Write address */
    __HAL_FLASH_WRITE_ADDRESS_BIT0_BIT7(Address & 0xFF);
    __HAL_FLASH_WRITE_ADDRESS_BIT8_BIT15((Address >> 8U) & 0xFF);
    __HAL_FLASH_WRITE_ADDRESS_BIT16_BIT23((Address >> 16U) & 0xFF);
    /* Laund the command */
    __disable_irq();
    __HAL_FLASH_COMMAND_LAUNCH();
    /* Wait for flag to be set */
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_COMMAND_COMPLETE) == RESET);
    __enable_irq();
}

/**
 * @brief  Write a longword (4bytes)
 * @param  Address
 * @param  pData: pointer to data
 * @retval None
 */
void FLASH_WriteLongWord_Polling(const uint32_t Address,\
                                 const uint8_t *pData)
{
    assert(IS_LONGWORD_ALIGNED(Address));
    assert(pData != NULL);
    /* Wait for previous command to be completed */
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_COMMAND_COMPLETE) == RESET);
    /* Check previous error and clear */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ACCESS_ERROR | FLASH_FLAG_PROTECT_VIOLATE | FLASH_FLAG_READ_COLLISION);
    /* Write to FCCOB command */
    __HAL_FLASH_WRITE_COMMAND(FLASH_COMMAND_PROGRAM_LONGWORD);
    /* Write address */
    __HAL_FLASH_WRITE_ADDRESS_BIT0_BIT7(Address & 0xFF);
    __HAL_FLASH_WRITE_ADDRESS_BIT8_BIT15((Address >> 8U) & 0xFF);
    __HAL_FLASH_WRITE_ADDRESS_BIT16_BIT23((Address >> 16U) & 0xFF);
    /* Write Bytes */
    __HAL_FLASH_WRITE_BYTE_3(*(pData + 0U));
    __HAL_FLASH_WRITE_BYTE_2(*(pData + 1U));
    __HAL_FLASH_WRITE_BYTE_1(*(pData + 2U));
    __HAL_FLASH_WRITE_BYTE_0(*(pData + 3U));
    /* Laund the command */
    __disable_irq();
    __HAL_FLASH_COMMAND_LAUNCH();
    /* Wait for flag to be set */
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_COMMAND_COMPLETE) == RESET);
    __enable_irq();
}
/******************************************************************************
 * End of file
 ******************************************************************************/
