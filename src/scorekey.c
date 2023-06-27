#include "scorekey.h"

#include <stddef.h>
#include <string.h>
#include "pd_api.h"

unsigned char codekey[] = { 0x01, 0x00};

void obfuscateCodeKey(PlaydateAPI* pd, unsigned char* codein, unsigned char** codeout)
{
	size_t len = strlen(codein);
	char* tmp = pd->system->realloc(NULL, len + 1);
	strcpy(tmp, codein);
	*codeout = tmp;
}

void deObfuscateCodeKey(PlaydateAPI* pd, unsigned char* codein, unsigned char** codeout)
{
	size_t len = strlen(codein);
	char* tmp = pd->system->realloc(NULL, len + 1);
	strcpy(tmp, codein);
	*codeout = tmp;
}
