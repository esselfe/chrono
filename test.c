#include "config.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>
#include <ncurses.h>
#include "chrono.h"

unsigned int test;

void Test(void) {
	unsigned int lineno_start = __LINE__;
    assert(ParseTimeToSeconds("0") == 0);
    assert(ParseTimeToSeconds("1") == 1);
    assert(ParseTimeToSeconds("2") == 2);
    assert(ParseTimeToSeconds("3") == 3);
    assert(ParseTimeToSeconds("4") == 4);
    assert(ParseTimeToSeconds("5") == 5);
    assert(ParseTimeToSeconds("10") == 10);
    assert(ParseTimeToSeconds("11") == 11);
    assert(ParseTimeToSeconds("15") == 15);
    assert(ParseTimeToSeconds("21") == 21);
    assert(ParseTimeToSeconds("25") == 25);
    assert(ParseTimeToSeconds("59") == 59);
    assert(ParseTimeToSeconds("60") == 60);
    assert(ParseTimeToSeconds("90") == 90);
    assert(ParseTimeToSeconds("1:0") == 60);
    assert(ParseTimeToSeconds("1:1") == 61);
    assert(ParseTimeToSeconds("1:02") == 62);
    assert(ParseTimeToSeconds("1:5") == 65);
    assert(ParseTimeToSeconds("1:05") == 65);
    assert(ParseTimeToSeconds("1:10") == 70);
    assert(ParseTimeToSeconds("1:120") == 3*60);
    assert(ParseTimeToSeconds("1:1200") == 21*60);
    assert(ParseTimeToSeconds("01:0") == 60);
    assert(ParseTimeToSeconds("2:00") == 120);
    assert(ParseTimeToSeconds("2:01") == 121);
    assert(ParseTimeToSeconds("2:05") == 125);
    assert(ParseTimeToSeconds("2:10") == 130);
    assert(ParseTimeToSeconds("1:00:00") == 3600);
    assert(ParseTimeToSeconds("1:00:01") == 3601);
    assert(ParseTimeToSeconds("1:00:11") == 3611);
    assert(ParseTimeToSeconds("1:01:00") == 3660);
    assert(ParseTimeToSeconds("1:01:01") == 3661);
    assert(ParseTimeToSeconds("1:01:11") == 3671);
    assert(ParseTimeToSeconds("1:01:21") == 60*60 + 60 + 21);
    assert(ParseTimeToSeconds("1:1:00") == 3660);
    assert(ParseTimeToSeconds("1:1:01") == 3661);
    assert(ParseTimeToSeconds("1:1:11") == 3671);
    assert(ParseTimeToSeconds("1:1:10") == 60*60 + 60 + 10);
    assert(ParseTimeToSeconds("2:00:00") == 7200);
    assert(ParseTimeToSeconds("2:00:01") == 7201);
    assert(ParseTimeToSeconds("2:00:10") == 7210);
    assert(ParseTimeToSeconds("2:00:11") == 7211);
    assert(ParseTimeToSeconds("2:1:0") == 2*60*60+60);
    assert(ParseTimeToSeconds("2:01:0") == 2*60*60+60);
    assert(ParseTimeToSeconds("2:1:1") == 2*60*60+60+1);
    assert(ParseTimeToSeconds("2:11:1") == 2*60*60+11*60+1);
    assert(ParseTimeToSeconds("10:00:01") == 10*60*60+1);
    assert(ParseTimeToSeconds("1:01:00:00") == 24*(60*60) + 60*60);
    assert(ParseTimeToSeconds("1:10:00:01") == 24*(60*60) + 10*60*60 + 1);
    assert(ParseTimeToSeconds("1:0:0:0:1") == 31*24*60*60 + 1);
    assert(ParseTimeToSeconds("1:0:0:02:1") == 31*24*60*60 + 2*60 + 1);
    assert(ParseTimeToSeconds("1:0:0:2:5") == 31*24*60*60 + 2*60 + 5);
    assert(ParseTimeToSeconds("1:10:0:0:1") == 31*24*60*60 + 10*24*60*60 + 1);
	time_t t0 = time(NULL);
	tm0 = gmtime(&t0);
	if (tm0->tm_year % 4 == 0) {
	    assert(ParseTimeToSeconds("2:0:0:0:2") == 31*24*60*60 + 29*24*60*60 + 2);
	    assert(ParseTimeToSeconds("1:0:0:0:0:0") == 366*24*60*60);
	}
	else {
	    assert(ParseTimeToSeconds("2:0:0:0:2") == 31*24*60*60 + 28*24*60*60 + 2);
    	assert(ParseTimeToSeconds("1:0:0:0:0:0") == 365*24*60*60);
	}
	unsigned int lineno_end = __LINE__-6;
    printf("All %u ParseTimeToSeconds() tests succeeded\n", lineno_end - lineno_start - 1);

	printf("char: %d\n", sizeof(char)*8);
    printf("uchar: %d\n", sizeof(unsigned char)*8);
    printf("short: %d\n", sizeof(short)*8);
    printf("ushort: %d\n", sizeof(unsigned short)*8);
    printf("int: %d\n", sizeof(int)*8);
    printf("uint: %d\n", sizeof(unsigned int)*8);
    printf("long: %d\n", sizeof(long)*8);
    printf("ulong: %d\n", sizeof(unsigned long)*8);
    printf("longlong: %d\n", sizeof(long long)*8);
    printf("ulonglong: %d\n", sizeof(unsigned long long)*8);
    printf("time_t: %d\n", sizeof(time_t)*8);
    struct timeval tvx;
    printf("tv_sec: %d\n", sizeof(tvx.tv_sec)*8);
    printf("tv_usec: %d\n", sizeof(tvx.tv_usec)*8);
    tvx.tv_sec = 0;
    --tvx.tv_sec;
    printf("tv_sec: %ssigned\n", (tvx.tv_sec<0)?"":"un");
    tvx.tv_usec = 0;
    --tvx.tv_usec;
    printf("tv_usec: %ssigned\n", (tvx.tv_usec<0)?"":"un");
	printf("INT_MAX: %lu\n", INT_MAX);
	printf("LONG_MAX: %lu\n", LONG_MAX);
	
	printf("years max (%ld seconds) fast forward:\n", LONG_MAX);
	while (1) {
		tvx.tv_sec += 10*60*60;
		tm0 = gmtime(&tvx.tv_sec);
		printf("\r%dy%dm%dd%02ld.%03lu               ",
			tm0->tm_year-70, tm0->tm_mon, tm0->tm_mday-1,
			tm0->tm_hour, tm0->tm_min, tm0->tm_sec, tvx.tv_usec);
		if (tvx.tv_sec >= LONG_MAX-10*60*60 || tvx.tv_sec < 0) {
			printf("\n...reached LONG_MAX, could have been >1000 years with 64bit long int\n");
			break;
		}
		else if (tm0->tm_year-70 >= 500) {
			printf("\n...stopping at 500 years\n");
			sleep(3);
			break;
		}
	}

	printf("timersub():\n");
	struct timeval tvy, tvd;
	tvx.tv_sec = 0;
	tvx.tv_usec = 0;
	tvy.tv_sec = 0;
	tvy.tv_usec = 0;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	// So I've been told that timersub() doesn't do negatives, so remove those
	/* tvx.tv_sec = 0;
	tvx.tv_usec = 0;
	tvy.tv_sec = 1;
	tvy.tv_usec = 0;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	*/
	tvx.tv_sec = 0;
	tvx.tv_usec = 1;
	tvy.tv_sec = 0;
	tvy.tv_usec = 1;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 1;
	tvx.tv_usec = 0;
	tvy.tv_sec = 0;
	tvy.tv_usec = 1;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 1;
	tvx.tv_usec = 1;
	tvy.tv_sec = 0;
	tvy.tv_usec = 2;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	
	// So I've been told that timersub() doesn't do negatives, so remove those
	/*tvx.tv_sec = 0;
	tvx.tv_usec = 0;
	tvy.tv_sec = 0;
	tvy.tv_usec = 1;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 10;
	tvx.tv_usec = 0;
	tvy.tv_sec = 10;
	tvy.tv_usec = 1;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 10;
	tvx.tv_usec = 0;
	tvy.tv_sec = 11;
	tvy.tv_usec = 0;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 10;
	tvx.tv_usec = 1;
	tvy.tv_sec = 11;
	tvy.tv_usec = 0;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 10;
	tvx.tv_usec = 1;
	tvy.tv_sec = 11;
	tvy.tv_usec = 1;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 10;
	tvx.tv_usec = 1;
	tvy.tv_sec = 11;
	tvy.tv_usec = 2;
	timersub(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	*/

	printf("timeradd():\n");
	tvx.tv_sec = 0;
	tvx.tv_usec = 0;
	tvy.tv_sec = 0;
	tvy.tv_usec = 0;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 0;
	tvx.tv_usec = 1;
	tvy.tv_sec = 0;
	tvy.tv_usec = 1;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 1;
	tvx.tv_usec = 0;
	tvy.tv_sec = 0;
	tvy.tv_usec = 999999;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 1;
	tvx.tv_usec = 0;
	tvy.tv_sec = 0;
	tvy.tv_usec = 1;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 1;
	tvx.tv_usec = 999999;
	tvy.tv_sec = 0;
	tvy.tv_usec = 1;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 9;
	tvx.tv_usec = 1;
	tvy.tv_sec = 0;
	tvy.tv_usec = 999999;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 10;
	tvx.tv_usec = 0;
	tvy.tv_sec = 11;
	tvy.tv_usec = 999999;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 10;
	tvx.tv_usec = 1;
	tvy.tv_sec = 20;
	tvy.tv_usec = 999999;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);
	tvx.tv_sec = 10;
	tvx.tv_usec = 999999;
	tvy.tv_sec = 20;
	tvy.tv_usec = 2;
	timeradd(&tvx, &tvy, &tvd);
	printf("  tvx:%0ld.%06ld tvy:%0ld.%06ld tvd:%0ld.%06ld\n",
		tvx.tv_sec, tvx.tv_usec, tvy.tv_sec, tvy.tv_usec, tvd.tv_sec, tvd.tv_usec);

	gettimeofday(&tvx, NULL);
	gettimeofday(&tvy, NULL);
	timersub(&tvy, &tvx, &tvd);
	printf("Time to execute gettimeofday(): %ld.%06ld\n", tvd.tv_sec, tvd.tv_usec);

	initscr();
	gettimeofday(&tvx, NULL);
	TimeReset();
	gettimeofday(&tvy, NULL);
	endwin();
	timersub(&tvy, &tvx, &tvd);
	printf("Time to execute TimeReset(): %ld.%06ld\n", tvd.tv_sec, tvd.tv_usec);

	printf("\n");
}

