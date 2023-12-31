#include <stdlib.h>
#include <stdio.h>
#include "pd_api.h"
#include "crank.h"
#include "maingameloop.h"
#include "commonvars.h"
#include "intro.h"
#include "sound.h"
#include "game.h"
#include "savestate.h"

LCDBitmap* loadImageAtPath(const char* path)
{
    const char* outErr = NULL;
    LCDBitmap* img = pd->graphics->loadBitmap(path, &outErr);
    if (outErr != NULL) {
        pd->system->error("Error loading image at path '%s': %s", path, outErr);
        pd->system->logToConsole("Error loading image at path '%s': %s", path, outErr);
    }
    return img;
}

LCDBitmapTable* loadBitmapTableAtPath(const char* path)
{
    const char* outErr = NULL;
    LCDBitmapTable* table = pd->graphics->loadBitmapTable(path, &outErr);
    if (outErr != NULL) {
        pd->system->error("Error loading image at path '%s': %s", path, outErr);
        pd->system->logToConsole("Error loading image at path '%s': %s", path, outErr);
    }
    return table;
}

void preloadImages(void)
{
    introFont = loadBitmapTableAtPath("images/bigfont");
    lcdFont = loadBitmapTableAtPath("images/lcdfont");
    Player = loadImageAtPath("images/player");
    Enemy = loadImageAtPath("images/enemy");
    Background = loadImageAtPath("images/background");
}

void MenuItemCallback(void* userdata)
{
    setHiScore(0);
}

void resetGlobals()
{
	frames = 0;
	for (int X = 0; X < 3; X++)
		for (int Y = 0; Y < 3; Y++)
			EnemyStates[X][Y] = false;
	for (int X = 0; X < 3; X++)
		PlayerStates[X] = false;
	PlayerStates[1] = false;
}

// game initialization
void setupGame(void)
{  
    resetGlobals();
	pd->graphics->setBackgroundColor(kColorBlack);
	gameState = gsInitIntro;
    setCrankMoveThreshold(70);
    preloadImages();    
    initSaveState();    
    initSound();
    setSoundOn(1);
    resetHiScoreMenuItem = pd->system->addMenuItem("Reset HiScore", MenuItemCallback, NULL);    
}

// main update function
int mainLoop(void* ud)
{
    int result = 1;
    prevButtons = currButtons;
    pd->system->getButtonState(&currButtons, NULL, NULL);

    //gamestate handling   
    switch (gameState)
    {
        case gsInitIntro:
        case gsIntro:
            intro();
            break;
        case gsInitGame:
        case gsGame:
            game();
            break;
        case gsInitGameIntro:
        case gsGameIntro:
            gameIntro();
            break;
        case gsInitGameOver:
        case gsGameOver:
            gameOver();
            break;
        default:
            break;
    }
    if (debugMode)
    {
        pd->system->drawFPS(0, 0);
    }
	return result;
}
