#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <unistd.h>     //long sysconf(int name); 
#include "ipc_common.h"


static int CheckShmSize(unsigned long ulShmSize)
{
	long lPageSize = sysconf(_SC_PAGESIZE);

	if ((ulShmSize % lPageSize) != 0)
	{
		printf("\r\nSuggested that the size of a shared memory segment is a multiple of PAGE_SIZE\r\n");
		//return RESULT_FAIL;
	}
	
	return RESULT_OK;
}

int IPC_SHM_Create(unsigned int uiShmKey, unsigned long ulShmSize, int *piShmId, void **ppvShmAddr)
{
	//int iResult = RESULT_OK;
	int iShmId;
	void *pvShmAddr = NULL;

	if (NULL == piShmId || NULL == ppvShmAddr)
	{
		goto ERR_LABEL;
	}
	
    //int shmget(key_t key, size_t size, int shmflg);

	if (CheckShmSize(ulShmSize) != RESULT_OK)
	{
		goto ERR_LABEL; 
	}
    
	//iShmId = shmget(uiShmKey, ulShmSize, IPC_CREAT | IPC_EXCL | 0666);
	iShmId = shmget(uiShmKey, ulShmSize, IPC_CREAT | 0666);
	if (iShmId < 0)
	{
		printf("create shared memory failed!");
		goto ERR_LABEL;
	}

	//void *shmat(int shmid, const void *shmaddr, int shmflg);
	pvShmAddr = shmat(iShmId, NULL, 0);

	if ((void *)-1 == pvShmAddr)
	{
		printf("\r\nshmat failed!\r\n");
		goto ERR_LABEL;
	}

	memset(pvShmAddr, 0, ulShmSize);
	*piShmId = iShmId;
	*ppvShmAddr = pvShmAddr;
	return RESULT_OK;
	
ERR_LABEL:
	//delete iShmId
	
	return RESULT_FAIL;
}


int IPC_SHM_Detach(void **ppvShmAddr)
{
    int iResult = RESULT_OK;
	//int shmdt(const void *shmaddr);
	if (NULL == ppvShmAddr || NULL == *ppvShmAddr)
	{
		goto ERR_LABEL;
	}
	
	iResult = shmdt(*ppvShmAddr);
	if (iResult < 0)
	{
		printf("\r\nshmdt failed!\r\n");
		goto ERR_LABEL;
	}

	*ppvShmAddr = NULL;

	return RESULT_OK;
	
ERR_LABEL:
	
	return RESULT_FAIL;
}


//int shmctl(int shmid, int cmd, struct shmid_ds *buf);
int IPC_SHM_GetStatus(int iShmId)
{
	struct shmid_ds stBuf;
	int iResult;
	struct tm *pstTm = NULL;
	char acTime[60] = {0};
	
	memset(&stBuf, 0, sizeof(stBuf));
	iResult = shmctl(iShmId, IPC_STAT, &stBuf);

	if (iResult < 0)
	{
		printf("\r\nshmctl failed!\r\n");
		return RESULT_FAIL;
	}

	printf("\r\nSize of segment (bytes) : %ld \r\n", stBuf.shm_segsz);

	printf("\r\nPID of creator : %d \r\n", stBuf.shm_cpid);

	printf("\r\nPID of last shmat(2)/shmdt(2) : %d \r\n", stBuf.shm_lpid);

	//
	pstTm = localtime(&stBuf.shm_atime); 
	snprintf(acTime, sizeof(acTime), "%4d-%2d-%2d %2d:%2d:%2d", (1900 + pstTm->tm_year), ( 1 + pstTm->tm_mon), pstTm->tm_mday,
									pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);
	printf("\r\nLast attach time : %s \r\n", acTime);

	//
	pstTm = localtime(&stBuf.shm_dtime); 
	snprintf(acTime, sizeof(acTime), "%4d-%2d-%2d %2d:%2d:%2d", (1900 + pstTm->tm_year), ( 1 + pstTm->tm_mon), pstTm->tm_mday,
									pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);
	printf("\r\nLast detach time : %s \r\n", acTime);

	//
	pstTm = localtime(&stBuf.shm_ctime); 
	snprintf(acTime, sizeof(acTime), "%4d-%2d-%2d %2d:%2d:%2d", (1900 + pstTm->tm_year), ( 1 + pstTm->tm_mon), pstTm->tm_mday,
									pstTm->tm_hour, pstTm->tm_min, pstTm->tm_sec);
	printf("\r\nLast change time : %s \r\n", acTime);
	
	return RESULT_OK;

}

int IPC_SHM_Delete(int iShmId, void *pvShmAddr)
{
	int iResult = RESULT_OK;

	if (pvShmAddr != NULL)
	{
		IPC_SHM_Detach(&pvShmAddr);
	}
	
	iResult = shmctl(iShmId, IPC_RMID, NULL);

	if (iResult < 0)
	{
		printf("\r\nshmctl failed!\r\n");
		return RESULT_FAIL;
	}

	return RESULT_OK;
}
/**/
int IPC_SHM_Get(unsigned int uiShmKey, int *piShmId, void **ppvShmAddr)
{
	//int iResult = RESULT_OK;
	int iShmId;
	void *pvShmAddr = NULL;

	if (NULL == piShmId || NULL == ppvShmAddr)
	{
		goto ERR_LABEL;
	}
	
    //int shmget(key_t key, size_t size, int shmflg);  
	iShmId = shmget(uiShmKey, 0, 0);
	if (iShmId < 0)
	{
		//printf("get share memory failed!");
		printf("\r\nshmget failed!\r\n");
		goto ERR_LABEL;
	}

	//void *shmat(int shmid, const void *shmaddr, int shmflg);
	pvShmAddr = shmat(iShmId, NULL, 0);
	if ((void *)-1 == pvShmAddr)
	{
		printf("\r\nshmat failed!\r\n");
		goto ERR_LABEL;
	}


	*piShmId = iShmId;
	*ppvShmAddr = pvShmAddr;
	return RESULT_OK;
	
ERR_LABEL:
	
	return RESULT_FAIL;

}


int IPC_SHM_GetId(unsigned int uiShmKey, int *piShmId)
{
	//int iResult = RESULT_OK;
	int iShmId;

	if (NULL == piShmId)
	{
		goto ERR_LABEL;
	}
    //int shmget(key_t key, size_t size, int shmflg);  
	iShmId = shmget(uiShmKey, 0, 0);
	if (iShmId < 0)
	{
		//printf("get share memory failed!");
		printf("\r\nshmget failed!\r\n");
		goto ERR_LABEL;
	}
	
	*piShmId = iShmId;
	return RESULT_OK;
	
ERR_LABEL:
	
	return RESULT_FAIL;
}


int IPC_SHM_Write(void *pvShmAddr, const void *pvData, unsigned int uiLen)
{

	memcpy(pvShmAddr, pvData, uiLen);
	
	return RESULT_OK;
}

int IPC_SHM_Read(const void *pvShmAddr, void *pvData, unsigned int uiLen)
{

	memcpy(pvData, pvShmAddr, uiLen);
	
	return RESULT_OK;
}

