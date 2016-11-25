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
#define CONTENT_FORM		"application/x-www-form-urlencode"
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
	short header_len;
	short form_len;
	http_data *h_receive;
	http_data *h_form;
	int fd;
}http_header;

void grd_header_init(http_header *hh, int fd);
int grd_header_parser(http_header *hh, const char *s_header);
const char *grd_header_propert(http_header *hh, const char *prop);
const char **grd_form_list_vars(http_header *hh);
const char *grd_form_value(http_header *hh, const char *var);

#endif  /*WEB_HEADER_H*/
