#ifndef SCOREKEY_H
#define SCOREKEY_H

#include "pd_api.h"

extern unsigned char codekey[];

void obfuscateCodeKey(PlaydateAPI* pd, unsigned char* codein, unsigned char** codeout);
void deObfuscateCodeKey(PlaydateAPI* pd, unsigned char* codein, unsigned char** codeout);

#endif