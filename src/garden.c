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

struct _propertis{
	unsigned int port;
	short max_threads;
	char sync : 2;
};

struct _data_conf{
	pthread_t pt;
	char sync : 2;
	int fd;
	pthread_mutex_t *mt;
};

int sig_fault(int sig);
int sig_term(int sig);
int sig_int(int sig);
void *t_callback(void *data);

const char *_usage = ""
"Usage: garden <propert> [value] ...\n"
"Run garden web server\n"
"\n"
"Example:\n"
"	garden -server -port 8000\n"
"\n"
" -port		<port_number>	set connection port.\n"
" -threads	<num_threads>	set max threads to client connect.\n"
" -sync				define synchronize threads.\n"
" -create	<application>	create application firectory.\n"
"\n";

int main(int argc, const char **argv){
	/* signals configurations */
	signal(SIGSEGV, sig_fault);
	signal(SIGTERM, sig_term);
	signal(SIGINT, sig_int);

	if(argc < 2){
		printf(_usage);

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
		}
	}

	pthread_t t_line[pr.max_threads];
	pthread_mutex_t t_sync;

	for(int x=0; x<pr.max_threads; x++){
		t_line[x] = 0;
	}
	
	int server = grd_server(pr.port);

	if(pr.sync)
		pthread_mutex_init(&t_sync, NULL);

	while(1){
		for(int x=0; x<pr.max_threads; x++){
			usleep(500000);
			if(t_line[x] == 0){
				int client = grd_accept(server);
				if(client == -1){
					perror("Error");
					continue;
				}
				struct _data_conf *conf = grd_alloc(sizeof(struct _data_conf));
				conf->pt = &t_line[x];
				conf->sync = pr.sync;
				conf->fd = client;
				conf->mt = &t_sync;

				pthread_create(&t_line[x], NULL, t_callback, (void *) conf);
				pthread_join(&t_line[x], NULL);
			}
		}
	}

	pthread_mutex_destroy(&t_sync);
		
	return 0;
}

void *t_callback(void *data){
	struct _data_conf *conf = (struct _data_conf*) data;
	if(conf->sync)
		pthread_mutex_lock(conf->mt);

	/* TODO */

	if(conf->sync)
		pthread_mutex_unlock(conf->mt);

	return NULL;
}

int sig_fault(int sig){
	fprintf(stderr, "Error: Segment fault\n");
	exit(sig);

	return sig;
}

int sig_term(int sig){
	/*fprintf(stderr, "Closing application\n");
	exit(sig);*/

	return sig;
}

int sig_int(int sig){
	fprintf(stderr, "Closing application\n");
	exit(sig);

	return sig;
}
