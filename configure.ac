
AC_INIT([chrono],[0.2.45],[bugs@esselfe.ca])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.9 foreign])
AC_CONFIG_HEADERS(config.h)

CFLAGS="-O2 -g"

AC_PROG_CC
AC_CHECK_HEADERS_ONCE([sys/time.h])

AC_CHECK_HEADERS(stdio.h sys/time.h limits.h errno.h)
LIBS="$LIBS -lc -lpthread"
AC_CHECK_HEADER(assert.h)
AC_CHECK_FUNCS(assert)
AC_CHECK_FUNCS(atoi exit getopt_long gettimeofday gmtime memset)
AC_FUNC_MALLOC
AC_CHECK_FUNCS(strdup strlen strerror timeradd timersub pthread_create usleep)

AC_ARG_ENABLE(ncurses,
	[AS_HELP_STRING([--disable-ncurses],[Do not compile ncurses code @<:@default=yes@:>@])],
	[enable_ncurses=$enableval], [enable_ncurses="yes"])

AC_MSG_CHECKING([whether to use ncurses])
if test x"$enable_ncurses" = xyes; then
	AC_MSG_RESULT(yes)
	CFLAGS=`pkg-config --cflags ncursesw`
	LIBS="$LIBS -lncurses"
	AC_CHECK_HEADERS(ncurses.h,,enable_ncurses=no)
	AC_CHECK_FUNCS([getch initscr endwin printw refresh])
	AC_DEFINE(HAVE_NCURSES, 1, [Define to 1 if you have ncurses installed])
else
	AC_MSG_RESULT(no)
	AC_MSG_WARN([*** ncurses support will not be compiled ***])
fi
AM_CONDITIONAL([HAVE_NCURSES], [test x"$enable_ncurses" = xyes])

AC_ARG_ENABLE(x11,
	[AS_HELP_STRING([--disable-x11], [Do not compile X11 code @<:@default=yes@:>@])],
	[enable_x11=$enableval], [enable_x11="yes"])

AC_MSG_CHECKING([whether to use X11])
if test x"$enable_x11" = xyes; then
	AC_MSG_RESULT(yes)
	LIBS="$LIBS -lX11 -lXext"
	AC_PATH_X
	AC_CHECK_HEADERS(X11/Xlib.h X11/Xatom.h X11/Xutil.h,,enable_x11=no)
	AC_CHECK_FUNCS([XOpenDisplay XPending XNextEvent XSetErrorHandler XCreateWindow])
	AC_CHECK_FUNCS([XSetWMNormalHints XSetWMHints XSetWMName XStringListToTextProperty])
	AC_CHECK_FUNCS([XSetWMIconName XLoadQueryFont XCreateGC XMapWindow XInternAtom])
	AC_CHECK_FUNCS([XSendEvent XClearArea XDrawText XUnmapWindow XDestroyWindow])
	AC_DEFINE(HAVE_X11, 1, [Define to 1 if you have libX11 installed])
else
	AC_MSG_RESULT(no)
	AC_MSG_WARN([*** libX11 support will not be compiled ***])
fi
AM_CONDITIONAL([HAVE_X11], [test x"$enable_x11" = xyes])

if test x"$enable_x11" = xno && test x"$enable_ncurses" = xno; then
	AC_MSG_WARN([One or both X11 and ncurses support must be enabled.])
	AC_MSG_ERROR([Use configure options --enable-ncurses and/or --enable-x11])
fi

AC_ARG_ENABLE(sdl,
	[AS_HELP_STRING([--disable-sdl], [Do not compile SDL2 code @<:@default=yes@:>@])],
	[enable_sdl=$enableval], [enable_sdl="yes"])

AC_MSG_CHECKING([whether to use SDL])
if test x"$enable_sdl" = xyes; then
	AC_MSG_RESULT(yes)
	LIBS="$LIBS -lSDL2"
	AC_CHECK_HEADERS(SDL2/SDL.h SDL2/SDL_audio.h,,enable_sdl=no)
	AC_CHECK_FUNCS([SDL_Init SDL_OpenAudio SDL_CloseAudio SDL_PauseAudio SDL_Quit])
	AC_DEFINE(HAVE_SDL, 1, [Define to 1 if you have SDL2 installed])
else
	AC_MSG_RESULT(no)
	AC_MSG_WARN([*** SDL2 support will not be compiled ***])
fi
AM_CONDITIONAL([HAVE_SDL], [test x"$enable_sdl" = xyes])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

