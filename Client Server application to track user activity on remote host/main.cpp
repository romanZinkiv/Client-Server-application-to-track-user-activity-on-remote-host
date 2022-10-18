#include <iostream>

//#define Linux

#ifdef Linux
#include "Linux.h"
#else
#include "Windows.h"
#endif

std::string ipAddress = "127.0.0.1";	//localhost
int port = 5577;

int main() {
#ifdef Linux
	startLinux(ipAddress.c_str(), port);
#else
	startWindows(ipAddress, port);
#endif

	return 0;
}
