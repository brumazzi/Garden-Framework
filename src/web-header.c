#include <web-header.h>
#include <time.h>
#include <sock-io.h>
#include <stdio.h>
#include <string.h>
#include <mem.h>
#include <attrib.h>

int grd_header_parser(http_header *hh, const char *s_header){
	unsigned int cur = 0;
	const char buff[PATH_MAX];

	sscanf(s_header, "%[^\r\n]", buff);
	char method[8];
	char uri[PATH_MAX];
	char http_v[16];
	sscanf(buff, "%8[^ ] %[^ ] %16[^\r\n]", method, uri, http_v);
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
	int max_header = -1;
	for(int x=0; s_header[x]; x++){
		if(s_header[x] == '\n')
			max_header++;
	}
	
	hh->h_receive = grd_alloc(max_header);
	for(int x=0; x<max_header; x++){
		sscanf((const char *)(s_header+cur), "\n%[^\r\n]", buff);
		int buff_len = strlen(buff);
		hh->h_receive[x].value = grd_alloc(buff_len);
		sscanf(buff, "%[^:]: %[^\r\n]",
				hh->h_receive[x].field,
				hh->h_receive[x].value
		      );
		cur += buff_len+2;
	}
	bzero(&hh->h_receive[max_header-1], sizeof(http_data));

	if(hh->method == GET){
		cur = 0;
		while(hh->uri[cur]){
			if(hh->uri[cur] == '?'){
				cur++;
				int vars = 1;
				int inc = 0;
				while(hh->uri[cur+inc]){
					if(hh->uri[cur+inc] == '&')
						vars++;
					inc++;
				}
				hh->h_form = grd_alloc(vars+1);
				bzero(&hh->h_form[vars],sizeof(http_data));
				for(int x=0; x<vars; x++){
					sscanf((const char *)hh->uri+cur,
							"%512[^=]=%[^&]",
							hh->h_form[x].field,
							hh->h_form[x].value);
					cur += strlen(hh->h_form[x].field)+
						strlen(hh->h_form[x].value)+
						2;
				}
				
			}
			cur++;
		}
	}
}
