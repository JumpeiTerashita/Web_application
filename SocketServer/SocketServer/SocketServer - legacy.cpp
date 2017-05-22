// SocketServer.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#define PORT_NO 8080

int main()
{
	WSADATA wsaData;
	int nErr;

	char buf[1024];
	int nBuf;
	
	//	Initiallize

	nErr = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (nErr != 0)
	{
		WSACleanup();
		return nErr;
	}


	//	Connection

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in adr;
	adr.sin_family = AF_INET;
	adr.sin_port = htons(PORT_NO);
	adr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (adr.sin_addr.S_un.S_addr == 0xffffffff)
	{
		printf("error!!!\n");

		closesocket(sock);
		WSACleanup();
		return 1;
	}

	printf("PORT:%d\n", PORT_NO);
	printf("SOCK:%d\n", sock);

	bind(sock, (struct sockaddr *)&adr, sizeof(adr));
	listen(sock, 3);
	for (int i = 0; i < 3; i++)
	{
		struct sockaddr_in adr2;
		int nAdr2 = sizeof(adr2);

		//	Stand by
		SOCKET sockClient = accept(sock, (struct sockaddr *)&adr2, &nAdr2);
		if (sockClient == INVALID_SOCKET)
		{
			printf("INVALID_SOCKET\n");
			continue;
		}

		//	Receive
		memset(buf, 0, sizeof(buf));
		nBuf = recv(sockClient, buf, sizeof(buf), 0);

		printf("%s", buf);

		char msg[] = "HTTP/1.1 200 OK\r\n\r\n";
		nBuf = send(sockClient, msg, strlen(msg), 0);

		shutdown(sockClient, SD_BOTH);
		closesocket(sockClient);
	}

	//	End Processing

	//	TCP Socket　Release
	closesocket(sock);

	// Release
	WSACleanup();

	return 0;
}

