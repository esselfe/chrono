#ifndef CHRONO_H
#define CHRONO_H 1

#include "config.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

extern const char *chrono_version_string;
extern char *window_title;
extern unsigned int use_x11, beep_enabled, beep_trigger;
extern unsigned int test, debug, endmainloop, paused, seconds_only;
extern unsigned int days, months, years;
extern unsigned int countdown_repeat, countdown;
extern char *countdown_command;
extern struct timeval tv_initial, tv0, tv_countdown, tv_countdown_restore,
    tv_start, tv_current, tv_diff, tv_prev,
    tv_paused, tv_paused_current, tv_paused_start;
extern struct tm *tm0;

/* from chrono.c */
void ShowHelp(void);
void TimeReset(void);
void TimeUpdate(void);

#ifdef HAVE_SDL
/* from beep.c */
void BeepInit(void);
#endif

/* from parse.c */
void ParseArgs(int *argc, char **argv);
long ParseTimeToSeconds(char *timestr);

/* from test.c */
extern unsigned int test;
void Test(void);

/* from xlib.c */
#ifdef HAVE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern Display *display;
extern char *display_name;
extern int winX, winY, winW, winH;
extern Window root_window, window;
extern GC gc;
extern int text_item_size;
extern XTextItem text_item;
extern unsigned int borderless, sticky; // bools
#define _NET_WM_STATE_REMOVE        0	// remove/unset property
#define _NET_WM_STATE_ADD           1	// add/set property
#define _NET_WM_STATE_TOGGLE        2	// toggle property

void xlibInit(void);
int xlibErrorFunc(Display *dsp, XErrorEvent *error);
int xlibIOErrorFunc(Display *dsp);
void xlibKeysCheck(void);
void xlibTimeStringUpdate(void);
void xlibWindowInit(void);
#endif

#ifdef HAVE_NCURSES
/* from ncurses.c */
void ncursesInit(void);
void *ncursesKeysThreadFunc(void *ptr);
void ncursesTimeStringUpdate(void);
#endif

#endif /* CHRONO_H */
