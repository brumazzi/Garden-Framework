#include <view.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <sock-io.h>
#include <mem.h>
#include <web-header.h>

int grd_view_open(http_header *hh){
	char lib_path[PATH_MAX];
	getcwd(lib_path, PATH_MAX);
	strcat(lib_path, "/views.so");
	void *lib_view = dlopen(lib_path, RTLD_NOW);

	void (*view)(http_header *hh, int (*resp)(http_header *, const char *));
	char uri[PATH_MAX];

	sscanf(hh->uri,"%[^?]", uri);
	
	int x = 0;
	for(x=0; x<strlen(uri); x++){
		if(uri[x] == '/')
			uri[x] = '_';
	}
	view = dlsym(lib_view, uri);

	if(!view){
		fprintf(stderr, "Invalid url: %[^?]", hh->uri);
		return GRD_VIEW_OPEN_ERROR;
	}

	view(hh, grd_callback_send);

	dlclose(lib_view);

	return GRD_VIEW_OPEN_SUCESS;
}
