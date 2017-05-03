#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ipc_shared_memory.h"
#include "ipc_semaphore.h"
#include "ipc_common.h"


#define BUFFER_SIZE_MAX      (100)    

typedef struct SHM_BUFFER_st
{
	int aiBuf[BUFFER_SIZE_MAX];
	int iLow;
	int iHigh;
	int iCurNum;
}SHM_BUFFER_ST;
/*生产者与消费者模型*/

SHM_INFO_ST g_stShmInfo = {0xFFEEDD03, -1, sizeof(SHM_BUFFER_ST), NULL};

SEM_INFO_ST g_stEmpty = {0xFFEEBB01, -1};  //用来表示空位的数量，初值为BUFFER_SIZE_MAX(缓存区的规模)

SEM_INFO_ST g_stFull = {0xFFEEBB02, -1};   //用来表示剩余商品的数量，初值为0

SEM_INFO_ST g_stMutex = {0xFFEEBB03, -1};  //用来表示剩余商品的数量，初值为0


//start client for consume
void test_consumer()
{
	//
	int iResult = RESULT_OK;
	int iCount = 0;
	int iValue = -1;
	SHM_BUFFER_ST *pstShmBuf = NULL;
	
	iResult = IPC_SHM_Get(g_stShmInfo.uiShmKey,  &g_stShmInfo.iShmId, &g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

	iResult = IPC_SEM_Get(g_stEmpty.uiSemKey, &g_stEmpty.iSemId);
	iResult |= IPC_SEM_Get(g_stFull.uiSemKey, &g_stFull.iSemId);
	iResult |= IPC_SEM_Get(g_stMutex.uiSemKey, &g_stMutex.iSemId);
	if (iResult != RESULT_OK)
	{
		return;
	}

	iResult = IPC_SEM_GetValue(g_stEmpty.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Empty:", iResult, iValue);

	iResult = IPC_SEM_GetValue(g_stFull.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Full:", iResult, iValue);

	iResult = IPC_SEM_GetValue(g_stMutex.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Mutex:", iResult, iValue);

	pstShmBuf = (SHM_BUFFER_ST *)g_stShmInfo.pvShmAddr;

	for (;iCount < 50; iCount++)
	{
	
		if (iCount % 10 == 0)
		{
			printf("\r\n");
		}
		
		IPC_SEM_P(g_stFull.iSemId);
		IPC_SEM_P(g_stMutex.iSemId);

		if (pstShmBuf->iCurNum > 0)
		{

			pstShmBuf->iLow++;
			if (pstShmBuf->iLow >= BUFFER_SIZE_MAX)
			{

				pstShmBuf->iLow = 0;
			}
			
			printf("%5d ", pstShmBuf->aiBuf[pstShmBuf->iLow]);
			fflush(stdout);
			pstShmBuf->aiBuf[pstShmBuf->iLow] = 0; // clean 
			pstShmBuf->iCurNum--;
		}
		else
		{
			printf("\r\n ********NO DATA*****\r\n");

		}
		
		IPC_SEM_V(g_stMutex.iSemId);
		IPC_SEM_V(g_stEmpty.iSemId);
	
	}
	
	printf("\r\n");
	printf("\r\n");
	
	iResult = IPC_SHM_Detach(&g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

}

//start server for produce
void test_producer()
{
	int iResult = RESULT_OK;
	SHM_BUFFER_ST *pstShmBuf = NULL;
	int iCount = 1;
	static int iNum = 0;
	int iValue = -1;
	
	
	iResult = IPC_SHM_Create(g_stShmInfo.uiShmKey, g_stShmInfo.ulShmSize, &g_stShmInfo.iShmId, &g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

	iResult = IPC_SEM_Create(g_stEmpty.uiSemKey, &g_stEmpty.iSemId);
	iResult |= IPC_SEM_Create(g_stFull.uiSemKey, &g_stFull.iSemId);
	iResult |= IPC_SEM_Create(g_stMutex.uiSemKey, &g_stMutex.iSemId);
	if (iResult != RESULT_OK)
	{
		return ;
	}

	iResult = IPC_SEM_SetValue(g_stEmpty.iSemId, BUFFER_SIZE_MAX);
	iResult |= IPC_SEM_SetValue(g_stFull.iSemId, 0);
	iResult |= IPC_SEM_SetValue(g_stMutex.iSemId, 1);
	if (iResult != RESULT_OK)
	{
		return ;
	}

	iResult = IPC_SEM_GetValue(g_stEmpty.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Empty:", iResult, iValue);

	iResult = IPC_SEM_GetValue(g_stFull.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Full:", iResult, iValue);

	iResult = IPC_SEM_GetValue(g_stMutex.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Mutex:", iResult, iValue);
	
	pstShmBuf = (SHM_BUFFER_ST *)g_stShmInfo.pvShmAddr;
	pstShmBuf->iLow = -1;
	
	while (1)
	{
		sleep(5);
		
		for (iCount = 0; iCount < 50; iCount++)
		{

			IPC_SEM_P(g_stEmpty.iSemId); //减少一个空位
			IPC_SEM_P(g_stMutex.iSemId); //锁上互斥锁
			
			//pstShmBuf->iCur++;
			pstShmBuf = (SHM_BUFFER_ST *)g_stShmInfo.pvShmAddr;
			
			pstShmBuf->aiBuf[pstShmBuf->iHigh] = iNum;

			pstShmBuf->iHigh++;
			if (pstShmBuf->iHigh >= BUFFER_SIZE_MAX)
			{

				pstShmBuf->iHigh = 0;
			}
			
			if (iCount % 10 == 0)
			{
				printf("\r\n");
			}
			printf("%5d ", iNum++);
			//fflush(stdout);
			pstShmBuf->iCurNum++;
			IPC_SEM_V(g_stMutex.iSemId);
	    	IPC_SEM_V(g_stFull.iSemId);
		}

		printf("\r\n");
		printf("\r\n");
	}

}

//delete existed shared memory segment and all semaphores
void test_delete_shm_sem()
{
	//
	int iResult = RESULT_OK;
	
	iResult = IPC_SHM_GetId(g_stShmInfo.uiShmKey,  &g_stShmInfo.iShmId);
	if (iResult != RESULT_OK)
	{
		return;
	}

	IPC_SHM_Delete(g_stShmInfo.iShmId, NULL);


	//
	iResult |= IPC_SEM_Get(g_stEmpty.uiSemKey,&g_stEmpty.iSemId);
	iResult |= IPC_SEM_Get(g_stFull.uiSemKey,&g_stFull.iSemId);
	iResult |= IPC_SEM_Get(g_stMutex.uiSemKey,&g_stMutex.iSemId);
	if (iResult != RESULT_OK)
	{
		return;
	}

	IPC_SEM_Delete(g_stEmpty.iSemId);
	IPC_SEM_Delete(g_stFull.iSemId);
	IPC_SEM_Delete(g_stMutex.iSemId);

}


//start client for consume
void test_print_shm()
{
	//
	int iResult = RESULT_OK;
	SHM_BUFFER_ST *pstShmBuf = NULL;
	int iValue = 0;
	int i = 0;
	
	iResult = IPC_SHM_Get(g_stShmInfo.uiShmKey,  &g_stShmInfo.iShmId, &g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}


	pstShmBuf = (SHM_BUFFER_ST *)g_stShmInfo.pvShmAddr;

	iResult = IPC_SEM_Get(g_stEmpty.uiSemKey, &g_stEmpty.iSemId);
	iResult |= IPC_SEM_Get(g_stFull.uiSemKey, &g_stFull.iSemId);
	iResult |= IPC_SEM_Get(g_stMutex.uiSemKey, &g_stMutex.iSemId);
	if (iResult != RESULT_OK)
	{
		return;
	}

	iResult = IPC_SEM_GetValue(g_stEmpty.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Empty:", iResult, iValue);

	iResult = IPC_SEM_GetValue(g_stFull.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Full:", iResult, iValue);

	iResult = IPC_SEM_GetValue(g_stMutex.iSemId,&iValue);
	printf("\r\n[%s]iResult =%d, iValue = %d\r\n", "Mutex:", iResult, iValue);
	

	printf("\r\nLow:%d, High:%d, iCurNum:%d\r\n", pstShmBuf->iLow, pstShmBuf->iHigh, pstShmBuf->iCurNum);

	
	for (i = 0; i < BUFFER_SIZE_MAX; i++)
	{
		if (i % 10 == 0)
		{
			printf("\r\n");
		}

		printf("%5d ", pstShmBuf->aiBuf[i]);
	}
	printf("\r\n");
	printf("\r\n");
	iResult = IPC_SHM_Detach(&g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

}

//./test_shared_memory [client | server]
int main(int argc, char **argv)
{

	if (1 == argc)
	{
		goto ERR_LABEL;
	}
    else
    {
    	//Please first start server (./test_shared_memory server), then excute client
		if (0 == strcmp("client", argv[1]))
		{
			test_consumer();
		}
		else if (0 == strcmp("server", argv[1]))
		{
			test_producer();
		}
		else if (0 == strcmp("delete", argv[1]))
		{
			//delete share memory segment
			test_delete_shm_sem();
		}
		else if (0 == strcmp("print", argv[1]))
		{
			//delete share memory segment
			test_print_shm();
		}
		else
		{
			goto ERR_LABEL;
		}
	}

	return 0;
	
ERR_LABEL:

	printf("\r\nThe format of command is error!");
	printf("\r\nFormat:");
	printf("\r\n    ./test_producer_consumer server");
	printf("\r\n    ./test_producer_consumer client");
	printf("\r\n    ./test_producer_consumer delete");
	printf("\r\n");

	return 1;
}

