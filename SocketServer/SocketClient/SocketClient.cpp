// SocketServer.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#define PORT_NO 8080

typedef struct
{
	char message[200];
	int choice0_id;
	char choice0[200];
	int choice1_id;
	char choice1[200];
} MESSAGE_DATA;


int main()
{
	WSADATA wsaData;
	int nErr;

	char ipBuf[256];
	char strInput[256];

	int nBuf;

	//	Initiallize

	nErr = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (nErr != 0)
	{
		WSACleanup();
		return nErr;
	}

	//	GET	Host IP
	printf("IPを入力してください:");
	gets_s(ipBuf, sizeof(ipBuf));


	//	Connection

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in adr;
	adr.sin_family = AF_INET;
	adr.sin_port = htons(PORT_NO);
	inet_pton(AF_INET, ipBuf, &(adr.sin_addr.S_un.S_addr));

	if (connect(sock, (struct sockaddr *)&adr, sizeof(adr)) == SOCKET_ERROR)
	{
		printf("error!!!\n");

		closesocket(sock);
		WSACleanup();
		return 1;
	}

	printf("PORT:%d\n", PORT_NO);
	printf("SOCK:%d\n", sock);

	BOOL bLoop = TRUE;
	strncpy_s(strInput, sizeof(strInput), "0", _TRUNCATE);

	while (bLoop)
	{
		//	Send
		nBuf = send(sock, strInput, (int)strlen(strInput) + 1, 0);
		if (nBuf == SOCKET_ERROR)
		{
			int nErrorCode = WSAGetLastError();
			printf("error:%d", nErrorCode);
			break;
		}
		printf("send > %s\n", strInput);

		//	Receive
		MESSAGE_DATA data;
		memset(&data, 0, sizeof(data));
		nBuf = recv(sock, (char *)&data, sizeof(data), 0);
		if (nBuf == SOCKET_ERROR)
		{
			printf("receive ERROR");
			break;
		}
		printf("%s\n", data.message);
		if (data.choice0_id == 255) break;

		printf("%d:%s\n", data.choice0_id, data.choice0);
		printf("%d:%s\n", data.choice1_id, data.choice1);

		//	Input
		gets_s(strInput, sizeof(strInput));

	}
	shutdown(sock, SD_BOTH);	
	
	//	End Processing

	//	TCP Socket　Release
	closesocket(sock);

	// Release
	WSACleanup();

	return 0;
}

