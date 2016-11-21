#ifndef  __CONNECT_H__
#define  __CONNECT_H__

#define GRD_ERROR_SOCKET	-1
#define GRD_ERROR_BIND		-2
#define GRD_ERROR_LISTEN	-3
#define GRD_ERROR_ACCEPT	-1

int grd_server(unsigned int port);
int grd_accept(int fd_server);
void grd_close(int fd);

#endif  /*CONNECT_H*/
