
#include <time.h>

#include "commonvars.h"
#include "sound.h"
#include "printfuncs.h"
#include "savestate.h"
#include "crank.h"
#include "scoresubmit/scoresubmit.h"
#include "pd_api.h"

void DrawGame(void)
{
	int X, Y;
	for (X = 0; X < 3; X++)
		for (Y = 0; Y < 3; Y++)
			if (EnemyStates[X][Y])
			{
				pd->graphics->drawBitmap(Enemy, 90 + (X * 54), 48 + (Y * 47), kBitmapUnflipped);
			}

	for (X = 0; X < 3; X++)
	{
		if (PlayerStates[X])
		{
			pd->graphics->drawBitmap(Player, 90 + (X * 54), 180, kBitmapUnflipped);
		}
	}
}

void MoveEnemy(void)
{
	int X, Y;
	for (X = 0; X < 3; X++)
		for (Y = 2; Y >= 1; Y--)
			EnemyStates[X][Y] = EnemyStates[X][Y - 1];
	for (X = 0; X < 3; X++)
		EnemyStates[X][0] = false;
	for (X = 0; X <= 1; X++)
		EnemyStates[rand() % 3][0] = true;
}

void MoveLeft(void)
{
	int X;
	for (X = 0; X < 2; X++)
	{
		if (PlayerStates[X + 1])
		{
			PlayerStates[X] = true;
			PlayerStates[X + 1] = false;
		}
	}
}

void MoveRight(void)
{
	int X;
	for (X = 2; X >= 1; X--)
	{
		if (PlayerStates[X - 1])
		{
			PlayerStates[X] = true;
			PlayerStates[X - 1] = false;
		}
	}
}

bool IsCollided(void)
{
	int X;
	bool Temp;
	Temp = false;
	for (X = 0; X < 3; X++)
		if (PlayerStates[X] && EnemyStates[X][2])
		{
			Temp = true;
			HitPosition = X;
		}
	return Temp;
}

void HitFlash(void)
{
	PlayerStates[HitPosition] = !PlayerStates[HitPosition];
	EnemyStates[HitPosition][2] = !EnemyStates[HitPosition][2];
}

void InitialiseStates(void)
{
	int X, Y;
	for (X = 0; X < 3; X++)
		for (Y = 0; Y < 3; Y++)
			EnemyStates[X][Y] = false;
	for (X = 0; X < 3; X++)
		PlayerStates[X] = false;
	PlayerStates[1] = true;
}

void initGame(void)
{
	srand(pd->system->getCurrentTimeMilliseconds());
	Teller = 25;
	FlashesDelay = 14;
	Flashes = 0;
	CanMove = true;
	Score = 0;
	Delay = 60;
	LivesLost = 0;
	CrashSoundPlayed = false;
	InitialiseStates();
}


void game(void)
{
	int X;
	pd->graphics->clear(kColorBlack);
    if (gameState == gsInitGame)
    {
        initGame();
        gameState -= gsInitDiff;
    }
    
    switch (crankUpdate())
    {
        case CRANKMOVERIGHT:
			if (CanMove)
				MoveRight();
            break;
        case CRANKMOVELEFT:
			if (CanMove)
				MoveLeft();
            break;
        default:
            break;
    }
    
    if ((currButtons & kButtonRight) && (!(prevButtons & kButtonRight)))
    {
		if (CanMove)
			MoveRight();
    }

    if ((currButtons & kButtonLeft) && (!(prevButtons & kButtonLeft)))
    {
		if (CanMove)
			MoveLeft();
    }

	pd->graphics->drawBitmap(Background, 0, 0, kBitmapUnflipped);
	printNumber(lcdFont, 24, 4, 8, Score, 6);
	printNumber(lcdFont, 17, 4, 8, getHiScore(), 6);
	if (LivesLost >= 1)
	{
		for (X = 0; X < LivesLost; X++)
			printMessage(lcdFont, 11 + X, 4, 8, "X");
	}
	Teller++;
	if (Teller >= Delay)
	{
		if (!IsCollided() && CanMove)
		{
			Teller = 0;
			for (X = 0; X < 3; X++)
				if (EnemyStates[X][2])
				{
					Score += 10;
					if (Score > getHiScore())
						setHiScore(Score);
					if ((Score % 100 == 0) && (Delay > 16))
						Delay--;
				}
			MoveEnemy();
			playTickSound();
		}
		else
		{
			if (!CrashSoundPlayed)
			{
				playCrashSound();
				CrashSoundPlayed = true;
			}
			CanMove = false;
			FlashesDelay++;
			if (FlashesDelay == 15)
			{
				Flashes++;
				HitFlash();
				FlashesDelay = 0;
				if (Flashes == 6)
				{
					Flashes = 0;
					CanMove = true;
					Teller = 0;
					CrashSoundPlayed = false;
					EnemyStates[HitPosition][2] = false;
					LivesLost++;
					FlashesDelay = 14;
					if (LivesLost == 3)
						gameState = gsInitGameOver;
				}
			}
		}
	}
	DrawGame();
}

