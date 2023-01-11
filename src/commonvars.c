#include "commonvars.h"
#include <stdbool.h>
#include "pd_api.h"

int gameState, debugMode;

PDMenuItem* resetHiScoreMenuItem;
PlaydateAPI* pd;
PDButtons currButtons, prevButtons;
LCDBitmap* Player, * Enemy, * Background;
LCDBitmapTable* introFont, * lcdFont;

bool EnemyStates[3][3];
bool PlayerStates[3];
int HitPosition, LivesLost;
int Teller, FlashesDelay, Flashes, Delay, Score;
bool CanMove, CrashSoundPlayed;

void setPDPtr(PlaydateAPI* p) {
    pd = p;
}
