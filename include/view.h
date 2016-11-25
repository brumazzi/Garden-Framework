#ifndef  __VIEW_H__
#define  __VIEW_H__

#define GRD_VIEW_LOAD_ERROR	0
#define GRD_VIEW_OPEN_ERROR	1
#define GRD_VIEW_OPEN_SUCESS	0

#include <web-header.h>

int grd_view_open(http_header *hh);

#endif  /*VIEW_H*/
