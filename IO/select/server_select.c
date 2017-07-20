/*******************************************
@Author: guangjung
@Date:   2017/07/20
********************************************/


#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> //for htons
#include <unistd.h>  //for close


#include "common.h"
static int m_iResponseNum = 0;
int ProcessMsg(int newfd)
{
	MSG_ST stRecvMsg;
	MSG_ST stRespMsg;

	memset(&stRecvMsg, 0, MSG_LEN_MAX);
	if (-1 != recv(newfd, &stRecvMsg, MSG_LEN_MAX, 0))
	{
		
		printf("\r\n Receive:");
		printf("\r\n\t TYPE: %d", stRecvMsg.stMsgHead.iType);
		printf("\r\n\t LEN: %d", stRecvMsg.stMsgHead.uiMsgBodyLen);
		printf("\r\n\t Name: %s", stRecvMsg.stMsgHead.acName);
		printf("\r\n\t Body: %s", stRecvMsg.stMsgBody.acBody);
		printf("\r\n***************************\r\n");
		
		//TODO
		memset(&stRespMsg, 0, MSG_LEN_MAX);
		stRespMsg.stMsgHead.iType = MSG_TYPE_RESPONSE;
		snprintf(stRespMsg.stMsgHead.acName, sizeof(stRespMsg.stMsgHead.acName), 
			"%s", "server");
		snprintf(stRespMsg.stMsgBody.acBody, sizeof(stRespMsg.stMsgBody.acBody), 
			"%s response %d", "server", ++m_iResponseNum);
		stRespMsg.stMsgHead.uiMsgBodyLen = strlen(stRespMsg.stMsgBody.acBody);
	
		
		if (-1 == send(newfd, &stRespMsg, MSG_LEN_MAX, 0))
		{
			//TODO
			printf("\r\n send failed.\r\n");
			return -1;
		}
	}
	else
	{
		printf("\r\n recv failed.\r\n");
		return -1;
	}

	return 0;
	
}
int main()
{
	int fd;
	int newfd;
	int result;
	struct sockaddr_in sin;
	struct sockaddr_in cin;
	socklen_t addrlen;
	fd_set allset;
	fd_set readset;
	struct timeval timeout;
	
	
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;  //使用IPV4 通信域
	sin.sin_addr.s_addr =INADDR_ANY; //服务器可以接受任意地址
	//inet_pton(AF_INET, "127.0.0.1", &sin);
	sin.sin_port = htons(SEVER_PORT); //端口号转化为网络字节序

	//create socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == fd)
	{
		printf("\r\n line[%d]create socket failed!\r\n", __LINE__);
		return -1;
	}

    if (-1 == bind(fd, (struct sockaddr *) &sin, sizeof(sin)))
    {
		printf("\r\n line[%d]bind socket failed!\r\n", __LINE__);
		goto ERR_LABEL;
	}

	if (-1 == listen(fd, PENDING_CONNECTION_NUM_MAX))
	{
		printf("\r\n listen socket failed!\r\n");
		goto ERR_LABEL;
	}
	FD_ZERO(&allset);
	FD_SET(fd, &allset);

	memset(&timeout, 0, sizeof(timeout));
	//timeout.tv_sec = 3;
	//timeout.tv_usec = 0;
	
	while (1)
	{
		readset = allset;
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;
		
		result = select(fd + 1, &readset, NULL, NULL, &timeout);
		if (-1 == result)
		{
			continue;
		}
		if (!FD_ISSET(fd, &readset))
		{
			continue;
		}
		addrlen = sizeof(cin);
		newfd = accept(fd, (struct sockaddr *)&cin, &addrlen);
		if (-1 == newfd)
		{
			continue;
		}
        //TODO
        ProcessMsg(newfd);
		
		if (-1 == close(newfd))
		{
			printf("\r\n close failed.\r\n");
		}				
		
	}

ERR_LABEL:
	
    if (-1 == close(fd))
    {
		printf("\r\n close socket failed!\r\n");
	}
    return -1;

}
