#include <web-header.h>
#include <time.h>
#include <sock-io.h>
#include <stdio.h>
#include <string.h>
#include <mem.h>
#include <attrib.h>
#include <stdlib.h>

void grd_header_init(http_header *hh, int fd){
	bzero(hh, sizeof(http_header));
	hh->fd = fd;
}

void grd_header_destroy(http_header *hh){
	int x = (int) hh->header_len;
	if(x){
		while((x)--)
			grd_free(hh->h_receive[x].value);
		grd_free(hh->h_receive);
	}

	x = (int) hh->form_len;
	if(x){
		while((x)--)
			grd_free(hh->h_form[x].value);
		grd_free(hh->h_form);
	}
}

int grd_header_parser(http_header *hh, const char *s_header){
	unsigned int cur = 0;
	char buff[PATH_MAX];

	sscanf(s_header, "%[^\r\n]", buff);
	char method[8];
	char uri[PATH_MAX];
	char http_v[16];
	sscanf(buff, "%8[^\ ] %[^\ ] %16[^\r\n]", method, uri, http_v);
	strcpy(hh->uri, uri);

	if(!strcmp(method, "GET")){
		hh->method = GET;
	}else if(!strcmp(method, "POST")){
		hh->method = POST;
	}else if(!strcmp(method, "PUT")){
		hh->method = PUT;
	}else if(!strcmp(method, "DELETE")){
		hh->method = DELETE;
	}else if(!strcmp(method, "HEAD")){
		hh->method = HEAD;
	}else{
		return -1;
	}

	cur = strlen(buff)+1;
	int max_header = 0;
	for(int x=0; s_header[x]; x++){
		if(s_header[x] == '\n')
			max_header++;
	}
	hh->header_len = max_header;
	hh->form_len = 0;
	
	hh->h_receive = grd_alloc(max_header);
	for(int x=0; x<max_header; x++){
		sscanf((char *)(s_header+cur), "\n%[^\r\n]", buff);
		int buff_len = strlen(buff);
		hh->h_receive[x].value = grd_alloc(buff_len);
		sscanf(buff, "%[^:]: %[^\r\n]",
				hh->h_receive[x].field,
				hh->h_receive[x].value
		      );
		cur += buff_len+2;
	}

	int fcur = 0;
	while(hh->uri[fcur]){
		if(hh->uri[fcur] == '?'){
			fcur++;
			int vars = 1;
			int inc = 0;
			while(hh->uri[fcur+inc]){
				if(hh->uri[fcur+inc] == '&')
					vars++;
				inc++;
			}
			hh->form_len = vars;
			hh->h_form = grd_alloc(vars);
			for(int x=0; x<vars; x++){
				sscanf((const char *)hh->uri+fcur,
						"%[^&]", buff);
				size_t fv_len = strlen(buff);
				hh->h_form[x].value = grd_alloc(fv_len);

				sscanf(buff, "%512[^=]=%[^&]", hh->h_form[x].field, hh->h_form[x].value);
				cur += strlen(buff)+1;
			}
		}
		fcur++;
	}
	if(hh->method != GET){
		const char *cont_len = grd_header_propert(hh, "Content-Length");
		size_t len;
		len = atoi(cont_len)+1;
		
		char *form = s_header+cur-len;

		cur = 0;
		while(form[cur]){
			int vars = 1;
			int inc = 0;
			while(form[cur+inc]){
				if(form[cur+inc] == '&')
					vars++;
				inc++;
			}
			hh->form_len = vars;
			hh->h_form = grd_alloc(vars);
			for(int x=0; x<vars; x++){
				bzero(buff, PATH_MAX);
				sscanf((const char *)form+cur, "%[^&]", buff);
				size_t fv_len = strlen(buff);
				hh->h_form[x].value = grd_alloc(fv_len);
					
				sscanf(buff, "%512[^=]=%[^&]", hh->h_form[x].field, hh->h_form[x].value);
				cur += strlen(buff)+1;
			}
			cur++;
		}
	}
}

const char *grd_header_propert(http_header *hh, const char *prop){
	for(int x=0; x<hh->header_len; x++){
		if(!strcmp(prop, hh->h_receive[x].field)){
			return hh->h_receive[x].value;
		}
	}

	return NULL;
}

const char *grd_form_value(http_header *hh, const char *var){
	for(int x=0; x<hh->form_len; x++){
		if(!strcmp(var, hh->h_form[x].field)){
			return hh->h_form[x].value;
		}
	}

	return NULL;
}

const char **grd_form_list_vars(http_header *hh){
	char **res = grd_alloc(hh->form_len);
	for(int x=0; x<hh->form_len; x++){
		res[x] = hh->h_form[x].field;
	}

	return (const char **)res;
}
