#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <connect.h>
#include <mem.h>

int grd_server(unsigned int port){
	struct sockaddr_in server;
	int fd_server;
	int on = 1;

	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0){
		perror("Server error: Fail to prepare socket.");
		return GRD_ERROR_SOCKET;
	}

	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	int bind_serv = bind(fd_server, (struct sockaddr*) &server, sizeof(server));
	if(bind_serv < 0){
		perror("Server error: Fail to bind server.");
		close(fd_server);
		return GRD_ERROR_BIND;
	}

	int list_serv = listen(fd_server, 0);
	if(list_serv < 0){
		perror("Server error: Fail to start server.");
		close(fd_server);
		return GRD_ERROR_LISTEN;
	}

	return fd_server;
}

int grd_accept(int fd_server){
	struct sockaddr_in client;
	int fd_client;
	socklen_t cli_len = sizeof(client);

	fd_client = accept(fd_server, (struct sockaddr *) &client, &cli_len);
	if(fd_client < 0){
		perror("Server error: Can't be accept clients");
		return GRD_ERROR_ACCEPT;
	}

	return fd_client;
}

void grd_close(int fd){
	close(fd);
}
