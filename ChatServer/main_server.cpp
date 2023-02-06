#include "ChatServer.h"

int main()
{
	ChatServer server = ChatServer();

	server.SetPortNumber(24);
	server.SetBlogSize(5);
	server.StartServer();

	return 0;
}
