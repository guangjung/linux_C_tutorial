#ifndef __QUEUE_H__
#define __QUEUE_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct QUEUE_st {
    void *pvData;                /**< Data of the queue. */
    unsigned int uiCapacity;   /**< queue capacity in number of elements. */
    unsigned int uiElemSize;   /**< Size of one element in the queue */
    unsigned int uiHead;       /**< Index of the first element in the queue. */
    unsigned int uiCount;      /**< Number of elements stored in the queue. */
} QUEUE_ST;

int Queue_Init(const unsigned int uiCapacity, const unsigned int uiElemSize, QUEUE_ST **ppstQueue);
int Queue_EnqueueOverwrite(QUEUE_ST *pstQueue, void *pvItem);
int Queue_Enqueue(QUEUE_ST *pstQueue, void *pvItem);
int Queue_Dequeue(QUEUE_ST *pstQueue, void *pvItem);
void Queue_Cleanup(QUEUE_ST *pstQueue);

#ifdef __cplusplus
}
#endif
#endif