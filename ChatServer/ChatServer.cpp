#include "ChatServer.h"
#include "Common_Server.h"
#include <thread>

#define MSG_SIZE 1024

ChatServer::ChatServer()
	: count(0)
	, isAccept(false)
{
}

ChatServer::~ChatServer()
{
}

void ChatServer::StartServer()
{
	WSADATA wsaData = {};

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET sock = SetTCPServer(m_PortNum, m_Blog);

	if (-1 == sock)
	{
		perror("대기 소켓 오류");
	}

	else
	{
		EventLoop(sock);
	}

	//AcceptLoop(sock);
	//closesocket(sock);

	WSACleanup();
}


SOCKET ChatServer::SetTCPServer(short portNum, int blog)
{
	SOCKET sock = {}; //대기 소켓
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN serverAddr = { 0 };

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr = GetDefaultMyIP();
	serverAddr.sin_port = htons(portNum);

	std::cout << "서버 포트: " << portNum << " 서버 아이피: " << inet_ntoa(serverAddr.sin_addr) << std::endl;

	int ret = 0;

	ret = bind(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	if (-1 == ret)
	{
		return -1;
	}

	ret = listen(sock, blog);

	if (-1 == ret)
	{
		return -1;
	}

	return sock;
}

IN_ADDR ChatServer::GetDefaultMyIP()
{
	//호스트 이름 얻기
	char locaHostName[MAX_PATH];
	IN_ADDR addr = { 0, }; //주소를 얻기 위한 변수 초기화

	if (SOCKET_ERROR == gethostname(locaHostName, MAX_PATH))
	{
		return addr;
	}

	//호스트 이름으로 호스트 엔트리를 구한다
	HOSTENT* hPtr = gethostbyname(locaHostName);

	while (hPtr && hPtr->h_name)
	{
		//호스트 엔트리 목록에서 PF_INET(IP4) 타입을 찾는다
		if (PF_INET == hPtr->h_addrtype)
		{
			memcpy(&addr, hPtr->h_addr_list[0], hPtr->h_length);
			break;
		}

		++hPtr;
	}

	return addr;

}

void ChatServer::EventLoop(SOCKET sock)
{
	AddNetworkEvent(sock, FD_ACCEPT);

	while (true)
	{
		int index = WSAWaitForMultipleEvents(count, handles, false, INFINITE, false);

		WSANETWORKEVENTS event;
		WSAEnumNetworkEvents(socks[index], handles[index], &event);

		switch (event.lNetworkEvents)
		{
		case FD_ACCEPT:
			AcceptProc(index);
			break;
		case FD_READ:
			ReadProc(index);
			break;
		case FD_CLOSE:
			CloseProc(index);
			break;
		}
	}

	closesocket(sock);
}


HANDLE ChatServer::AddNetworkEvent(SOCKET sock, long event)
{
	HANDLE handle = WSACreateEvent();
	socks[count] = sock;
	handles[count] = handle;

	++count;

	WSAEventSelect(sock, handle, event);

	return handle;
}

void ChatServer::AcceptProc(int index)
{
	SOCKADDR_IN cliAddr = {0};
	int len = sizeof(cliAddr);

	SOCKET doSock = accept(socks[0], (SOCKADDR*)&cliAddr, &len);

	if (FD_SETSIZE == count)
	{
		std::cout << "채팅 서버 인원 초과!" << std::endl;
		closesocket(doSock);
		return;
	}

	AddNetworkEvent(doSock, FD_READ | FD_CLOSE);

	isAccept = true;
}

void ChatServer::ReadProc(int index)
{
	char msg[MSG_SIZE] = "";
	recv(socks[index], msg, sizeof(msg), 0);

	if (true == isAccept)
	{
		isAccept = false;

		nicknames[index] = msg;
		std::cout << nicknames[index] + " 채팅 서버 입장" << std::endl;

		return;
	}

	SOCKADDR_IN cliAddr = { 0 };
	int len = sizeof(cliAddr);

	getpeername(socks[index], (SOCKADDR*)&cliAddr, &len);

	std::string str = "";

	for (int i = 0; i < count; ++i)
	{
		if (index == i)
		{
			str = nicknames[i] + " 님: " + msg;
		}

		else
		{
			str = nicknames[index] + " 님: " + msg;
		}

		send(socks[i], str.c_str(), sizeof(str), 0);
	}
}

void ChatServer::CloseProc(int index)
{
	SOCKADDR_IN cliAddr = { 0 };
	int len = sizeof(cliAddr);

	getpeername(socks[index], (SOCKADDR*)&cliAddr, &len);

	std::cout << nicknames[index] + " 접속 종료" << std::endl;

	std::string str = nicknames[index] + " 님이 나갔습니다";

	closesocket(socks[index]);
	WSACloseEvent(handles[index]);

	--count;

	socks[index] = socks[count];
	handles[index] = handles[count];
	nicknames[index] = nicknames[count];

	for (int i = 0; i < count; ++i)
	{
		send(socks[i], str.c_str(), sizeof(str), 0);
	}
}




//void ChatServer::AcceptLoop(SOCKET sock)
//{
//	SOCKET doSock;
//	SOCKADDR_IN cliAddr = { 0 };
//
//	int len = sizeof(cliAddr);
//
//	while (true)
//	{
//		doSock = accept(sock, (SOCKADDR*)&cliAddr, &len);
//
//		if (-1 == doSock)
//		{
//			perror("Accept 실패");
//			break;
//		}
//
//		std::cout << inet_ntoa(cliAddr.sin_addr) << std::endl <<
//			ntohs(cliAddr.sin_port) << " 연결 요청 수락" << std::endl;
//
//		_beginthread(DoIt, 0, (void*)doSock);
//	}
//}

//void ChatServer::DoIt(void* param)
//{
//	SOCKET doSock = (SOCKET)param;
//
//	SOCKADDR_IN cliAddr = { 0 };
//	int len = sizeof(cliAddr);
//	getpeername(doSock, (SOCKADDR*)&cliAddr, &len);
//
//	char msg[MSG_SIZE] = "";
//	
//	while (0 < recv(doSock, msg, sizeof(msg), 0))
//	{
//		std::cout << inet_ntoa(cliAddr.sin_addr) << ": " << msg << std::endl;
//		send(doSock, msg, sizeof(msg), 0);
//	}
//
//	closesocket(doSock);
//}
