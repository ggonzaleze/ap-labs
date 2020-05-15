#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <unistd.h>
#include <ftw.h>
#include <string.h>
#include "logger.h"
#define BUFFER (10 * (sizeof(struct inotify_event) + 1))

int initfile;

int addWatcher(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
	int watcher = inotify_add_watch(initfile, fpath, IN_CREATE | IN_DELETE | IN_MODIFY | IN_ISDIR);
	if(watcher == -1){
		errorf("Error occurred  adding Watcher \n");
        	exit(1);
	}
	return 0;
}

void assignWatch(char *fpath){
	if(nftw(fpath,addWatcher,1000,0) == -1){
		errorf("Error occurred assigning Watcher \n");
	}
}

void alertChange(struct inotify_event *event, char *path){
	if(event->mask & IN_CREATE){
		infof("Created %s \n",event->name);
		assignWatch(path);
	}
	else if(event->mask & IN_DELETE){
		infof("Deleted %s \n",event->name);
                assignWatch(path);
	}
	else if(event->mask & IN_MODIFY){
		infof("Modified %s \n",event->name);
                assignWatch(path);
	}
}

int main(int argc, char *argv[]){
	if((initfile = inotify_init()) == -1){
		errorf("Error occurred initializing inotify \n");
		exit(1);
	}
	int readf;
	char *p;
	struct inotify_event *event;
	char buff[BUFFER];
	char path[strlen(argv[1])*100];
	strcpy(path, argv[1]);
	assignWatch(path);
	while(1){
		readf = read(initfile,buff,BUFFER);
		if(readf == 0){
			errorf("Error occurred initializing inotify \n");
		}
		for(p = buff; p < buff + readf; ){
			event = (struct inotify_event*) p;
			alertChange(event, path);
			p = p + (sizeof(struct inotify_event)+event->len);
		}
	}
	close(initfile);
	return 0;
}
