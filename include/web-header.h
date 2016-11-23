#ifndef  __WEB_HEADER_H__
#define  __WEB_HEADER_H__

#include <limits.h>

#ifndef HTTP_MAX_HEADER_SIZE
#	define HTTP_MAX_HEADER_SIZE (80*1024)
#endif

#define CONTENT_HTML		"text/html"
#define CONTENT_PLAIN		"text/plain"
#define CONTENT_APP_STREAM	"application/octet-stream"
#define CONTENT_PDF		"application/pdf"
#define CONTENT_ACCEPT_ALL	"*/*"

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
	enum http_method method;
	short http_version;
	char uri[PATH_MAX];
	http_data *h_receive;
	http_data *h_form;
	int fd;
}http_header;

int grd_header_parser(http_header *hh, const char *s_header);

#endif  /*WEB_HEADER_H*/
