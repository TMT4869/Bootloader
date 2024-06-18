#ifndef _EMB_QUEUE
#define _EMB_QUEUE

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/******************************************************************************
 * EXPORTED TYPEDEF
 ******************************************************************************/

/**
 * @brief Queue control struct
 */
typedef struct __Queue_Struct
{
    uint32_t elementCount;   /*< Count of queue's elements */
    uint32_t elementSize;    /*< Size of each element */
    uint32_t queueFront;     /*< Points to the front (head) of the queue storage area. */
    uint32_t queueBack;      /*< Points to the back (tail) of the queue storage area */
    uint32_t queueSize;      /*< Size of queue */
    uint8_t *queueBuffer;    /*< Data memory */
}Queue_TypeDef;

/**
 * @brief Queue errors enumeration
 */
typedef enum __Queue_Status_Enum
{
    QUEUE_OK,
    QUEUE_ERROR_FULL,
    QUEUE_ERROR_EMPTY,
    QUEUE_BUSY
}Queue_StatusTypeDef;
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
 * @brief  Initialize queue
 */
void Queue_Init(Queue_TypeDef *pQueue,\
                uint8_t *dataBuffer,\
                const uint32_t elementSize,\
                const uint32_t queueSize);

/**
 * @brief  Enqueue data to queue
 */
Queue_StatusTypeDef Queue_Enqueue(Queue_TypeDef *pQueue,\
                                  const uint8_t *pData,\
                                  const uint32_t numOfElements);

/**
 * @brief  Dequeue data and store it to a buffer
 */
Queue_StatusTypeDef Queue_Dequeue(Queue_TypeDef *pQueue,\
                                  uint8_t *pBuffer,\
                                  const uint32_t numOfElements);


/**
 * @brief Check if a queue is full
 */
bool Queue_IsFull(const Queue_TypeDef *pQueue);

/**
 * @brief Check if a queue is empty
 */
bool Queue_IsEmpty(const Queue_TypeDef *pQueue);

/**
 * @brief  Increase the index of the back (used when data is enqueued by other means)
 */
Queue_StatusTypeDef Queue_BackPushIndex(Queue_TypeDef *pQueue, const uint32_t numOfElements);

/**
 * @brief Increase the index of the front (used when data is dequeued by other means)
 */
Queue_StatusTypeDef Queue_FrontPopIndex(Queue_TypeDef *pQueue, const uint32_t numOfElements);

/**
 * @brief Peek inside the front of queue
 */
const uint8_t *Queue_PeekFront(const Queue_TypeDef *pQueue);

/**
 * @brief Peek inside the back of queue
 */
const uint8_t *Queue_PeekBack(const Queue_TypeDef *pQueue);

/**
 * @brief Get the buffer of the element at the back of the queue
 */
uint8_t *Queue_GetBackBuffer(const Queue_TypeDef *pQueue);
#endif /* _EMB_QUEUE */

/******************************************************************************
 * End of file
 ******************************************************************************/
