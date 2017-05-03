#ifndef _IPC_SEMAPHORE_H_
#define _IPC_SEMAPHORE_H_
#ifdef __cplusplus
extern "C" {
#endif

union semun 
{
    int   val;               /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

typedef struct SEM_INFO_st
{
	const unsigned int uiSemKey;     //key value of semaphore 
	int iSemId;                       //
}SEM_INFO_ST;

int IPC_SEM_Create(unsigned int uiSemKey, int *piSemId);
int IPC_SEM_Get(unsigned int uiSemKey, int *piSemId);
int IPC_SEM_P(int iSemId);
int IPC_SEM_V(int iSemId);
int IPC_SEM_SetValue(int iSemId, int iValue);
int IPC_SEM_GetValue(int iSemId, int *iValue);
int IPC_SEM_Delete(int iSemId);

#ifdef __cplusplus
}
#endif

#endif
