#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "ipc_common.h"
#include "ipc_semaphore.h"

int IPC_SEM_Create(unsigned int uiSemKey, int *piSemId)
{
	int iSemId = -1;
	
	//int semget(key_t key, int nsems, int semflg);
	
	iSemId = semget(uiSemKey, 1, IPC_CREAT | 0666);
	//iSemId = semget(uiSemKey, 1, IPC_CREAT | IPC_EXCL | 0660);
	if (iSemId < 0)
	{
		printf("%s semget failed!", __func__);
		//fflush(stdout);
		return RESULT_FAIL;
	}

	*piSemId = iSemId;

	return RESULT_OK;

}


int IPC_SEM_Get(unsigned int uiSemKey, int *piSemId)
{
	int iSemId = -1;
	
	//int semget(key_t key, int nsems, int semflg);
	
	iSemId = semget(uiSemKey, 0, 0);
	//iSemId = semget(uiSemKey, 1, IPC_CREAT | IPC_EXCL | 0660);
	if (iSemId < 0)
	{
		printf("%s semget failed!", __func__);
		return RESULT_FAIL;
	}

	*piSemId = iSemId;

	return RESULT_OK;

}

// P operation
int IPC_SEM_P(int iSemId)
{
	struct sembuf sem_op_wait;
	int iResult;
	sem_op_wait.sem_num = 0;        /* Operate on semaphore 0 */
    sem_op_wait.sem_op = -1;         /* Increment value by one */
    sem_op_wait.sem_flg = SEM_UNDO;

	//int semop(int semid, struct sembuf *sops, size_t nsops);

	iResult = semop(iSemId, &sem_op_wait, 1);
	if (iResult < 0)
	{
		printf("%s semop failed!", __func__);
		return RESULT_FAIL;

	}

	return RESULT_OK;
}

// V operation
int IPC_SEM_V(int iSemId)
{
	struct sembuf sem_op_wakeup;
	int iResult;
	sem_op_wakeup.sem_num = 0;        /* Operate on semaphore 0 */
    sem_op_wakeup.sem_op = 1;         /* Increment value by one */
    sem_op_wakeup.sem_flg = SEM_UNDO;

	//int semop(int semid, struct sembuf *sops, size_t nsops);
	iResult = semop(iSemId, &sem_op_wakeup, 1);
	if (iResult < 0)
	{
		printf("%s semop failed!", __func__);
		return RESULT_FAIL;

	}

	return RESULT_OK;
}



int IPC_SEM_SetValue(int iSemId, int iValue)
{
	union semun unSemArg;
	unSemArg.val = iValue;
	int iResult;

	// int semctl(int semid, int semnum, int cmd, ...);
	iResult = semctl(iSemId, 0, SETVAL, unSemArg);

	if (iResult < 0)
	{
		printf("%s semctl failed!", __func__);
		return RESULT_FAIL;
	}

	return RESULT_OK;
}


int IPC_SEM_GetValue(int iSemId, int *iValue)
{
	int iCount;

	// int semctl(int semid, int semnum, int cmd, ...);
	iCount = semctl(iSemId, 0, GETVAL);

	if (iCount < 0)
	{
		printf("%s semctl failed!", __func__);
		return RESULT_FAIL;
	}
	
	*iValue = iCount;
	
	return RESULT_OK;
}


int IPC_SEM_Delete(int iSemId)
{
	int iResult;
	//Immediately remove the semaphore set. The argument semnum is ignored
	iResult = semctl(iSemId, 0, IPC_RMID);

	if (iResult < 0)
	{
		printf("%s semctl failed!", __func__);
		return RESULT_FAIL;
	}

	return RESULT_OK;
}
