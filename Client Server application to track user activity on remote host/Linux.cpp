#include "Linux.h"

void startLinux(const char* ipAddress, int port)
{
	int sock = createSocket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in hint = fillStructure(AF_INET, ipAddress, port);
	connectServer(sock, hint);
	startSurvey(sock);

	if (shutdown(sock, SHUT_RDWR) < 0) {
		// error in shutting down
		exit(1);
	}

	close(sock);
}

int createSocket(int af, int type, int protocol)
{
	int sock = socket(af, type, protocol);
	if (sock < 0) {
		// error in socket creation
		// exit
		exit(1);
	}
	return sock;
}

struct sockaddr_in fillStructure(int af, const char* ipAddress, int port)
{
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof sa);
	sa.sin_family = af;
	sa.sin_port = htons(port);
	inet_pton(af, ipAddress, &sa.sin_addr);
	return sa;
}

void connectServer(int sock, sockaddr_in hint)
{
	if (connect(sock, (struct sockaddr*)&hint, sizeof hint) == -1) {
		int err = errno;
		printf("connect: strerror=%d: %s \n", err, strerror(err));
		// exit(1);
		exit(1);
	}
}

void startSurvey(int sock)
{
	Display* d;
	Window inwin;      /* root window the pointer is in */
	Window inchildwin; /* child win the pointer is in */
	int rootx, rooty; /* relative to the "root" window; we are not interested in these,
						 but we can't pass NULL */
	int childx, childy;  /* the values we are interested in */
	Atom atom_type_prop; /* not interested */
	int actual_format;   /* should be 32 after the call */
	unsigned int mask;   /* status of the buttons */
	unsigned long n_items, bytes_after_ret;
	Window* props; /* since we are interested just in the first value, which is
			  a Window id */

			  /* default DISPLAY */
	d = XOpenDisplay(NULL);

	/* ask for active window (no error check); the client must be freedesktop
	   compliant */
	(void)XGetWindowProperty(d, DefaultRootWindow(d),
		XInternAtom(d, "_NET_ACTIVE_WINDOW", True),
		0, 1, False, AnyPropertyType,
		&atom_type_prop, &actual_format,
		&n_items, &bytes_after_ret, (unsigned char**)&props);

	XQueryPointer(d, props[0], &inwin, &inchildwin,
		&rootx, &rooty, &childx, &childy, &mask);
	int pSavedX = childx, pSavedY = childy;
	char startingMessage[] = "Started sending mouse data:\n";
	int sendMessage = send(sock, startingMessage, 29, 0);
	if (sendMessage == -1)
	{
		int err = errno;
		printf("Can't send data to server, Err=%d: %s \n", err, strerror(err));
		// exit(1);
		exit(1);
	}

	while (true)
	{
		sleep(5);//60
		XQueryPointer(d, props[0], &inwin, &inchildwin,
			&rootx, &rooty, &childx, &childy, &mask);
		printf("relative to active window: %d,%d\n", childx, childy);
		if (pSavedX != childx || pSavedY != childy)
		{
			//  "Active"
			int sendMessage = send(sock, "Active\n", 8, 0);
			pSavedX = childx, pSavedY = childy;
			//printf("relative to active window: %d,%d\n", childx, childy);
			if (sendMessage == -1)
			{
				int err = errno;
				printf("Can't send data to server, Err=%d: %s \n", err, strerror(err));
				// exit(1);
				exit(1);
			}
		}
		else
		{
			//  "Passive"
			int sendMessage = send(sock, "Passive\n", 9, 0);
			pSavedX = childx, pSavedY = childy;
			//printf("relative to active window: %d,%d\n", childx, childy);
			if (sendMessage == -1)
			{
				int err = errno;
				printf("Can't send data to server, Err=%d: %s \n", err, strerror(err));
				// exit(1);
				exit(1);
			}
		}
	}

	XFree(props);           /* free mem */
	(void)XCloseDisplay(d); /* and close the display */
}
