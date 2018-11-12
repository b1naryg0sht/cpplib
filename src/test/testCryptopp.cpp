#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <StdTypes.h>
#include <Cryptopp.h>

using namespace cppbrick;

int main()
{
	std::string src = "hello, workd, !@#$%^&^&**";
	char *dst = NULL;
	XCrypto::base64_encrypt(src.c_str(), src.size(), dst);
	if(strcmp(dst, "aGVsbG8sIHdvcmtkLCAhQCMkJV4mXiYqKg==") != 0)
	{
		printf("Base64 encrypt test fail\n");
		exit(1);
	}
	delete dst;

	char *crypto = "dGhpcyBpcyBhIHRlc3Q=";//"this is a test"
	char *plaintext = NULL;
	uint_t len;
	XCrypto::base64_decrypt(crypto, plaintext, len);
	if(strcmp(plaintext, "this is a test") != 0)
	{
		printf("Base64 decrypt test fail.after decrypt, plain text is:%s\n", plaintext);
		exit(1);
	}
	delete plaintext;


	//digest
	std::string digest;
	if(XDigest::md5_calculate_digest(src, digest) == true 
		&& digest != std::string("E385B5AE25DE108A3814744DF8D34911"))
	{
		printf("md5_calculate_digest test fail.digest:%s != E385B5AE25DE108A3814744DF8D34911\n", digest.c_str());
		exit(1);		
	}
	
	if(XDigest::sha256_calculate_digest(src, digest) == true &&
		digest != "3E7FF9835F1808E4DE187A7D8E20D2E23FADFA54477ED5510190BC898AFF253C")
	{
		printf("sha256_calculate_digest test fail.digest:%s != 3E7FF9835F1808E4DE187A7D8E20D2E23FADFA54477ED5510190BC898AFF253C\n", digest.c_str());
		exit(1);	
	}

	if(XDigest::sha1_calculate_digest(src, digest) == true &&
		digest != "650BE8AF5F03E913BFF0E7C774A053EA8D952437")
	{
		printf("sha1_calculate_digest test fail.digest:%s != 650BE8AF5F03E913BFF0E7C774A053EA8D952437\n", digest.c_str());
		exit(1);	
	}


}

