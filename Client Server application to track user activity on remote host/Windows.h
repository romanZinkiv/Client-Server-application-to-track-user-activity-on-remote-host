#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

void startWindows(string ipAddr, int p);
void initWinSock();
SOCKET createSocket(int af, int type, int protocol);
sockaddr_in fillStructure(int af, string ipAddress, int port);
void connectServer(int sock, sockaddr_in hint);
void startSurvey(int sock);
