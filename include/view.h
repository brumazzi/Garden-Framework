#ifndef  __VIEW_H__
#define  __VIEW_H__

#define GRD_VIEW_LOAD_ERROR	0
#define GRD_VIEW_OPEN_ERROR	1
#define GRD_VIEW_OPEN_SUCESS	0

void *grd_views_load(const char *lib_view);
void grd_views_close(void *lib_view);
int grd_view_open(void *lib_view, const char *url, const char *header, int fd_client);

#endif  /*VIEW_H*/
