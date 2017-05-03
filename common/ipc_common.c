void LOG_Print(int iLogLevel, const char *pcFileName, int iLine, const char *pcLogInfo ,...)
{
	char acLog[512] = {0};
	char acLevel[8] = {0};
	va_list ap;

	va_start(ap, pcLogInfo);
	vsnprintf(acLog, sizeof(acLog), pcLogInfo, ap);
	va_end(ap);


	switch (iLogLevel)
	{
		case LOG_LEVEL_DEBUG:
			snprintf(acLevel, sizeof(acLevel), "%s", "DBG");
			break;
		case LOG_LEVEL_TRACE:
			snprintf(acLevel, sizeof(acLevel), "%s", "TRC");
			break;
		case LOG_LEVEL_INFO:
			snprintf(acLevel, sizeof(acLevel), "%s", "INO");
			break;
		case LOG_LEVEL_WARN:
			snprintf(acLevel, sizeof(acLevel), "%s", "WRN");
			break;
		case LOG_LEVEL_ERROR:
			snprintf(acLevel, sizeof(acLevel), "%s", "ERR");
			break;
		default:
			snprintf(acLevel, sizeof(acLevel), "%s", "UNK");
			break;
	}
	
	printf("\r\n[%s] %20s [%d] %s\r\n", acLevel, pcFileName, iLine, acLog);
	fflush(stdout);
	
	return;
}
