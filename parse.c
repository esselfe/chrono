#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include "chrono.h"

static const struct option long_options[] = {
	{"help", no_argument, NULL, 'h'},
#ifdef HAVE_SDL
	{"beep", no_argument, NULL, 'B'},
#endif
#ifdef HAVE_X11
	{"borderless", no_argument, NULL, 'b'},
#endif
	{"command", required_argument, NULL, 'C'},
	{"countdown", required_argument, NULL, 'c'},
	{"debug", no_argument, NULL, 'D'},
	{"repeat", no_argument, NULL, 'r'},
	{"seconds", no_argument, NULL, 's'},
	{"test", no_argument, NULL, 't'},
	{"title", required_argument, NULL, 'T'},
	{"version", no_argument, NULL, 'V'},
#if defined HAVE_X11 && defined HAVE_NCURSES
	{"X11", no_argument, NULL, 'x'},
#endif
#ifdef HAVE_X11
	{"display", required_argument, NULL, 'd'},
	{"sticky", no_argument, NULL, 'S'},
	{"position-x", required_argument, NULL, 'X'},
	{"position-y", required_argument, NULL, 'Y'},
	{"width", required_argument, NULL, 'W'},
	{"height", required_argument, NULL, 'H'},
#endif
	{NULL, 0, NULL, 0}
};
#if defined HAVE_X11 && defined HAVE_NCURSES && !defined HAVE_SDL
static const char *short_options = "hbC:c:Dd:rSsT:tVxX:Y:W:H:";
#elif defined HAVE_X11 && !defined HAVE_NCURSES && !defined HAVE_SDL
static const char *short_options = "hbC:c:Dd:rSsT:tVX:Y:W:H:";
#elif defined HAVE_X11 && defined HAVE_NCURSES && defined HAVE_SDL
static const char *short_options = "hBbC:c:Dd:rSsT:tVxX:Y:W:H:";
#elif defined HAVE_X11 && !defined HAVE_NCURSES && defined HAVE_SDL
static const char *short_options = "hBbC:c:Dd:rSsT:tVX:Y:W:H:";
#else
static const char *short_options = "hbC:c:DrsT:tV";
#endif

void ParseArgs(int *argc, char **argv) {
	int c;
	while (1) {
		c = getopt_long(*argc, argv, short_options, long_options, NULL);
		if (c == -1) break;

		switch (c) {
		case 'h':
			ShowHelp();
			exit(0);
#ifdef HAVE_SDL
		case 'B':
			beep_enabled = 1;
			BeepInit();
			break;
#endif
#ifdef HAVE_X11
		case 'b':
			borderless = 1;
			break;
#endif
		case 'C':
			if (optarg != NULL)
				countdown_command = strdup(optarg);
			break;
		case 'c':
			if (optarg != NULL) {
				countdown = 1;
				tv_countdown.tv_sec = (long)ParseTimeToSeconds(optarg);
				if (tv_countdown.tv_sec <= 0)
					tv_countdown.tv_sec = 1;
			}
			break;
		case 'D':
			debug = 1;
			break;
#ifdef HAVE_X11
		case 'd':
			if (optarg != NULL) {
				display_name = (char *)malloc(strlen(optarg)+1);
				sprintf(display_name, "%s", optarg);
			}
			break;
#endif
		case 'r':
			countdown_repeat = 1;
			break;
		case 's':
			seconds_only = 1;
			break;
		case 'T':
			if (optarg != NULL) {
				window_title = (char *)malloc(strlen(optarg)+1);
				sprintf(window_title, "%s", optarg);
			}
			break;
		case 't':
			test = 1;
			debug = 1;
			break;
		case 'V':
			printf("chrono %s\n", chrono_version_string);
			exit(0);
#if defined HAVE_X11 && defined HAVE_NCURSES
		case 'x':
			use_x11 = 1;
			break;
#endif
#ifdef HAVE_X11
		case 'S':
			sticky = 1;
			break;
		case 'W':
			if (optarg != NULL)
				winW = atoi(optarg);
			break;
		case 'H':
			if (optarg != NULL)
				winH = atoi(optarg);
			break;
		case 'X':
			if (optarg != NULL)
				winX = atoi(optarg);
			break;
		case 'Y':
			if (optarg != NULL)
				winY = atoi(optarg);
			break;
#endif
		default:
			fprintf(stderr, "chrono error: Unknown option: %d:%c:\n", c, (char) c);
			break;
		}
	}
}

