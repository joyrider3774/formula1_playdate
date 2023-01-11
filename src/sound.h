#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>
#include "pd_api.h"

void initSound(void);
void setSoundOn(int value);
int isSoundOn(void);
void playTickSound(void);
void playCrashSound(void);
FilePlayer* loadSoundFile(const char* path);

#endif