#ifndef  __SOCK_IO_H__
#define  __SOCK_IO_H__

#define GRD_DATA_TYPE_TEXT	1
#define GRD_DATA_TYPE_FILE	2

#define GRD_RECV_ERROR	1
#define GRD_RECV_SUCESS	0

#define GRD_SEND_ERROR	1
#define GRD_SEND_SUCESS	0

int grd_recv(int fd_sock, char **recvd);
int grd_send(int fd_sock, void *data, int data_type);

#endif  /*SOCK-IO_H*/
