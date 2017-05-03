#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ipc_shared_memory.h"
#include "ipc_semaphore.h"
#include "ipc_common.h"

SHM_INFO_ST g_stShmInfo = {0xFFEEDD02, -1, 4096, NULL};

SEM_INFO_ST g_stSemInfo = {0xFFEECC02, -1};

void test_get_shm_status_info()
{
	int iResult = RESULT_OK;
	
	iResult = IPC_SHM_Create(g_stShmInfo.uiShmKey, g_stShmInfo.ulShmSize, &g_stShmInfo.iShmId, &g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

	//
	iResult = IPC_SHM_GetStatus(g_stShmInfo.iShmId);
	if (iResult != RESULT_OK)
	{
		printf("\r\n IPC_SHM_GetStatus failed !\r\n");
	}

	//
	iResult = IPC_SHM_Delete(g_stShmInfo.iShmId, g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		printf("\r\n IPC_SHM_Delete failed !\r\n");
	}

	return;

}

//write data to shared memory segment
void test_shm_sem_client(const char *pcStr)
{
	//
	int iResult = RESULT_OK;
	int iCount = 1;
	int iValue = -1;
	
	iResult = IPC_SHM_Get(g_stShmInfo.uiShmKey,  &g_stShmInfo.iShmId, &g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

	iResult = IPC_SEM_Get(g_stSemInfo.uiSemKey, &g_stSemInfo.iSemId);
	if (iResult != RESULT_OK)
	{
		return;
	}

	iResult = IPC_SEM_SetValue(g_stSemInfo.iSemId, 1);
	if (iResult != RESULT_OK)
	{
		return ;
	}

	iResult = IPC_SEM_GetValue(g_stSemInfo.iSemId,&iValue);
	printf("\r\n[%d]iResult =%d, iValue = %d\r\n", __LINE__, iResult, iValue);
	
	for (;iCount <= 100; iCount++)
	{
		IPC_SEM_P(g_stSemInfo.iSemId);
		snprintf((char *) g_stShmInfo.pvShmAddr, 50, "%s[%d]", pcStr, iCount);
		IPC_SEM_V(g_stSemInfo.iSemId);
	}
	
	iResult = IPC_SHM_Detach(&g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

}

//Read data to shared memory segment 
//shared memory segment is created by server;
void test_shm_sem_server()
{
	int iResult = RESULT_OK;
	static int iCount = 1;
	int iValue = -1;
	
	
	iResult = IPC_SHM_Create(g_stShmInfo.uiShmKey, g_stShmInfo.ulShmSize, &g_stShmInfo.iShmId, &g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		fflush(stdout);
		return;
	}

	iResult = IPC_SEM_Create(g_stSemInfo.uiSemKey, &g_stSemInfo.iSemId);
	if (iResult != RESULT_OK)
	{
		fflush(stdout);
		return ;
	}

	iResult = IPC_SEM_GetValue(g_stSemInfo.iSemId,&iValue);
	printf("\r\n[%d]iResult =%d, iValue = %d\r\n", __LINE__, iResult, iValue);

	iResult = IPC_SEM_SetValue(g_stSemInfo.iSemId, 0);
	if (iResult != RESULT_OK)
	{
		return ;
	}

	iResult = IPC_SEM_GetValue(g_stSemInfo.iSemId,&iValue);
	printf("\r\n[%d]iResult =%d, iValue = %d\r\n", __LINE__, iResult, iValue);
	
	while (1)
	{
		sleep(5);
		printf("\r\n");
		for (iCount = 1; iCount <= 50; iCount++)
		{
			IPC_SEM_P(g_stSemInfo.iSemId);
			printf("[%04d] %s \r\n", iCount, (char *)g_stShmInfo.pvShmAddr);
			IPC_SEM_V(g_stSemInfo.iSemId);
		}
		
	}

}

//delete existed shared memory segment
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
	IPC_SEM_Get(g_stSemInfo.uiSemKey,&g_stSemInfo.iSemId);
	if (iResult != RESULT_OK)
	{
		return;
	}
	
	IPC_SEM_Delete(g_stSemInfo.iSemId);

}

//./test_shared_memory [client | server]
int main(int argc, char **argv)
{

	if (1 == argc)
	{
		test_get_shm_status_info();
	}
    else
    {
    	//Please first start server (./test_shared_memory server), then excute client
		if (0 == strcmp("client", argv[1]))
		{
			test_shm_sem_client((2 == argc) ? "Hello World." : argv[2]);
		}
		else if (0 == strcmp("server", argv[1]))
		{
			test_shm_sem_server();
		}
		else if (0 == strcmp("delete", argv[1]))
		{
			//delete share memory segment
			test_delete_shm_sem();
		}
		else
		{
			printf("\r\nThe format of command is error!");
			printf("\r\nFormat:");
			printf("\r\n    ./test_shared_memory");
			printf("\r\n    ./test_shared_memory server");
			printf("\r\n    ./test_shared_memory client [option]");
			printf("\r\n    ./test_shared_memory delete");
			printf("\r\n");
		}
	}

	return 0;
}

