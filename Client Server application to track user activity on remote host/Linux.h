#pragma once
#include <arpa/inet.h> //inet_pton
#include <cstring>     //memset
#include <errno.h>		//strerror
#include <stdio.h>	//printf
#include <sys/socket.h>
#include <unistd.h> //close
#include <stdlib.h>	//exit

void startLinux(const char* ipAddr, int p);
int createSocket(int af, int type, int protocol);
struct sockaddr_in fillStructure(int af, const char* ipAddress, int port);
void connectServer(int sock, struct sockaddr_in hint);
void startSurvey(int sock);
