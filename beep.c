#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include "chrono.h"

#define BEEP_BUFFER_SIZE 4096
char beep_buffer[BEEP_BUFFER_SIZE];

void BeepMakeBuffer(unsigned int freq) {
	unsigned int wavelen = 44100 / freq;
	int cnt;
	int x;
	int i;
	int j;
	for (cnt = 0, x = 127; cnt < BEEP_BUFFER_SIZE; cnt += wavelen) {
		if (cnt >= BEEP_BUFFER_SIZE) break;
		// wave up
		for (j = 0, i = 128; i <= 255; i++, j++) {
			beep_buffer[cnt+j] = x;
			x += 2;
		}
		// wave down
		for (i = 255; i >= 0; i--, j++) {
			beep_buffer[cnt+j] = x;
			x -= 2;
		}
		// wave up
		for (i = 0; i <= 127; i++, j++) {
			beep_buffer[cnt+j] = x;
			x += 2;
		}
	}
}

void BeepPlay(unsigned int freq, unsigned int length) {
	BeepMakeBuffer(freq);
	SDL_PauseAudio(0);
	usleep(length*1000);
	SDL_PauseAudio(1);
}

void BeepCallbackFunc(void *userdata, Uint8 *stream, int len) {
	snprintf((char *)stream, len, "%s", beep_buffer);
}

void *BeepThreadFunc(void *argp) {
	while (beep_enabled) {
		if (beep_trigger) {
			beep_trigger = 0;
			memset(beep_buffer, '#', 4096);
			BeepPlay(440, 125);
			BeepPlay(880, 125);
			BeepPlay(512, 125);
			BeepPlay(682, 125);
		}
		else
			usleep(100000);
	}
	
	return NULL;
}

void BeepInit(void) {
	memset(beep_buffer, '#', BEEP_BUFFER_SIZE);

	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec desired;
	SDL_AudioSpec obtained;
	desired.freq = 44100;
	desired.format = AUDIO_U8;
	desired.channels = 1;
	desired.samples = 4096;
	desired.callback = BeepCallbackFunc;
	SDL_OpenAudio(&desired, &obtained);

	pthread_t thr;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&thr, &attr, BeepThreadFunc, NULL);
	pthread_detach(thr);
	pthread_attr_destroy(&attr);
}

