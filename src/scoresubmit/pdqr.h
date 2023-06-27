#ifndef PDQR_H
#define PDQR_H

#include "pd_api.h"

LCDBitmap* CreateQrCode(PlaydateAPI* pd, const char* value, int desiredSize);

#endif