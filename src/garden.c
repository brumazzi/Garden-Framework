#include <connect.h>
#include <sock-io.h>
#include <stdio.h>

int main(int argc, char **argv){
#ifdef DEBUG
	/*
	 * this code up the server, send and receive a message,
	 * run on debug mode to test the framework.
	 */
	int fd_server = grd_server(8001);
	char *data = 0;

	int fd_client = grd_accept(fd_server);
	grd_send(fd_client, "This is a message\n", 1);
	grd_recv(fd_client, &data);
	grd_close(fd_client);
	grd_close(fd_server);

	printf("%s\n", data);
#endif

	return 0;
}
