all:
	gcc -c logger.c -o logger.o
	gcc -c monitor.c -o mon.o
	gcc logger.o mon.o -o monitor
clean:
	rm logger.o
	rm mon.o
	rm monitor

include ../../common.mk
