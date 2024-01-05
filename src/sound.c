#include <stdint.h>
#include <string.h>
#include "pd_api.h"
#include "commonvars.h"
#include "sound.h"

int sound_on = 0;

FilePlayer* tickSound;
FilePlayer* crashSound;

void setSoundOn(int value)
{
    sound_on = value;
}


int isSoundOn(void)
{
    return sound_on;
}

FilePlayer* loadSoundFile(const char* path)
{
    FilePlayer* soundPlayer = pd->sound->fileplayer->newPlayer();
    pd->sound->fileplayer->setStopOnUnderrun(soundPlayer, 1);
    pd->sound->fileplayer->setVolume(soundPlayer, 0.7f, 0.7f);
    pd->sound->fileplayer->setRate(soundPlayer, 1.0f);
    pd->sound->fileplayer->loadIntoPlayer(soundPlayer, path);
    return soundPlayer;
}

void initSound(void)
{
    tickSound = loadSoundFile("sound/tick");
    crashSound = loadSoundFile("sound/crash");
}

void deinitSound(void)
{
	pd->sound->fileplayer->freePlayer(tickSound);
	pd->sound->fileplayer->freePlayer(crashSound);
}

void playSound(FilePlayer* soundPlayer)
{
    if (pd->sound->fileplayer->isPlaying(soundPlayer))
    {
        pd->sound->fileplayer->stop(soundPlayer);
    }
    pd->sound->fileplayer->play(soundPlayer, 1);   
}

void playTickSound(void)
{
    if(!sound_on)
    {
        return;
    }
    playSound(tickSound);
}

void playCrashSound(void)
{
    if(!sound_on)
    {
        return;
    }
    playSound(crashSound);
}
