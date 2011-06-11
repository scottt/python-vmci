#include <assert.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>

void socket_startup(void)
{
	int r;
	WSADATA wsa_data;
	WORD version_requested = MAKEWORD(2, 2);


	if ((r = WSAStartup(version_requested, &wsa_data)) != 0) {
		assert(0);
	}
}

void socket_cleanup(void)
{
	int r;
	if ((r = WSACleanup()) != 0)
		assert(0);
}

void socket_close(int fd)
{
	int r;
	if ((r = closesocket(fd)) != 0) {
		fprintf(stderr, "%s: failed: %s\n", __func__, strerror(errno));
		assert(0);
	}
}
