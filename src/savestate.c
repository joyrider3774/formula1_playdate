#include <string.h>
#include <stdbool.h>
#include "commonvars.h"


char header[16];
unsigned int check;
unsigned int saveTime;
int hiScore;
int valid;

unsigned int encrypt(unsigned int filepos, unsigned int time, void* data, unsigned int size, bool returnOriginalPos)
{
    #define keySize 21
    const unsigned char key[keySize] = { 0x58, 0x030, 0x49, 0x53, 0x67, 0x68, 0x68, 0x41, 0x4E, 0x36, 0x78, 0x69, 0x75, 0x33, 0x70, 0x58, 0x42, 0x78, 0x76, 0x46, 0x5C };
    for (unsigned i = 0; i < size; i++)
    {
        *((unsigned char*)(data)+i) ^= key[(filepos + i + time) % keySize];
    }

    if (returnOriginalPos)
    {
        return filepos;
    }
    else
    {
        return filepos + size;
    }
}

void loadSaveState(void)
{
    SDFile* saveStateFile = pd->file->open("savestate.srm", kFileReadData);
    //does not exist
    if (saveStateFile == NULL)
        return;

    int ret = pd->file->read(saveStateFile, &saveTime, sizeof(saveTime));
    if (ret == -1)
    {
        valid = 0;
        pd->system->error("Error reading header from savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    encrypt(0, 0, &saveTime, sizeof(saveTime), false);

    ret = pd->file->read(saveStateFile, header, sizeof(header));
    if (ret == -1)
    {
        valid = 0;
        pd->system->error("Error reading header from savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    unsigned int pos = encrypt(0, saveTime, header, sizeof(header), false);

    ret = pd->file->read(saveStateFile, &check, sizeof(check));
    if (ret == -1)
    {
        valid = 0;
        pd->system->error("Error reading hiscore from savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    pos = encrypt(pos, saveTime, &check, sizeof(check), false);

    ret = pd->file->read(saveStateFile, &hiScore, sizeof(hiScore));
    if (ret == -1)
    {
        valid = 0;
        pd->system->error("Error reading hiscore from savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    pos = encrypt(pos, saveTime, &hiScore, sizeof(hiScore), false);

    ret = pd->file->close(saveStateFile);
    if (ret == -1)
    {
        valid = 0;
        pd->system->error("Error closing savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
}

void saveSaveState(void)
{
   
    valid = 1;
    check = (hiScore % 97);

    SDFile* saveStateFile = pd->file->open("savestate.srm", kFileWrite);
    if (saveStateFile == NULL)
    {
        pd->system->error("Couldn’t open savestate file for writing!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    
    saveTime = pd->system->getCurrentTimeMilliseconds();

    encrypt(0, 0, &saveTime, sizeof(saveTime), true);
    int ret = pd->file->write(saveStateFile, &saveTime, sizeof(saveTime));
    if (ret == -1)
    {
        encrypt(0, 0, &saveTime, sizeof(saveTime), true);
        pd->system->error("Error writing header to savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    encrypt(0, 0, &saveTime, sizeof(saveTime), true);

    unsigned int pos = encrypt(0, saveTime, header, sizeof(header), true);
    ret = pd->file->write(saveStateFile, header, sizeof(header));
    if (ret == -1)
    {

        pos = encrypt(0, saveTime, header, sizeof(header), false);
        pd->system->error("Error writing header to savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    pos = encrypt(0, saveTime, header, sizeof(header), false);

    pos = encrypt(pos, saveTime, &check, sizeof(check), true);
    ret = pd->file->write(saveStateFile, &check, sizeof(check));
    if (ret == -1)
    {

        pos = encrypt(pos, saveTime, &check, sizeof(check), false);
        pd->system->error("Error writing header to savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    pos = encrypt(pos, saveTime, &check, sizeof(check), false);

    pos = encrypt(pos, saveTime, &hiScore, sizeof(hiScore), true);
    ret = pd->file->write(saveStateFile, &hiScore, sizeof(hiScore));
    if (ret == -1)
    {
        pos = encrypt(pos, saveTime, &hiScore, sizeof(hiScore), false);
        pd->system->error("Error writing hiScore to savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
    pos = encrypt(pos, saveTime, &hiScore, sizeof(hiScore), false);

    //crashes the simulator ???
#ifndef _WIN32
    ret = pd->file->flush(saveStateFile);
    if (ret == -1)
    {
        pd->system->error("Error flushing savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
#endif

    ret = pd->file->close(saveStateFile);
    if (ret == -1)
    {
        pd->system->error("Error closing savesate file!");
        pd->system->logToConsole(pd->file->geterr());
        return;
    }
}

void validateSaveState(void)
{
    if ((hiScore % 97) != check)
    {
        hiScore = 0;
        valid = 0;
    }

    if ((strcmp(header, "formula1_001") != 0) || !valid)
    {
        valid = 0;        
        strcpy(header, "formula1_001");
    }

    if (!valid)
    {
        saveSaveState();
    }
}

void initSaveState(void)
{
    valid = 1;
    hiScore = 0;
    check = 1;
    loadSaveState();
    validateSaveState();
}

int getHiScore(void)
{
    return hiScore;
}

void setHiScore(int value)
{
    hiScore = value;
    saveSaveState();
}

