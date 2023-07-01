#include <stdbool.h>
#include <stdint.h>
#include "qrcodegen.h"
#include "pd_api.h"
#include "pdqr.h"

LCDBitmap* CreateQrCode(PlaydateAPI* pd, const char* value, int desiredSize)
{
    LCDBitmap* Result = NULL;
    // Text data
    uint8_t qr0[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(value, tempBuffer, qr0, qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (ok)
    {
        
        int size = qrcodegen_getSize(qr0);
        int blocksize = desiredSize / size;
        int realsize = size * blocksize;
        Result = pd->graphics->newBitmap(realsize, realsize, kColorWhite);
        pd->graphics->pushContext(Result);
        pd->graphics->clearClipRect();
        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                if (qrcodegen_getModule(qr0, x, y))
                {
                    pd->graphics->fillRect((x * blocksize),(y * blocksize), blocksize, blocksize, kColorBlack);
                }
                else
                {
                    //pd->graphics->fillRect(x * blocksize, y * blocksize, blocksize, blocksize, kColorWhite);
                }
            }
        }
        pd->graphics->popContext();
    }
    return Result;
}

