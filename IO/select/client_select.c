/*******************************************
@Author: guangjung
@Date:   2017/07/20
********************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h> //for htons
#include <unistd.h>  //for close

#include "common.h"

int SendMsg(int fd, char *pcMsg)
{
	MSG_ST stSendMsg;

	memset(&stSendMsg, 0, MSG_LEN_MAX);
	stSendMsg.stMsgHead.iType = MSG_TYPE_REQUEST;
	
	snprintf(stSendMsg.stMsgHead.acName, sizeof(stSendMsg.stMsgHead.acName),
		"%s", "client");
	
	snprintf(stSendMsg.stMsgBody.acBody, sizeof(stSendMsg.stMsgBody.acBody),
		"%s", pcMsg);

	stSendMsg.stMsgHead.uiMsgBodyLen = strlen(stSendMsg.stMsgBody.acBody);
	
	if (-1 == send(fd, &stSendMsg, MSG_LEN_MAX, 0))
	{
		perror("\r\n send failed!\r\n");
	}

	return 0;
	
}

int RecvMsg(int fd)
{
	MSG_ST stRecvMsg;

	memset(&stRecvMsg, 0, MSG_LEN_MAX);
	
	if (-1 == recv(fd, &stRecvMsg, MSG_LEN_MAX, 0))
	{
		printf("\r\n recv failed!\r\n");
		return -1;
	}

	printf("\r\n Receive:");
	printf("\r\n\t TYPE: %d", stRecvMsg.stMsgHead.iType);
	printf("\r\n\t LEN: %d", stRecvMsg.stMsgHead.uiMsgBodyLen);
	printf("\r\n\t Name: %s", stRecvMsg.stMsgHead.acName);
	printf("\r\n\t Body: %s", stRecvMsg.stMsgBody.acBody);
	
	printf("\r\n***************************\r\n");
	return 0;
	
}

int main(int argc, char **argv)
{
	int fd;
	//int result;
	struct sockaddr_in sin;
	socklen_t addrlen;

	if (argc <= 1)
	{
		printf("\r\n parameters are less than 1. \r\n");
	    return -1;
	}
	
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;  //使用IPV4 通信域
	//sin.sin_addr.s_addr =INADDR_ANY; //服务器可以接受任意地址
	inet_pton(AF_INET, SEVER_IP_ADDR, &sin.sin_addr);
	sin.sin_port = htons(SEVER_PORT); //端口号转化为网络字节序

	//create socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == fd)
	{
		printf("\r\n line[%d]create socket failed!\r\n", __LINE__);
		return -1;
	}

    if (-1 == connect(fd, (struct sockaddr *) &sin, sizeof(sin)))
    {
		printf("\r\n line[%d]connectsocket failed!\r\n", __LINE__);
		goto ERR_LABEL;
	}

	if (0 == SendMsg(fd, argv[1]))
	{
		if (0 != RecvMsg(fd))
		{
			printf("\r\n recv failed.\r\n");
		}
		//return 0;
	}
	else
	{
		printf("\r\n send failed.\r\n");
	}
	
ERR_LABEL:

    if (-1 == close(fd))
    {
		printf("\r\n close socket failed!\r\n");
	}
    return 0;

}

