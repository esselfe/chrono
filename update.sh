#!/bin/bash

case "$1" in
-r)
	cp -uv ./{chrono.c,chrono.h,config.h.in,parse.c,configure,configure.ac,INSTALL} ~/code/chrono
	cp -uv ./{beep.c,Makefile.am,Makefile.in,ncurses.c,README,test.c,xlib.c} ~/code/chrono
	exit 0
	;;
esac

cp -uv ~/code/chrono/{chrono.c,chrono.h,config.h.in,parse.c,configure,configure.ac,INSTALL} .
cp -uv ~/code/chrono/{beep.c,Makefile.am,Makefile.in,ncurses.c,README,test.c,xlib.c} .
git add chrono.c chrono.h config.h.in parse.c configure configure.ac INSTALL
git add beep.c Makefile.am Makefile.in ncurses.c README test.c xlib.c
