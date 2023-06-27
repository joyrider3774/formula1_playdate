#ifndef SCORESUBMIT_H
#define SCORESUBMIT_H

#include "pd_api.h"

LCDBitmap* CreateQrCodeSubmit(PlaydateAPI* pd, char* secret_key, unsigned int gameId, unsigned int score, int DesiredQrCodeSize, char** verifyCodeOut);
char* CreateVerifierCode(PlaydateAPI* pd, char* secret_key, unsigned int gameId, unsigned int score);

#endif