void initGameOver(void)
{
	if (QRCode)
		pd->graphics->freeBitmap(QRCode);
	if (verifyCode)
		pd->system->realloc(verifyCode, 0);
#ifdef FORMULA1_PLAYDATE_CODEKEY
	char codekey[] = {FORMULA1_PLAYDATE_CODEKEY};
#else
	char codekey[] = "SECRETKEY";
#endif
	QRCode = CreateQrCodeSubmit(pd, codekey, 1, Score, 205, &verifyCode);
	pd->system->logToConsole("score: %u verifier:%s", Score, verifyCode);
}

void gameOver(void)
{
	int X;
	pd->graphics->clear(kColorBlack);
	if (gameState == gsInitGameOver)
	{
		initGameOver();
		gameState -= gsInitDiff;
	}

	switch (crankUpdate())
	{
		case CRANKMOVERIGHT:
		case CRANKMOVELEFT:
			gameState = gsInitGame;
			break;
		default:
			break;
	}

	if (((currButtons & kButtonRight) && (!(prevButtons & kButtonRight))) ||
		((currButtons & kButtonLeft) && (!(prevButtons & kButtonLeft))) ||
		((currButtons & kButtonA) && (!(prevButtons & kButtonA))) ||
		((currButtons & kButtonB) && (!(prevButtons & kButtonB))))
	{
		gameState = gsInitGame;
	}


	pd->graphics->drawBitmap(Background, 0, 0, kBitmapUnflipped);
	printNumber(lcdFont, 24, 4, 8, Score, 6);
	printNumber(lcdFont, 17, 4, 8, getHiScore(), 6);
	if (LivesLost >= 1)
	{
		for (X = 0; X < LivesLost; X++)
			printMessage(lcdFont, 11 + X, 4, 8, "X");
		DrawGame();		
	}
	
	if (QRCode && (Score == getHiScore()))
	{
		pd->graphics->fillRect(20, 0, 280, 240, kColorBlack);
		pd->graphics->fillRect(21, 1, 278, 239, kColorWhite);
		int tw = pd->graphics->getTextWidth(NULL, "SUBMIT YOUR HI-SCORE ONLINE", strlen("SUBMIT YOUR HI-SCORE ONLINE"), kASCIIEncoding, 0);
		pd->graphics->drawText("SUBMIT YOUR HI-SCORE ONLINE", strlen("SUBMIT YOUR HI-SCORE ONLINE"), kASCIIEncoding, (320 - tw) >> 1, 2);
		char* msg;
		pd->system->formatString(&msg, "%d", Score);
		tw = pd->graphics->getTextWidth(NULL, msg, strlen(msg), kASCIIEncoding, 0);
		pd->graphics->drawText(msg, strlen(msg), kASCIIEncoding, (320 - tw) >> 1, 17);
		pd->system->realloc(msg, 0);
		if (QRCode)
		{
			int w, h;
			pd->graphics->getBitmapData(QRCode, &w, &h, NULL, NULL, NULL);
			//-80 from drawoffset set
			pd->graphics->drawBitmap(QRCode, (320 - w) >> 1, 33, kBitmapUnflipped);
		}
	}

}

void FlashIntro(void)
{
	int X, Y;
	for (X = 0; X < 3; X++)
		for (Y = 0; Y < 3; Y++)
			EnemyStates[X][Y] = !EnemyStates[X][Y];
	for (X = 0; X < 3; X++)
		PlayerStates[X] = !PlayerStates[X];
}


void gameIntroInit(void)
{
	LivesLost = 0;
	FlashesDelay = 0;
	Score = 0;
}

void gameIntro(void)
{
	int X;
	pd->graphics->clear(kColorBlack);
	if (gameState == gsInitGameIntro)
	{
		gameIntroInit();
		gameState -= gsInitDiff;
	}

	switch (crankUpdate())
	{
		case CRANKMOVERIGHT:
		case CRANKMOVELEFT:
			gameState = gsInitGame;
			break;
		default:
			break;
	}

	if (((currButtons & kButtonRight) && (!(prevButtons & kButtonRight))) ||
		((currButtons & kButtonLeft) && (!(prevButtons & kButtonLeft))) ||
		((currButtons & kButtonA) && (!(prevButtons & kButtonA))) ||
		((currButtons & kButtonB) && (!(prevButtons & kButtonB))))
	{
		gameState = gsInitGame;
	}

	pd->graphics->drawBitmap(Background, 0, 0, kBitmapUnflipped);
	FlashesDelay++;
	if (FlashesDelay == 25)
	{
		FlashesDelay = 0;
		FlashIntro();
		if (PlayerStates[0])
		{
			LivesLost = 3;
			Score = 888888;
		}
		else
		{
			LivesLost = 0;
			Score = 0;
		}
	}
	if (Score > 0)
	{
		printNumber(lcdFont, 24, 4, 8, Score, 6);
		printNumber(lcdFont, 17, 4, 8, Score, 6);
	}
	if (LivesLost >= 1)
	{
		for (X = 0; X < LivesLost; X++)
			printMessage(lcdFont, 11 + X, 4, 8, "X");
	}
	DrawGame();		
}

