#ifndef CHRONO_H
#define CHRONO_H 1

#include "config.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

extern const char *chrono_version_string;
extern char *window_title;
extern unsigned int use_x11;
extern unsigned int beep_enabled;
extern unsigned int beep_trigger;
extern unsigned int test;
extern unsigned int debug;
extern unsigned int endmainloop;
extern unsigned int paused;
extern unsigned int seconds_only;
extern unsigned int days;
extern unsigned int months;
extern unsigned int years;
extern unsigned int countdown_repeat;
extern unsigned int countdown;
extern char *countdown_command;
extern struct timeval tv_initial;
extern struct timeval tv0;
extern struct timeval tv_countdown;
extern struct timeval tv_countdown_restore;
extern struct timeval tv_start;
extern struct timeval tv_current;
extern struct timeval tv_diff;
extern struct timeval tv_prev;
extern struct timeval tv_paused;
extern struct timeval tv_paused_current;
extern struct timeval tv_paused_start;
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
extern int winX;
extern int winY;
extern int winW;
extern int winH;
extern Window root_window;
extern Window window;
extern GC gc;
extern int text_item_size;
extern XTextItem text_item;
extern unsigned int borderless; // bool
extern unsigned int sticky;     // bool
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
