#ifndef  __WEB-HEADER_H__
#define  __WEB-HEADER_H__

#include <limits.h>

#ifndef HTTP_MAX_HEADER_SIZE
#	define HTTP_MAX_HEADER_SIZE (80*1024);
#endif

enum http_method{
	DELETE,
	GET,
	HEAD,
	POST,
	PUT,
};

typedef struct{
	char field[512];
	void *value;
}http_data;

typedef struct{
	http_method method;
	short http_version;
	char uri[PATH_MAX];
	http_data *h_receive;
	http_data *h_form;
	int fd;
}http_header;

enum CONTENT_TYPE{
	CONTENT_HTML,
	CONTENT_PLAIN,
	CONTENT_O_STREAM,
	CONTENT_PFD,
	CONTENT_ALL
};

const char *content_type[] = {
	"text/html",
	"text/plain",
	"application/octet-stream",
	"application/pdf",
	"*/*",
};

int grd_callback_send(http_header *hh, const char *content);
int grd_header_parser(http_header *hh, const char *s_header);

#endif  /*WEB-HEADER_H*/
