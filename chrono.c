/* June 2019
chrono is a Linux terminal/ncurses and/or X11 based chronometer with
optional millisecond precision, seconds, minutes, hours, days, months,
years, repeateable countdown, pause and reset function
*/
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <ctype.h>
#include <assert.h>
#include "chrono.h"

#ifdef HAVE_NCURSES
#include <ncurses.h>
#endif

#ifdef HAVE_X11
#include <X11/Xlib.h>
#endif

#ifdef HAVE_SDL
#include <SDL2/SDL.h>
#endif

// The version is set in configure.ac
const char *chrono_version_string = VERSION;
char *window_title;
unsigned int use_x11;
unsigned int beep_enabled;
unsigned int beep_trigger;
unsigned int debug;
unsigned int endmainloop;
unsigned int paused;
unsigned int seconds_only;
unsigned int days;
unsigned int months;
unsigned int years;
unsigned int countdown_repeat;
unsigned int countdown;
char *countdown_command;
struct timeval tv_initial;
struct timeval tv0;
struct timeval tv_countdown;
struct timeval tv_countdown_restore;
struct timeval tv_start;
struct timeval tv_current;
struct timeval tv_diff;
struct timeval tv_prev;
struct timeval tv_paused;
struct timeval tv_paused_current;
struct timeval tv_paused_start;
struct tm *tm0;

void ShowHelp(void) {
	printf("chrono options:\n"
"\t-h, --help               Show this help message\n"
#ifdef HAVE_X11
"\t-b, --borderless         Create X11 window without decorations or borders\n"
#endif
#ifdef HAVE_SDL
"\t-B, --beep               Play a sound at the end of a countdown\n"
#endif
"\t-C, --command COMMAND    Execute COMMAND when countdown ends\n"
"\t-c, --countdown FORMAT   Countdown using one of these formats: 15, 2:30 or 1:15:05\n"
"\t-D, --debug              Show detailed debug informations\n"
#ifdef HAVE_X11
"\t-d, --display NAME       Use specified X11 display name (ie \":0.0\" or \":1.0\")\n"
#endif
"\t-r, --repeat             Restart countdown once reaching 0\n"
#ifdef HAVE_X11
"\t-S, --sticky             Show window on all desktops\n"
#endif
"\t-s, --seconds            Display seconds instead of milliseconds\n"
"\t-T, --title TEXT         Put TEXT as window title\n"
"\t-t, --test               Run correctness tests and exit\n"
"\t-V, --version            Show the program version and exit\n"
#if HAVE_X11 && HAVE_NCURSES
"\t-x, --X11                Use XOrg X11 Xlib as GUI\n"
#endif
#ifdef HAVE_X11
"\t-X, --position-x XNUM    Set the horizontal window position\n"
"\t-Y, --position-y YNUM    Set the vertical window position\n"
"\t-W, --width WNUM         Set window width\n"
"\t-H, --height HNUM        Set window height\n"
#endif
"Once chrono has started, press 'q' to quit, 'r' to reset and 'p' or <space> to pause\n");
}

void TimeReset(void) {
	gettimeofday(&tv0, NULL);

	if (paused)
		tv_paused_start = tv0;
	
	if (countdown)
		tv_countdown = tv_countdown_restore;
	else
		tv_start = tv0;
	
	tv_paused.tv_sec = 0;
	tv_paused.tv_usec = 0;
	if (paused)
		tv_paused_start = tv0;
	
	TimeUpdate();

#ifdef HAVE_X11
	if (use_x11)
		xlibTimeStringUpdate();
#endif
#ifdef HAVE_NCURSES
	if (!use_x11)
		ncursesTimeStringUpdate();
#endif
}

void TimeUpdate(void) {
	gettimeofday(&tv0, NULL);
	timersub(&tv0, &tv_start, &tv_diff);
	timersub(&tv_diff, &tv_paused, &tv_current);
	if (countdown) {
		timersub(&tv_current, &tv_prev, &tv_diff);
		if (tv_diff.tv_sec < 0)
			tv_diff.tv_sec = 0;
		tv_prev = tv_current;
		timersub(&tv_countdown, &tv_diff, &tv_countdown);
		if (tv_countdown.tv_sec <= 0 && tv_countdown.tv_usec <= 1000 && 
			(!days && !months && !years) || tv_countdown.tv_sec < 0) {
			if (countdown_repeat) {
				// not a typo, needs some vars to be reupdated
				TimeReset();
				TimeReset();
			}
			else
				endmainloop = 1;

			if (countdown_command)
				system(countdown_command);
#ifdef HAVE_SDL
			if (beep_enabled)
				beep_trigger = 1;
#endif
		}
		tm0 = gmtime(&tv_countdown.tv_sec);
	}
	else
		tm0 = gmtime(&tv_current.tv_sec);

	days = tm0->tm_mday - 1;
	months = tm0->tm_mon;
	years = tm0->tm_year - 70;
}

void SignalFunc(int signum) {
	switch (signum) {
	case SIGUSR1:
		paused = !paused;
		gettimeofday(&tv0, NULL);
		if (paused)
			tv_paused_start = tv0;
		else {
			tv_paused_current = tv0;
			timersub(&tv_paused_current, &tv_paused_start, &tv_diff);
			timeradd(&tv_paused, &tv_diff, &tv_paused);
		}
		TimeUpdate();
#ifdef HAVE_NCURSES
		if (!use_x11)
			ncursesTimeStringUpdate();
#endif
#ifdef HAVE_X11
		if (use_x11)
			xlibTimeStringUpdate();
#endif
		break;
	case SIGUSR2:
		TimeReset();
		break;
	}
}

int main(int argc, char **argv) {
	signal(SIGUSR1, SignalFunc);
	signal(SIGUSR2, SignalFunc);

	ParseArgs(&argc, argv);

	if (test) {
		Test();
		exit(0);
	}
	
	if (countdown || countdown_repeat)
		tv_countdown_restore = tv_countdown;

#if defined HAVE_X11 && !defined HAVE_NCURSES
	use_x11 = 1;
#endif

#ifdef HAVE_X11
	xlibInit();
#endif

#ifdef HAVE_NCURSES
	ncursesInit();
#endif
	
	struct timeval tv_loop_start;
	struct timeval tv_loop_end;
	gettimeofday(&tv_start, NULL);
	while (!endmainloop) {
		gettimeofday(&tv_loop_start, NULL);
#ifdef HAVE_X11
		if (use_x11)
			xlibKeysCheck();
#endif
		
		if (!paused) {
			TimeUpdate();
#ifdef HAVE_X11
			if (use_x11)
				xlibTimeStringUpdate();
#endif
#ifdef HAVE_NCURSES
			if(!use_x11)
				ncursesTimeStringUpdate();
#endif
		}

		gettimeofday(&tv_loop_end, NULL);
		timersub(&tv_loop_end, &tv_loop_start, &tv_diff);
		if (tv_diff.tv_sec == 0 && tv_diff.tv_usec < 1000)
			usleep(1000-tv_diff.tv_usec);
		else
			usleep(1000);
	}

#ifdef HAVE_SDL
	if (beep_enabled) {
		SDL_CloseAudio();
		SDL_Quit();
	}
#endif
#ifdef HAVE_X11
	if (use_x11) {
		XUnmapWindow(display, window);
		XDestroyWindow(display, window);
		XCloseDisplay(display);
	}
#endif
#ifdef HAVE_NCURSES
	if (!use_x11)
		endwin();
#endif
	return 0;
}

