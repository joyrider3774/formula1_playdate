#ifndef SCOREKEY_H
#define SCOREKEY_H

#include "pd_api.h"

extern char codekey[];

void obfuscateCodeKey(PlaydateAPI* pd, char* codein, char** codeout);
void deObfuscateCodeKey(PlaydateAPI* pd, char* codein, char** codeout);

#endif