#include <string.h>
#include "pd_api.h"
#include "pdqr.h"
#include "base64.h"
#include "hmacsha1.h"

unsigned char* CreateVerifierCode(PlaydateAPI* pd, char* secret_key, unsigned int gameId, unsigned int score)
{
	unsigned char* verifycode;
	char* verifycodemsg;

	pd->system->formatString(&verifycodemsg, "%u%u%s", score, gameId, secret_key);
	uint8_t hashedverifycode[SHA1_HASH_SIZE];
	hmac_sha1(secret_key, strlen(secret_key) * sizeof(char), verifycodemsg, strlen(verifycodemsg), hashedverifycode, SHA1_HASH_SIZE);
	verifycode = base64_encode(pd, (unsigned char*)hashedverifycode, SHA1_HASH_SIZE, NULL);
	pd->system->realloc(verifycodemsg, 0);
	return verifycode;
}

LCDBitmap* CreateQrCodeSubmit(PlaydateAPI* pd, char* secret_key, unsigned int gameId, unsigned int score, int DesiredQrCodeSize, unsigned char** verifyCodeOut)
{
	//unsigned int t = pd->system->getCurrentTimeMilliseconds();
	char* qrsubmit;
	unsigned char * verifyCode = CreateVerifierCode(pd, secret_key, gameId, score);
	pd->system->formatString(&qrsubmit, "https://scores.joyrider3774.xyz/AddScoreVerify.php?game=%d&score=%u&verify=%s", gameId, score, verifyCode);
	LCDBitmap* result = CreateQrCode(pd, qrsubmit, DesiredQrCodeSize);
	if (verifyCodeOut)
		*verifyCodeOut = verifyCode;
	else
		pd->system->realloc(verifyCode, 0);
	pd->system->realloc(qrsubmit, 0);
	//pd->system->logToConsole("%u", pd->system->getCurrentTimeMilliseconds() - t);
	return result;
}
