#include <stdio.h>
#include "logger.h"
#include <stdarg.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>

#define RESET		0
#define BRIGHT 		1
#define DIM		2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

void textcolor(int attr, int fg, int bg);

int toSyslog = 0;

int initLogger(char *logType) {
	printf("Initializing Logger on: %s\n", logType);
	if(strcmp(logType,"syslog") == 0)
		toSyslog = 1;
	if(strcmp(logType,"stdout") == 0)
		toSyslog = 0;
	return 0;
}

int infof(const char *format, ...) {
	va_list args;
	va_start(args, format);
	if(toSyslog == 1){
		vsyslog(LOG_INFO, format, args);
		va_end(args);
		return 0;
	}else{
		textcolor(BRIGHT, GREEN, BLACK);
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                int r = vfprintf(stdout,format, args);
                textcolor(RESET, WHITE, BLACK);
                va_end(args);
                return r;
	}
}

int warnf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	if(toSyslog == 1){
                vsyslog(LOG_WARNING, format, args);
                va_end(args);
                return 0;
        }else{
        	textcolor(BRIGHT, YELLOW, BLACK);
		time_t t = time(NULL);
        	struct tm tm = *localtime(&t);
        	printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        	int r = vfprintf(stdout, format, args);
        	textcolor(RESET, WHITE, BLACK);
        	va_end(args);
        	return r;
	}
}

int errorf(const char *format, ...) {
	va_list args;
        va_start(args, format);
	if(toSyslog == 1){
                vsyslog(LOG_ERR, format, args);
                va_end(args);
                return 1;
        }else{
        	textcolor(BRIGHT, RED, BLACK);
		time_t t = time(NULL);
       		struct tm tm = *localtime(&t);
        	printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        	int r = vfprintf(stdout, format, args);
        	textcolor(RESET, WHITE, BLACK);
        	va_end(args);
        	return r;
	}
}
int panicf(const char *format, ...) {
	va_list args;
        va_start(args, format);
	if(toSyslog == 1){
                vsyslog(LOG_ALERT, format, args);
                va_end(args);
                return 1;
        }else{
		time_t t = time(NULL);
        	struct tm tm = *localtime(&t);
        	printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        	int r = vfprintf(stdout, format, args);
        	va_end(args);
		raise (SIGABRT);
        	return r;
	}
}

void textcolor(int attr, int fg, int bg){
	char command[13];
	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}
