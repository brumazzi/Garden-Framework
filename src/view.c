#include <view.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <sock-io.h>
#include <mem.h>

void *grd_views_load(const char *lib_view){
	char lib_path[PATH_MAX];
	getcwd(lib_path, PATH_MAX);
	strcat(lib_path, "/views/");
	strcat(lib_path, lib_view);
	strcat(lib_path, ".so");

	void *handle = dlopen(lib_path, RTLD_NOW);

	return handle;
}

void grd_views_close(void *lib_view){
	dlclose(lib_view);
}

int grd_view_open(void *lib_view, const char *url, const char *header, int fd_client){
	void (*view)(int cli_recv, int (*resp)(int, void *, int));

	view = dlsym(lib_view, "index");
	/*if(!strcmp(url, "/"))
		view = dlsym(lib_view, "index");
	else{
		int x = 0;
		char *buff = grd_alloc(strlen(url));
		strcpy(buff, url);
		while(buff[x])
			if(buff[x] == '/')
				buff[x] = '_';
		view = dlsym(lib_view, buff);
	}*/

	if(!view){
		fprintf(stderr, "Invalid url: %s", url);
		return GRD_VIEW_OPEN_ERROR;
	}

	view(fd_client, grd_send);

	return GRD_VIEW_OPEN_SUCESS;
}
