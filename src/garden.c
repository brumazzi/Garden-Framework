#include <connect.h>
#include <sock-io.h>
#include <stdio.h>
#include <attrib.h>
#include <view.h>

#ifdef DEBUG
const char *header = "GET / HTTP/1.1\r\n"
"Host: 127.0.0.1:8081\r\n"
"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0\r\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
"Accept-Language: pt-BR,pt;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"DNT: 1\r\n"
"Connection: keep-alive\r\n"
"Upgrade-Insecure-Requests: 1\r\n\r\n";
#endif

int main(int argc, char **argv){
	debug{
		/*
		 * this code up the server, send and receive a message,
		 * run on debug mode to test the framework.
		 */
		/*int port = 8001;
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
		grd_close(fd_server);*/

		//printf("%s\n", data);
		http_header hh;
		grd_header_parser(&hh, header);
		for(int x=0; x<8; x++){
			printf("%s: %s\n", hh.h_receive[x].field, hh.h_receive[x].value);
		}
		
		puts("here");
	}

	return 0;
}
