#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "queue.h"
typedef struct DATA_NODE_st
{
    int iNum;
} DATA_NODE_ST;

int
main(int argc, char **argv)
{
    QUEUE_ST *pstQueue = NULL;
    DATA_NODE_ST stData;
    DATA_NODE_ST *pstData = NULL;
    int iResult = 0;
    int i = 0;
    unsigned int uiPos = 0;

    iResult = Queue_Init(20, sizeof(DATA_NODE_ST), &pstQueue);
    if (iResult != 0) {
        goto ERR_LABEL;
    }

    for (i = 0; i < 30; i++) {
        stData.iNum = i + 1;
        iResult = Queue_EnqueueOverwrite(pstQueue, (void *)&stData);
        if (iResult != 0) {
            goto ERR_LABEL;
        }
    }

    for (i = 0; i < pstQueue->uiCount; i++) {
        uiPos = (pstQueue->uiHead + i) % pstQueue->uiCapacity;
        pstData = (DATA_NODE_ST *)(pstQueue->pvData + (uiPos * pstQueue->uiElemSize));
        if (i % 10 == 0) {
            printf("\r\n");
        }
        printf("(%d, %d) ", uiPos, pstData->iNum);
    }
    printf("\r\n\r\n");

    Queue_Cleanup(pstQueue);
    return 0;

ERR_LABEL:
    Queue_Cleanup(pstQueue);
    return -1;

}

