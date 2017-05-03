
#ifndef _IPC_SHARED_MEMORY_H_
#define _IPC_SHARED_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SHM_INFO_st
{
	const unsigned int uiShmKey;     //key value of shared memory 
	int iShmId;                       //
	const unsigned long ulShmSize;
	void *pvShmAddr;

}SHM_INFO_ST;


int IPC_SHM_Create(unsigned int uiShmKey, unsigned long ulShmSize, int *piShmId, void **ppvShmAddr);
int IPC_SHM_Detach(void **ppvShmAddr);
int IPC_SHM_GetStatus(int iShmId);
int IPC_SHM_Delete(int iShmId, void *pvShmAddr);
int IPC_SHM_Get(unsigned int uiShmKey, int *piShmId, void **ppvShmAddr);
int IPC_SHM_GetId(unsigned int uiShmKey, int *piShmId);


#ifdef __cplusplus
}
#endif
#endif
