#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#include <vmci_sockets.h>

#include "sock-portable.h"

enum {
	BUFSIZE = 4096,
};

char *program_name;


void print_usage_exit(void)
{
	fprintf(stderr, "usage: %s SERVER_CID:PORT\n", program_name);
	exit(2);
}

int main(int argc, char **argv)
{
	struct sockaddr_vm their_addr = {0};
	int vmci_address_family;
	unsigned int server_cid, server_port;
	int sockfd = -1;
	char *p;

	program_name = basename(argv[0]);

	if (argc != 2)
		print_usage_exit();

	if (!(p = strchr(argv[1], ':'))) {
		fprintf(stderr, "%s: invalid SERVER_CID:PORT: \"%s\"\n", program_name, argv[1]);
		exit(2);
	}

	*p = '\0';
	server_cid = (unsigned int)strtoul(argv[1], &p, 0);
	assert(*p == '\0');
	server_port = (unsigned int)strtoul(p+1, &p, 0);
	assert(*p == '\0');

	socket_startup();

	if ((vmci_address_family = VMCISock_GetAFValue()) < 0) {
		fprintf(stderr, "VMCISock_GetAFValue failed: %d. You probably need root privileges\n", vmci_address_family);
		goto cleanup;
	}
	if ((sockfd = socket(vmci_address_family, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		goto cleanup;
	}

	their_addr.svm_family = vmci_address_family;
	their_addr.svm_cid = server_cid; // in native byte order, i.e. little endian for the platforms that VMWare runs on
	their_addr.svm_port = server_port;
	fprintf(stderr, "connecting to %u:%u\n", server_cid, server_port);
	if (connect(sockfd, (struct sockaddr *) &their_addr, sizeof(their_addr)) == -1) {
		perror("connect");
		goto close;
	}
	send(sockfd, "hello", 5, 0);
close:
	if (sockfd > 0)
		socket_close(sockfd);
cleanup:
	return 0;
}
