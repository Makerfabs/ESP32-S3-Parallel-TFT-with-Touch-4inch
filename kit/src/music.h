#ifndef MUSIC_H
#define MUSIC_H

#include "config.h"

extern Audio audio;

void music_init();
void music_mute();
void Task_Audio(void *pvParameters);

int get_music_list(fs::FS &fs, const char *dirname, String *list, int length);
void open_new_song(String filename);
void audio_eof_mp3(const char *info);
void next_song();

#endif