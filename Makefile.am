
bin_PROGRAMS = chrono
chrono_SOURCES = chrono.c chrono.h parse.c test.c

if HAVE_SDL
chrono_SOURCES += beep.c
endif

if HAVE_NCURSES
chrono_SOURCES += ncurses.c
endif

if HAVE_X11
chrono_SOURCES += xlib.c
endif

