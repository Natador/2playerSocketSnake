/*
 * Utility functions to control certain Unix components.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <cstdlib>

void killHandle(int signum);
int timedif(struct timeval start, struct timeval end);

#endif
