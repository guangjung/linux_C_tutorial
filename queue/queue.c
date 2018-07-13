#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

int
Queue_Init(const unsigned int uiCapacity, const unsigned int uiElemSize, QUEUE_ST **ppstQueue)
{
    QUEUE_ST *pstQueueTemp = NULL;

    if (NULL == ppstQueue) {
        return -1;
    }

    pstQueueTemp = calloc(1, sizeof(*ppstQueue));
    if (NULL == pstQueueTemp) {
        return -1;
    }

    pstQueueTemp->pvData = calloc(uiCapacity, uiElemSize);
    if (NULL == pstQueueTemp) {
        goto CLEAN_UP;
    }

    pstQueueTemp->uiCapacity = uiCapacity;
    pstQueueTemp->uiElemSize = uiElemSize;
    pstQueueTemp->uiHead = 0;
    pstQueueTemp->uiCount = 0;

    *ppstQueue = pstQueueTemp;
    return 0;

CLEAN_UP:
    free(pstQueueTemp);
    return -1;
}

/**
 * scenario:  always add new data to queue whenever queue is full or not
 */
int
Queue_EnqueueOverwrite(QUEUE_ST *pstQueue, void *pvItem)
{
    unsigned int uiPos = 0;

    if (NULL == pstQueue || NULL == pvItem) {

        return -1;
    }

    if (pstQueue->uiCount == pstQueue->uiCapacity) {
        /* queue is full,  overwrite the first data */
        uiPos = pstQueue->uiHead % pstQueue->uiCapacity;

        /* move head position */
        pstQueue->uiHead = (pstQueue->uiHead + 1) % pstQueue->uiCapacity;
    } else {
        uiPos = (pstQueue->uiHead + pstQueue->uiCount) % pstQueue->uiCapacity;
        pstQueue->uiCount++;
    }

    memcpy((pstQueue->pvData + (uiPos * pstQueue->uiElemSize)), pvItem, pstQueue->uiElemSize);
    /* pstQueue->uiCount = (pstQueue->pvData); */

    return 0;
}

/**
 * scenario:  always add new data to queue until queue is full.
 */
int
Queue_Enqueue(QUEUE_ST *pstQueue, void *pvItem)
{
    unsigned int uiPos = 0;

    if (NULL == pstQueue || NULL == pvItem) {

        return -1;
    }

    if (pstQueue->uiCount >= pstQueue->uiCapacity) {
        /* queue is full */
        return -1;
    }

    uiPos = (pstQueue->uiHead + pstQueue->uiCount) % pstQueue->uiCapacity;
    memcpy((pstQueue->pvData + (uiPos * pstQueue->uiElemSize)), pvItem, pstQueue->uiElemSize);
    pstQueue->uiCount++;

    return 0;
}

int
Queue_Dequeue(QUEUE_ST *pstQueue, void *pvItem)
{
    unsigned int uiPos = 0;

    if (NULL == pstQueue || NULL == pvItem) {

        return -1;
    }

    if (0 == pstQueue->uiCount) {
        /* queue is full */
        return 0;
    }

    uiPos = pstQueue->uiHead % pstQueue->uiCapacity;
    memcpy((pstQueue->pvData + (uiPos * pstQueue->uiElemSize)), pvItem, pstQueue->uiElemSize);

    /* move head position */
    pstQueue->uiHead++;
    /* Decrement queue element number */
    pstQueue->uiCount--;

    return 0;
}

void
Queue_Cleanup(QUEUE_ST *pstQueue)
{
    if (NULL != pstQueue) {
        free(pstQueue->pvData);
        free(pstQueue);
    }
}