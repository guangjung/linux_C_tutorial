
#ifndef _IPC_COMMON_H_
#define _IPC_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif


#define RESULT_OK           (0)
#define RESULT_FAIL         (1)


typedef enum LOG_LEVEL_en
{
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_TRACE = 1,
	LOG_LEVEL_INFO =  2,
	LOG_LEVEL_WARN =  3,
	LOG_LEVEL_ERROR = 4

}LOG_LEVEL_EN;


#define LOG_PRINT_WARN(acLogInfo, args...)   \
	do {\
		LOG_Print(LOG_LEVEL_WARN, __FILE__, __LINE__, acLogInfo, ##args);\
	} while(0)

#define LOG_PRINT_ERR(acLogInfo, args...)   \
	do {\
		LOG_Print(LOG_LEVEL_ERROR, __FILE__, __LINE__, acLogInfo, ##args);\
	} while(0)

void LOG_Print(int iLogLevel, const char *pcFileName, int iLine, const char *pcLogInfo ,...);

#ifdef __cplusplus
}
#endif
#endif

