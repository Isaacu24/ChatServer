#include "ChatClient.h"

int main()
{
	ChatClient client = ChatClient();

	std::cout << "접속할 서버 포트 번호를 적어주세요." << std::endl;
	UINT port = 0;
	std::cin >> port;

	client.SetPortNumber(24);

	std::cout << "접속할 서버 IP 주소를 적어주세요." << std::endl;
	std::string iP = "";
	std::cin >> iP;

	client.SetIPNumber(iP);

	std::cout << "사용할 닉네임을 적어주세요." << std::endl;
	std::string nickname = "";
	std::cin >> nickname;

	client.SetNickname(nickname);

	client.StartClinet();

	return 0;
}