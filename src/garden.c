/*
 *
 * 	Name: Garden
 * 	Full Name: Garden-Framework
 * 	Version: 0.1.0
 * 	Description:
 * 		garden framework is a framework developed in C,
 * 		and use C libraries to create routes by yours
 * 		methods.
 *	Author:	Daniel B. Brumazzi
 *	E-mail:	brumazzi_daniel@yahoo.com.br
 *
 */

#include <connect.h>
#include <sock-io.h>
#include <stdio.h>
#include <attrib.h>
#include <view.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <mem.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SYS_VERSION	"v0.1.0"

struct _propertis{
	unsigned int port;
	short max_threads;
	char sync : 2;
};

struct _data_conf{
	pthread_t *pt;
	char sync : 2;
	int fd;
	pthread_mutex_t *mt;
};

void sig_fault(int sig);
void sig_term(int sig);
void sig_int(int sig);
void *t_callback(void *data);

const char *_usage = ""
"Usage: garden <propert> [value] ...\n"
"Run garden web server\n"
"\n"
"Example:\n"
"	garden -server -port 8000\n"
"\n"
" -server			start application server.\n"
" -port		<port_number>	set connection port.\n"
" -threads	<num_threads>	set max threads to client connect.\n"
" -sync				define synchronize threads.\n"
" -create	<application>	create application firectory.\n"
" -version			show version"
"";

int fd_server;

int main(int argc, const char **argv){
	/* signals configurations */
	signal(SIGSEGV, sig_fault);
	signal(SIGTERM, sig_term);
	signal(SIGINT, sig_int);

	if(argc < 2){
		puts(_usage);

		return 2;
	}

	/* default config */
	struct _propertis pr;
	pr.port = 8001;
	pr.max_threads = 1;
	pr.sync = 0;

	/* get application path */
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);

	char serv_up = 0;
	/* check argvs */
	for(int x=1; x<argc; x++){
		if(!strcmp(argv[x], "-port")){ /* define port value */
			pr.port = atoi(argv[++x]);
			if(!pr.port){
				fprintf(stderr, "Invalid port %s\n", argv[x-1]);
				return 1;
			}
			continue;
		}if(!strcmp(argv[x], "-threads")){ /* set max threads to use */
			pr.max_threads = atoi(argv[++x]);
			if(pr.max_threads < 1){
				fprintf(stderr, "Invalid threads value \"%s\"\n", argv[x-1]);
				return 1;
			}
			continue;
		}if(!strcmp(argv[x], "-sync")){ /* active synchronized threads */
			pr.sync = 1;
			continue;
		}if(!strcmp(argv[x], "-create")){ /* create application folder - no necessary */
			char app_name[PATH_MAX];

			strcpy(app_name, path);
			strcat(app_name, argv[++x]);
			mkdir(app_name, 0755);
			return 0;
		}if(!strcmp(argv[x], "-server")){
			serv_up = 1;
			continue;
		}if(!strcmp(argv[x], "-version")){
			puts(SYS_VERSION);
		}

		fprintf(stderr, "Invalid argument: %s\n", argv[x]);
	}

	if(!serv_up){
		fprintf(stderr, "Use \"-server\" to run garden server\n");
		return 4;
	}

	pthread_t t_line[pr.max_threads];
	pthread_mutex_t t_sync;

	for(int x=0; x<pr.max_threads; x++){
		t_line[x] = 0;
	}
	
	int server = grd_server(pr.port);
	fd_server = server;
	if(server < 0){
		perror("Error");
		return 3;
	}

	fprintf(stderr, "garden as runing in port %i\n", pr.port);

	if(pr.sync)
		pthread_mutex_init(&t_sync, NULL);

	struct _data_conf *conf;
	char *header = 0;
#ifndef DEBUG
	while(1){
		for(int x=0; x<pr.max_threads; x++){
			if(t_line[x] == 0){
				int client = grd_accept(server);
				if(client < 0){
					perror("Error");
					continue;
				}
				conf = grd_alloc(sizeof(struct _data_conf));
				conf->pt = &t_line[x];
				conf->sync = pr.sync;
				conf->fd = client;
				conf->mt = &t_sync;

				pthread_create(&t_line[x], NULL, t_callback, (void *) conf);
				pthread_join(t_line[x], NULL);
			}
		}
	}
#endif
	if(pr.sync)
		pthread_mutex_destroy(&t_sync);

	grd_close(server);
		
	return 0;
}

void *t_callback(void *data){
	struct _data_conf *conf = (struct _data_conf*) data;
	if(conf->sync)
		pthread_mutex_lock(conf->mt);

	char *header = NULL;
	grd_recv_len(conf->fd, &header, HTTP_MAX_HEADER_SIZE);

	if(header){
		http_header hh;
		grd_header_init(&hh, conf->fd);

		grd_header_parser(&hh, header);

		grd_view_open(&hh);

		grd_header_destroy(&hh);
		grd_free(header);
	}else{
		perror("Error");
	}

	if(conf->sync)
		pthread_mutex_unlock(conf->mt);

	(*conf->pt) = 0;
	grd_close(conf->fd);

	grd_free(data);
	return NULL;
}

void sig_fault(int sig){
	fprintf(stderr, "Error: Segment fault\n");
	exit(sig);
}

void sig_term(int sig){
	fprintf(stderr, "Closing application\n");
	exit(sig);
}

void sig_int(int sig){
	fprintf(stderr, "Closing application\n");
	grd_close(fd_server);
	exit(sig);
}
