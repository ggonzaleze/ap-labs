#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>

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

int infof(const char *format, ...){
	va_list args;
	va_start(args, format);
	textcolor(BRIGHT, GREEN, BLACK);
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	int r = vfprintf(stdout,format, args);
	textcolor(RESET, WHITE, BLACK);
	va_end(args);
	return r;
}

int warnf(const char *format, ...){
        va_list args;
        va_start(args, format);
        textcolor(BRIGHT, YELLOW, BLACK);
	time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        int r = vfprintf(stdout, format, args);
        textcolor(RESET, WHITE, BLACK);
        va_end(args);
        return r;
}

int errorf(const char *format, ...){
        va_list args;
        va_start(args, format);
        textcolor(BRIGHT, RED, BLACK);
	time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        int r = vfprintf(stdout, format, args);
        textcolor(RESET, WHITE, BLACK);
        va_end(args);
        return r;
}

int panicf(const char *format, ...){
        va_list args;
        va_start(args, format);
	time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("%d-%02d-%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        int r = vfprintf(stdout, format, args);
        va_end(args);
	raise (SIGABRT);
        return r;
}

void textcolor(int attr, int fg, int bg){
	char command[13];
	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}
