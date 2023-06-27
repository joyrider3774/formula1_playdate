#include "scorekey.h"

#include <stddef.h>
#include <string.h>
#include "pd_api.h"

//this is a stub i did not commit the real code to prevent the key to be easily reversed

char codekey[] = { 0x01, 0x00};

void obfuscateCodeKey(PlaydateAPI* pd, char* codein, char** codeout)
{
	size_t len = strlen(codein);
	char* tmp = pd->system->realloc(NULL, len + 1);
	strcpy(tmp, codein);
	*codeout = tmp;
}

void deObfuscateCodeKey(PlaydateAPI* pd, char* codein, char** codeout)
{
	size_t len = strlen(codein);
	char* tmp = pd->system->realloc(NULL, len + 1);
	strcpy(tmp, codein);
	*codeout = tmp;
}
