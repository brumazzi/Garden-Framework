#include <sock-io.h>
#include <mem.h>
#include <unistd.h>
#include <attrib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <web-header.h>
#include <time.h>

FILE *tmp;

const char *day_name[] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

const char *month_name[] = {
	"Jan",
	"Fev",
	"Mar",
	"Apr",
	"Mai",
	"Jun",
	"Jul",
	"Ago",
	"Set",
	"Out",
	"Nov",
	"Dec"
};

const char *h_skull_OK = "HTTP/1.1 200 OK\r\n"
"Date: %s, %d %s %d:%d:%d GMT\r\n"
"Server: Garden\r\n"
"Accept-Ranges: bytes\r\n"
//"Last-Modified: \r\n"
//"ETag: %s\r\n"
"Connection: Keep-Alive\r\n"
"Content-Length: %lli\r\n"
"Content-Type: %s\r\n"
"\r\n";

const char *h_skull_not_found = "HTTP/1.1 404 Not Found\r\n"
"Date: %s, %d %s %d:%d:%d GMT\r\n"
"Server: Apache\r\n"
"Content-Length: 261\r\n"
"Connection: Keep-Alive\r\n"
"Content-Type: text/html; charset=iso-8859-1\r\n"
"\r\n"
"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
"<html><head>\n"
"<title>404 Not Found</title>\n"
"</head><body>\n"
"<h1>Not Found</h1>\n"
"<p>The requested URL /icon was not found on this server.</p>\n"
"<hr>\n"
"<address>Apache Server at localhost Port 80</address>\n"
"</body></html>\n";

int grd_recv(int fd_sock, char **recvd){
	if(!tmp){
		perror("Receive error: error to alloc receiva data");
		return GRD_RECV_ERROR;
	}

	char c;
	int k13 = 0;
	while(k13 < 4){
		if(read(fd_sock, &c, 1) != 1){
			perror("Receive error: error to receive packages");
			return GRD_RECV_ERROR;
		}
		if(c == '\r' || c == '\n')
			k13++;
		else
			k13=0;

		fwrite(&c, sizeof(char), sizeof(char), tmp);
	}

	size_t flen = ftell(tmp);
	(*recvd) = grd_alloc(flen);
	fseek(tmp, SEEK_SET, 0);

	fread(*recvd, flen, sizeof(char), tmp);

	return GRD_RECV_SUCESS;
}

int grd_recv_len(int fd_sock, char **recvd, size_t len){
	(*recvd) = grd_alloc(len);
	if(read(fd_sock, (*recvd), len) != len){
		perror("Receive error: error to receive packages");
		return GRD_RECV_ERROR;
	}

	return GRD_RECV_SUCESS;
}

int grd_send(int fd_sock, void *data, int data_type){
	if(data_type == GRD_DATA_TYPE_TEXT)
		write(fd_sock, (const char *) data, strlen((const char *)data));
	else if(data_type == GRD_DATA_TYPE_FILE)
		sendfile(fd_sock, (int) data, NULL, 4000);
	else
		return GRD_SEND_ERROR;
	return GRD_SEND_SUCESS;
}

void initialize sock_io_init(void){
	tmp = tmpfile();
}

void finalize sock_io_end(void){
	fclose(tmp);
}

int grd_callback_send(http_header *hh, const char *content){
	char header[HTTP_MAX_HEADER_SIZE];
	time_t now;

	time(&now);
	struct tm *h_time = localtime(&now);

	sprintf(header,content ? h_skull_OK : h_skull_not_found,
			day_name[h_time->tm_wday],
			h_time->tm_mday,
			month_name[h_time->tm_mon],
			h_time->tm_year,
			h_time->tm_hour,
			h_time->tm_min,
			h_time->tm_sec,
			strlen(content),
			"text/html"
			);
	grd_send(hh->fd, header, 1);
	grd_send(hh->fd, content, 1);

	return 0;
}

