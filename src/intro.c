 #include <stdint.h>

#include "commonvars.h"
#include "pd_api.h"
#include "printfuncs.h"
#include "intro.h"

#define FRAMEDELAY 60

int frames = 0;
int ay;

void initIntro(void)
{
    ay = pd->display->getHeight();
}

void intro(void)
{
    if (gameState == gsInitIntro)
    {
        initIntro();
        gameState -= gsInitDiff;
    }
    
    frames++;
    pd->graphics->clear(kColorBlack);
    if (frames < FRAMEDELAY)
    {
        printMessage(introFont, (14 - 12) >> 1, 4, 24, "WILLEMS DAVY");
    }
    else
    {
        if (frames < FRAMEDELAY *2)
        {
            printMessage(introFont, (14-8) >> 1, 4, 24, "PRESENTS");
        }
        else
        {
            if(ay > 0)
            {
                ay -= 10;
            }
            else
            {
                gameState = gsInitGameIntro;
            }
        }
    }
        
    if (((currButtons & kButtonA) && (!(prevButtons & kButtonA))) ||
        ((currButtons & kButtonB) && (!(prevButtons & kButtonB))))
    {            
        gameState = gsInitGameIntro;
    }

    if (currButtons & kButtonUp)
    {
        debugMode = 1;
    }
}
