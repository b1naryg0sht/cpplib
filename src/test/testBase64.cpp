#include <iostream>
#include <unistd.h>
#include <string.h>
#include <Base64.h>

using namespace cppbrick;

int main()
{
	char *src = "hello, workd, !@#$%^&^&**";
	char *dst = NULL;
	Base64::encrypt(src, strlen(src), dst);
	if(strcmp(dst, "aGVsbG8sIHdvcmtkLCAhQCMkJV4mXiYqKg==") != 0)
	{
		printf("Base64 encrypt test fail\n");
		exit(1);
	}
	delete dst;

	char *crypto = "dGhpcyBpcyBhIHRlc3Q=";//"this is a test"
	char *plaintext = NULL;
	int ret = Base64::decrypt(crypto, plaintext);
	if(strcmp(plaintext, "this is a test") != 0)
	{
		printf("Base64 decrypt test fail.ret=%d.after decrypt, plain text is:%s\n", ret, plaintext);
		exit(1);
	}
	delete plaintext;

}

