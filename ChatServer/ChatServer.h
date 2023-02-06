#pragma once
#include "Common_Server.h"

class ChatServer
{
public:
	ChatServer();
	~ChatServer();

	void StartServer();

	void SetPortNumber(UINT num)
	{
		m_PortNum = num;
	}

	void SetBlogSize(UINT size)
	{
		m_Blog = size;
	}

private:
	//void AcceptLoop(SOCKET sock);
	//static void DoIt(void* param);

	SOCKET SetTCPServer(short portNum, int blog);
	IN_ADDR GetDefaultMyIP();

	void EventLoop(SOCKET sock);

	HANDLE AddNetworkEvent(SOCKET sock, long event);

	void AcceptProc(int index);
	void ReadProc(int index);
	void CloseProc(int index);

private:
	UINT m_PortNum;
	UINT m_Blog;

	SOCKET socks[FD_SETSIZE];
	HANDLE handles[FD_SETSIZE];
	std::string nicknames[FD_SETSIZE];

	int count;

	bool isAccept;
};

