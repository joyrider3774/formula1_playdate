#include <string.h>
#include "pd_api.h"
#include "pdqr.h"
#include "base64.h"
#include "hmacsha1.h"

//https://gist.github.com/jesobreira/4ba48d1699b7527a4a514bfa1d70f61a
char* urlencode(PlaydateAPI* pd, char* originalText)
{
	// allocate memory for the worst possible case (all characters need to be encoded)
	char* encodedText = (char*) pd->system->realloc(NULL, (sizeof(char) * strlen(originalText) * 3 + 1));

	const char* hex = "0123456789abcdef";

	size_t pos = 0;
	for (size_t i = 0; i < strlen(originalText); i++) {
		if (('a' <= originalText[i] && originalText[i] <= 'z')
			|| ('A' <= originalText[i] && originalText[i] <= 'Z')
			|| ('0' <= originalText[i] && originalText[i] <= '9')) {
			encodedText[pos++] = originalText[i];
		}
		else {
			encodedText[pos++] = '%';
			encodedText[pos++] = hex[originalText[i] >> 4];
			encodedText[pos++] = hex[originalText[i] & 15];
		}
	}
	encodedText[pos] = '\0';
	return encodedText;
}

char* CreateVerifierCode(PlaydateAPI* pd, char* secret_key, unsigned int gameId, unsigned int score)
{
	char* verifycode;
	char* verifycodemsg;

	pd->system->formatString(&verifycodemsg, "%u%u%s", score, gameId, secret_key);
	unsigned char hashedverifycode[SHA1_HASH_SIZE];
	hmac_sha1(secret_key, strlen(secret_key) * sizeof(char), verifycodemsg, strlen(verifycodemsg), hashedverifycode, SHA1_HASH_SIZE);
	verifycode = (char *) base64_encode(pd, hashedverifycode, SHA1_HASH_SIZE, NULL);
	pd->system->realloc(verifycodemsg, 0);
	return verifycode;
}

LCDBitmap* CreateQrCodeSubmit(PlaydateAPI* pd, char* secret_key, unsigned int gameId, unsigned int score, int DesiredQrCodeSize, char** verifyCodeOut)
{
	//unsigned int t = pd->system->getCurrentTimeMilliseconds();
	char* qrsubmit;
	char * verifyCode = CreateVerifierCode(pd, secret_key, gameId, score);
	//need to urlencode verifycode because of base64 plus signs (+), score and gameid should be fine
	char* verifyCodeEncoded = urlencode(pd, verifyCode);
	pd->system->formatString(&qrsubmit, "https://scores.joyrider3774.xyz/AddScoreVerify.php?game=%d&score=%u&verify=%s", gameId, score, verifyCodeEncoded);
	pd->system->realloc(verifyCodeEncoded, 0);
	LCDBitmap* result = CreateQrCode(pd, qrsubmit, DesiredQrCodeSize);
	if (verifyCodeOut)
		*verifyCodeOut = verifyCode;
	else
		pd->system->realloc(verifyCode, 0);
	pd->system->realloc(qrsubmit, 0);
	//pd->system->logToConsole("%u", pd->system->getCurrentTimeMilliseconds() - t);
	return result;
}
