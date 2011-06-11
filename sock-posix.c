#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void socket_startup(void)
{
}

void socket_cleanup(void)
{
}

void socket_close(int fd)
{
	int r;
	if ((r = close(fd)) != 0) {
		fprintf(stderr, "%s: failed: %s\n", __func__, strerror(errno));
		assert(0);
	}
}
