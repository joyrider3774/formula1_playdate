#ifndef commonvars_h
#define commonvars_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "pd_api.h"

#define FRAMERATE 50

#define gsGame 0
#define gsIntro 1
#define gsGameIntro 2
#define gsGameOver 3

#define gsInitDiff 50

#define gsInitGame (gsInitDiff + gsGame)
#define gsInitIntro (gsInitDiff + gsIntro)
#define gsInitGameIntro (gsInitDiff + gsGameIntro)
#define gsInitGameOver (gsInitDiff + gsGameOver)

extern int gameState, debugMode;
extern bool EnemyStates[3][3];
extern bool PlayerStates[3];
extern int HitPosition, LivesLost;
extern int Teller, FlashesDelay, Flashes, Delay, Score;
extern bool CanMove, CrashSoundPlayed;

extern PDMenuItem* resetHiScoreMenuItem;
extern PlaydateAPI *pd;
extern PDButtons currButtons, prevButtons;
extern LCDBitmap *Player, *Enemy, *Background;
extern LCDBitmapTable* introFont, *lcdFont;

void setPDPtr(PlaydateAPI* p);

#endif