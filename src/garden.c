#include <connect.h>
#include <sock-io.h>
#include <stdio.h>
#include <attrib.h>
#include <view.h>

int main(int argc, char **argv){
	debug{
		/*
		 * this code up the server, send and receive a message,
		 * run on debug mode to test the framework.
		 */
		int port = 8001;
		int fd_server = grd_server(port);
		void *view = grd_views_load("app");
		char *data = 0;

		printf("Garden connect in port %i\n", port);

		int fd_client = grd_accept(fd_server);
		grd_recv(fd_client, &data);
		data[strlen(data)-4] = 0;

		int res = grd_view_open(view, data, "header", fd_client);
		printf("%s\n", res?"sucess":"page not found");

		grd_views_close(view);
		grd_close(fd_client);
		grd_close(fd_server);

		printf("%s\n", data);
	}

	return 0;
}
