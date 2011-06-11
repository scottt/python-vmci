#define __USE_MINGW_ANSI_STDIO 1
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <vmci_sockets.h>

#include "sock-portable.h"

enum {
	BUFSIZE = 4096,
	CONNECTION_BACKLOG = 10,
};

int main()
{
	int listen_fd, client_fd, nfds;
	uint64_t buf_size, t;
	socklen_t size;
	struct sockaddr_vm my_addr = {0}, their_addr;
	int vmci_address_family;
	fd_set read_fds;
	unsigned int cid;
	uint8_t buf[BUFSIZE];

	socket_startup();

	if ((vmci_address_family = VMCISock_GetAFValue()) < 0) {
		fprintf(stderr, "VMCISock_GetAFValue failed: %d. You probably need root privileges\n", vmci_address_family);
		goto cleanup;
	}

	if ((listen_fd = socket(vmci_address_family, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		goto cleanup;
	}

	/*
	 * SO_VMCI_BUFFER_SIZE – Default size of communicating buffers; 65536 bytes if not set.
	 * SO_VMCI_BUFFER_MIN_SIZE – Minimum size of communicating buffers; defaults to 128 bytes.
	 * SO_VMCI_BUFFER_MAX_SIZE – Maximum size of communicating buffers; defaults to 262144 bytes.
	 */

	buf_size = 32768;
	/* reduce buffer to above size and check */
	if (setsockopt(listen_fd, vmci_address_family, SO_VMCI_BUFFER_SIZE, (void *)&buf_size, sizeof(buf_size)) == -1) {
		perror("setsockopt");
		goto close;
	}

	size = sizeof(t);
	if (getsockopt(listen_fd, vmci_address_family, SO_VMCI_BUFFER_SIZE, (void *)&t, &size) == -1) {
		perror("getsockopt");
		goto close;
	}
	if (t != buf_size) {
		fprintf(stderr, "SO_VMCI_BUFFER_SIZE not set to size requested.\n");
		goto close;
	}

	my_addr.svm_family = vmci_address_family;
	my_addr.svm_cid = VMADDR_CID_ANY;
	my_addr.svm_port = VMADDR_PORT_ANY;
	if (bind(listen_fd, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1) {
		perror("bind");
		goto close;
	}

	if ((cid = VMCISock_GetLocalCID()) == (unsigned int)-1) {
		fprintf(stderr, "VMCISock_GetLocalCID failed\n");
	} else {
		fprintf(stderr, "server cid: %u\n", cid);
	}

	size = sizeof(my_addr);
	if (getsockname(listen_fd, (struct sockaddr *)&my_addr, &size) == -1) {
		perror("getsockname");
		goto close;
	}

	fprintf(stderr, "server (cid, port): %u:%u\n", my_addr.svm_cid, my_addr.svm_port);

	for (;;) {
		if (listen(listen_fd, CONNECTION_BACKLOG) == -1) {
			perror("listen");
			goto close;
		}

		size = sizeof(their_addr);
		if ((client_fd = accept(listen_fd, (struct sockaddr *) &their_addr, &size)) == -1) {
			perror("accept");
			goto close;
		}
		fprintf(stderr, "client connected\n");

		FD_ZERO(&read_fds);
		FD_SET(client_fd, &read_fds);
		nfds = client_fd + 1;
		if (select(nfds, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			goto close;
		}
		if (FD_ISSET(client_fd, &read_fds)) {
			ssize_t s;
			if ((s = recv(client_fd, (void*)buf, sizeof(buf), 0)) < 0) {
				fprintf(stderr, "recv failed: %s\n", strerror(errno));
			} else {
				int i;
				fprintf(stderr, "recved %lld bytes\n", (long long int)s);
				for (i=0; i<s; i++) {
					putc(buf[i], stderr);
				}
				putc('\n', stderr);
			}
		}

	}
close:
	socket_close(listen_fd);
cleanup:
	socket_cleanup();
	return 0;
}
