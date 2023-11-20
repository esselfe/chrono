#include "config.h"
#include <stdlib.h>
#include <ncurses.h>
#include <sys/time.h>
#include "chrono.h"

#include <ncurses.h>
#include <pthread.h>

void ncursesInit(void) {
	if (!use_x11) {
		initscr();
		noecho();
		pthread_t thr;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&thr, &attr, ncursesKeysThreadFunc, NULL);
		pthread_detach(thr);
		pthread_attr_destroy(&attr);
	}

	if (window_title) {
		char *cmdstr = (char *)malloc(1024);
		sprintf(cmdstr, "echo -en \"\033]0;%s\007\"", window_title);
		system(cmdstr);
		free(cmdstr);
	}
}

void *ncursesKeysThreadFunc(void *ptr) {
	int c;
	while (!endmainloop) {
		c = getch();
		switch (c) {
		case 'D':
			debug = !debug;
			break;
		case 'p':
		case ' ':
			paused = !paused;
			if (paused)
				gettimeofday(&tv_paused_start, NULL);
			else {
				gettimeofday(&tv_paused_current, NULL);
				timersub(&tv_paused_current, &tv_paused_start, &tv_diff);
				timeradd(&tv_paused, &tv_diff, &tv_paused);
			}
			TimeUpdate();
			ncursesTimeStringUpdate();
			break;
		case 'q':
			endmainloop = 1;
			break;
		case 'r':
			TimeReset();
			break;
		case 's':
			seconds_only = !seconds_only;
			break;
		}
	}
	return NULL;
}

void ncursesTimeStringUpdate(void) {
	clear();

	if (years)
			printw(" %dy", years);
	if (months)
			printw(" %dm", months);
	if (days)
			printw(" %dd", days);

	if (seconds_only)
		printw(" %02ld:%02ld:%02ld\t\t\r",
			tm0->tm_hour, tm0->tm_min, tm0->tm_sec);
	else {
		if (countdown)
			printw(" %02ld:%02ld:%02ld.%03lu\t\t\r",
				tm0->tm_hour, tm0->tm_min, tm0->tm_sec, tv_countdown.tv_usec/1000);
		else
			printw(" %02ld:%02ld:%02ld.%03lu\t\t\r",
				tm0->tm_hour, tm0->tm_min, tm0->tm_sec, tv_current.tv_usec/1000);
	}
	refresh();
}

