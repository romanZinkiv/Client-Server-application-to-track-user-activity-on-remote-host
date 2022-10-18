#include "Windows.h"

void startWindows(string ipAddress, int port)
{
	initWinSock();
	SOCKET sock = createSocket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in hint = fillStructure(AF_INET, ipAddress, port);
	connectServer(sock, hint);
	startSurvey(sock);

	closesocket(sock);
	WSACleanup();
}

void initWinSock()
{
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		exit(1);
	}
}

SOCKET createSocket(int af, int type, int protocol)
{
	SOCKET sock = socket(af, type, protocol);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		exit(1);
	}
	return sock;
}

sockaddr_in fillStructure(int af, string ipAddress, int port)
{
	sockaddr_in hint;
	hint.sin_family = af;
	hint.sin_port = htons(port);
	inet_pton(af, ipAddress.c_str()/*const char * */, &hint.sin_addr);
	return hint;
}

void connectServer(int sock, sockaddr_in hint)
{
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		exit(1);
	}
}

void startSurvey(int sock)
{
	POINT p;
	GetCursorPos(&p);
	int pSavedX = p.x, pSavedY = p.y;
	string startingMessage = "Started sending mouse data:\n";
	int sendMessage = send(sock, startingMessage.c_str(), startingMessage.size() + 1, 0);
	if (sendMessage == SOCKET_ERROR)
	{
		cerr << "Can't send data to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		exit(1);
	}

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));//60000
		GetCursorPos(&p);
		if (pSavedX != p.x || pSavedY != p.y)
		{
			//  "Active"
			int sendMessage = send(sock, "Active\n", 8, 0);
			pSavedX = p.x, pSavedY = p.y;
			cout << "p.x = " << p.x << "	p.y = " << p.y << endl;
			if (sendMessage == SOCKET_ERROR)
			{
				cerr << "Can't send data to server, Err #" << WSAGetLastError() << endl;
				closesocket(sock);
				WSACleanup();
				exit(1);
			}
		}
		else
		{
			//  "Passive"
			int sendMessage = send(sock, "Passive\n", 9, 0);
			pSavedX = p.x, pSavedY = p.y;
			cout << "p.x = " << p.x << "	p.y = " << p.y << endl;
			if (sendMessage == SOCKET_ERROR)
			{
				cerr << "Can't send data to server, Err #" << WSAGetLastError() << endl;
				closesocket(sock);
				WSACleanup();
				exit(1);
			}
		}
	}
}
