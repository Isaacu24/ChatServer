#include "ChatClient.h"

int main()
{
	ChatClient client = ChatClient();

	std::cout << "������ ���� ��Ʈ ��ȣ�� �����ּ���." << std::endl;
	UINT port = 0;
	std::cin >> port;

	client.SetPortNumber(24);

	std::cout << "������ ���� IP �ּҸ� �����ּ���." << std::endl;
	std::string iP = "";
	std::cin >> iP;

	client.SetIPNumber(iP);

	std::cout << "����� �г����� �����ּ���." << std::endl;
	std::string nickname = "";
	std::cin >> nickname;

	client.SetNickname(nickname);

	client.StartClinet();

	return 0;
}