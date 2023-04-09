#pragma once
#include "Common_Client.h"

class ChatClient
{
public:
	ChatClient();
	~ChatClient();

	void StartClinet();

	void SetPortNumber(UINT num)
	{
		m_PortNum = num;
	}
	void SetIPNumber(const std::string& num)

	{
		m_IPNum = num;
	}

	void SetNickname(const std::string& name)
	{
		m_NickName = name;
	}

private:
	static void RecvThreadPoint(void* param);

	UINT m_PortNum;
	std::string m_IPNum;
	std::string m_NickName;
};

