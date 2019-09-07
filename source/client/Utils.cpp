#include "Utils.h"
#include "curses.h"
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

// Control-c killHandler
void killHandle(int signum) {
	endwin();
	exit(1);
}

// Calculates the time difference between two different struct timeval structures. Returns the number of milliseconds
int timedif(struct timeval start, struct timeval end) {
	return (end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec);
}
