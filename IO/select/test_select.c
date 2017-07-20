#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int i = 0;
	char acSendMsg[200];
	
	for (i = 0; i < 1000; i++)
	{
	    snprintf(acSendMsg, sizeof(acSendMsg), "./client_select \"request [%d]\"", i +1);
	    system(acSendMsg);
	}
	
	return 0;

}