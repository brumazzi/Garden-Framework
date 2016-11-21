#include <sock-io.h>
#include <mem.h>
#include <unistd.h>
#include <attrib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/socket.h>

FILE *tmp;

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