// timestr should be like "01:45:30" or "2:15" or "10"
long ParseTimeToSeconds(char *timestr) {
	unsigned int cnt = strlen(timestr)-1, seconds_total, hours, minutes, seconds,
		doing_seconds = 1, doing_minutes = 0, doing_hours = 0, doing_days = 0,
		doing_months = 0, doing_years = 0;
	char *c = timestr, buffer[32];
	if (debug)
		printf("\"%s\"\n", timestr);
	while (1) {
		memset(buffer, 0, 32);
		if (cnt == 0)
			buffer[0] = *c;
		else {
			if (*(c+cnt-1) == ':')
				buffer[0] = *(c+cnt);
			else if (*(c+cnt-2) == ':') {
				buffer[0] = *(c+cnt-1);
				buffer[1] = *(c+cnt);
			}
			else if (*(c+cnt-3) == ':') {
				buffer[0] = *(c+cnt-2);
				buffer[1] = *(c+cnt-1);
				buffer[2] = *(c+cnt);
			}
			else if (*(c+cnt-4) == ':') {
				buffer[0] = *(c+cnt-3);
				buffer[1] = *(c+cnt-2);
				buffer[2] = *(c+cnt-1);
				buffer[3] = *(c+cnt);
			}
			else if (*(c+cnt-5) == ':') {
				buffer[0] = *(c+cnt-4);
				buffer[1] = *(c+cnt-3);
				buffer[2] = *(c+cnt-2);
				buffer[3] = *(c+cnt-1);
				buffer[4] = *(c+cnt);
			}
			else if (*(c+cnt-6) == ':') {
				buffer[0] = *(c+cnt-5);
				buffer[1] = *(c+cnt-4);
				buffer[2] = *(c+cnt-3);
				buffer[3] = *(c+cnt-2);
				buffer[4] = *(c+cnt-1);
				buffer[5] = *(c+cnt);
			}
			else if (*(c+cnt-7) == ':') {
				buffer[0] = *(c+cnt-6);
				buffer[1] = *(c+cnt-5);
				buffer[2] = *(c+cnt-4);
				buffer[3] = *(c+cnt-3);
				buffer[4] = *(c+cnt-2);
				buffer[5] = *(c+cnt-1);
				buffer[6] = *(c+cnt);
			}
			// 64-bit signed long is 19 chars long, 9223372036854775807 from LONG_MAX in limits.h
			// but only do 7 chars max, 9 999 999
			else {
				buffer[0] = *(c+cnt-1);
				buffer[1] = *(c+cnt);
			}
		}
		if (debug)
			printf("<%s>", buffer);

		if (doing_seconds) {
			seconds = atoi(buffer);
			seconds_total = seconds;
			doing_seconds = 0;
			doing_minutes = 1;
			if (debug)
				printf(" (%us) [%ut]", seconds, seconds_total);
		}
		else if (doing_minutes) {
			minutes = atoi(buffer);
			seconds_total += minutes * 60;
			doing_minutes = 0;
			doing_hours = 1;
			if (debug)
				printf(" (%um) [%ut]", minutes, seconds_total);
		}
		else if (doing_hours) {
			hours = atoi(buffer);
			seconds_total += hours * 60 * 60;
			doing_hours = 0;
			doing_days = 1;
			if (debug)
				printf(" (%uh) [%ut]", hours, seconds_total);
		}
		else if (doing_days) {
			days = atoi(buffer);
			seconds_total += days * 24 * 60 * 60;
			doing_days = 0;
			doing_months = 1;
			if (debug)
				printf(" (%ud) [%ut]", days, seconds_total);
		}
		else if (doing_months) {
			months = atoi(buffer);
			if (months >= 1)
				seconds_total += 31 * 24 * 60 * 60;
			if (months >= 2) {
				time_t t0 = time(NULL);
				tm0 = gmtime(&t0);
				if (tm0->tm_year % 4 == 0)
					seconds_total += 29 * 24 * 60 * 60;
				else
					seconds_total += 28 * 24 * 60 * 60;
			}
			if (months >= 3)
				seconds_total += 31 * 24 * 60 * 60;
			if (months >= 4)
				seconds_total += 30 * 24 * 60 * 60;
			if (months >= 5)
				seconds_total += 31 * 24 * 60 * 60;
			if (months >= 6)
				seconds_total += 30 * 24 * 60 * 60;
			if (months >= 7)
				seconds_total += 31 * 24 * 60 * 60;
			if (months >= 8)
				seconds_total += 31 * 24 * 60 * 60;
			if (months >= 9)
				seconds_total += 30 * 24 * 60 * 60;
			if (months >= 10)
				seconds_total += 31 * 24 * 60 * 60;
			if (months >= 11)
				seconds_total += 30 * 24 * 60 * 60;
			doing_months = 0;
			doing_years = 1;
			if (debug)
				printf(" (%um) [%ut]", months, seconds_total);
		}
		else if (doing_years) {
			years = atoi(buffer);
			time_t t0 = time(NULL);
			tm0 = gmtime(&t0);
			if (tm0->tm_year % 4 == 0)
				seconds_total += years * 366 * 24 * 60 * 60;
			else
				seconds_total += years * 365 * 24 * 60 * 60;
			doing_years = 0;
			if (debug)
				printf(" (%uy) [%ut]", years, seconds_total);
		}
		if (debug)
			printf(" #%u %u\n", cnt, seconds_total);

		if (cnt == 0 || cnt == 1) {
			if (debug)
				printf("break, cnt: %u\n", cnt);
			break;
		}
		else if (cnt == 2)
			cnt -= 2;
		else if (cnt >= 3) {
			if (*(c+cnt-1) == ':')
				cnt -= 2;
			else if (*(c+cnt-2) == ':')
				cnt -= 3;
			else if (*(c+cnt-3) == ':')
				cnt -= 4;
			else if (*(c+cnt-4) == ':')
				cnt -= 5;
			else if (*(c+cnt-5) == ':')
				cnt -= 6;
			else if (*(c+cnt-6) == ':')
				cnt -= 7;
			else if (*(c+cnt-7) == ':')
				cnt -= 8;
			else
				cnt -= 2;
		}
	}

	return seconds_total;
}

