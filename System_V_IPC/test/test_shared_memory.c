#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ipc_shared_memory.h"
#include "ipc_common.h"

SHM_INFO_ST g_stShmInfo = {0xFFEEDD01, -1, 4096, NULL};

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
void test_shm_client(const char *pcStr)
{
	//
	int iResult = RESULT_OK;
	iResult = IPC_SHM_Get(g_stShmInfo.uiShmKey,  &g_stShmInfo.iShmId, &g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

	snprintf((char *) g_stShmInfo.pvShmAddr, strlen(pcStr) + 1, "%s", pcStr);

	sleep(5);
	iResult = IPC_SHM_Detach(&g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}

}

//Read data to shared memory segment 
//shared memory segment is created by server;
void test_shm_server()
{
	int iResult = RESULT_OK;
	static int iCount = 0;
	iResult = IPC_SHM_Create(g_stShmInfo.uiShmKey, g_stShmInfo.ulShmSize, &g_stShmInfo.iShmId, &g_stShmInfo.pvShmAddr);
	if (iResult != RESULT_OK)
	{
		return;
	}
	
	while (1)
	{
		sleep(5);

		printf("\r\n[%04d] %s \r\n", ++iCount, (char *)g_stShmInfo.pvShmAddr);
		
	}

}

//delete existed shared memory segment
void test_delete_shm()
{
	//
	int iResult = RESULT_OK;
	iResult = IPC_SHM_GetId(g_stShmInfo.uiShmKey,  &g_stShmInfo.iShmId);
	if (iResult != RESULT_OK)
	{
		return;
	}

	IPC_SHM_Delete(g_stShmInfo.iShmId, NULL);

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
			test_shm_client((2 == argc) ? "Hello World." : argv[2]);
		}
		else if (0 == strcmp("server", argv[1]))
		{
			test_shm_server();
		}
		else if (0 == strcmp("delete", argv[1]))
		{
			//delete share memory segment
			test_delete_shm();
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

