/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MKE16Z4_DEF
#define __MKE16Z4_DEF

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/

/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
  DRV_ERROR_NONE = 0x00U,
  DRV_ERROR      = 0x01U,
  DRV_BUSY       = 0x02U,
  DRV_TIMEOUT    = 0x03U
} DRV_StatusTypeDef;

/**
 * @brief  Flag and interrupt status
 */
typedef enum
{
  RESET = 0U,
  SET = !RESET
} FlagStatus, ITStatus;

/**
 * @brief  Functional status
 */
typedef enum
{
  DISABLE = 0U,
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

/**
 * @brief  Error status
 */
typedef enum
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;

/**
  * @brief  HAL Lock structures definition
  */
typedef enum
{
  DRV_UNLOCKED = 0x00U,
  DRV_LOCKED   = 0x01U
} DRV_LockTypeDef;

/* Exported macro ------------------------------------------------------------*/

#define NULL                      ((void *)0U)
/* For functions without body to not generate warning, X is a parameter or local variable*/
#define UNUSED(X)                  ((void *)X)

/* Convert pointer to address without warnings */
#define TO_ADDRESS(PTR)               ((uint32_t)(PTR))

/* Convert address to a pointer without warnings */
#define TO_POINTER(ADDR)              ((void *)(ADDR))

/* Convert index to bit position */
#define BIT_POS(NUM)               ((uint32_t)1U << NUM)

#define IS_NVIC_PRIORITY(PRIORITY)  ((PRIORITY) < 0x05U)

#endif /* ___MKE16Z4_DEF */

/******************************************************************************
 * End of file
 ******************************************************************************/
