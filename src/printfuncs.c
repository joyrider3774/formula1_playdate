#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "commonvars.h"
#include "printfuncs.h"

// function provided by @Pharap
// If the array is not big enough,
int formatInteger(char *array, int integer)
{
    int maxDigits = 10;
    int maxCharacters = (maxDigits + 1);
    int lastIndex = (maxCharacters - 1);

    array[lastIndex] = '\0';

    if (integer == 0)
    {
        array[lastIndex - 1] = '0';
        return 1;
    }

    int digits = 0;

    do
    {
        int digit = integer % 10;
        integer /= 10;

        ++digits;

        array[lastIndex - digits] = ('0' + digit);
    } while (integer > 0);

    return digits;
};

//print a number on levelselect or game screen
void printNumber(LCDBitmapTable* fontBitmapTable, int ax, int ay, int tileSize, int aNumber, int maxDigits)
{
    const int buffSize = 10;
    char number[10 + 1]; //buffsize + 1
    int digits = formatInteger(number, aNumber);
    int maxFor = digits;
    if (digits > maxDigits)
        maxFor = maxDigits;
    for (int c = 0; c < maxFor; c++)
    {
        if (number[buffSize - digits + c] == '\0')
            return;
        
        LCDBitmap* bitmap = pd->graphics->getTableBitmap(fontBitmapTable, number[buffSize - digits + c] - 47);
        if (bitmap)
        {
            pd->graphics->drawBitmap(bitmap, (ax + (maxDigits - digits) + c) * tileSize, ay * tileSize, kBitmapUnflipped);
        }
    }
}

//print a message on the title screen on ax,ay, the tileset from titlescreen contains an alphabet
void printMessage(LCDBitmapTable* fontBitmapTable, int ax, int ay, int tileSize, const char* amsg)
{
    // based on input from @Pharap
    const char *p = amsg;
    int index = 0;

    while (1)
    {
        char fChar = *(p++);
        int tile = 61;
        switch (fChar)
        {
        case '\0':
            return;

        case ':':
            tile = 37;
            break;

        case 'a':
            tile = 40;
            break;

        case 'b':
            tile = 38;
            break;

        default:
            if ((fChar >= 'A') && (fChar <= 'Z'))
                tile = fChar - 54;

            if ((fChar >= '0') && (fChar <= '9'))
                tile = fChar - 47;
            break;
        }
        LCDBitmap* bitmap = pd->graphics->getTableBitmap(fontBitmapTable, tile);
        if (bitmap)
        {
            pd->graphics->drawBitmap(bitmap, (ax + index) * tileSize, ay * tileSize, kBitmapUnflipped);
        }
        ++index;
    }
}
