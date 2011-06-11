#ifndef MYSOCKET_H
#define MYSOCKET_H

#ifdef __WIN32__
#include <winsock2.h>
typedef int socklen_t;
#else
#include <sys/select.h>
#include <sys/socket.h>
#endif

void socket_startup(void);
void socket_cleanup(void);
void socket_close(int);

#endif
