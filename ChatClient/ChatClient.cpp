#include "ChatClient.h"
#define MSG_SIZE 1024

ChatClient::ChatClient()
	: m_PortNum(24)
	, m_IPNum{}
	, m_NickName{}
{
}

ChatClient::~ChatClient()
{
}

void ChatClient::StartClinet()
{
	WSADATA wsaData = {};
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN serverAddr = { 0 }; //서버 주소

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(m_IPNum.c_str());
	serverAddr.sin_port = htons(m_PortNum);

	int re = 0;

	re = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	if (-1 == re)
	{
		return;
	}

	_beginthread(RecvThreadPoint, 0, (void*)sock);

	char msg[MSG_SIZE] = {};

	int send_length = send(sock, m_NickName.c_str(), sizeof(m_NickName), 0);
	std::cin.ignore(INT_MAX,'\n');

	while (true)
	{
		gets_s(msg, ARRAYSIZE(msg)); //블로킹 함수
		send(sock, msg, sizeof(msg), 0); //블로킹 함수

		if (0 == strcmp(msg, "system/exit"))
		{
			break;
		}
	}

	closesocket(sock);

	WSACleanup();
}

void ChatClient::RecvThreadPoint(void* param)
{
	SOCKET sock = (SOCKET) param;

	char msg[MSG_SIZE] = "";

	while (recv(sock, msg, sizeof(msg), 0))
	{
		std::cout << msg << '\n';
	}

	closesocket(sock);
}
