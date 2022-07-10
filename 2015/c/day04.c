#include <stdio.h>
#include <string.h>
#include <err.h>

#ifdef __APPLE__
# include <CommonCrypto/CommonCrypto.h>
# define MD5_Init	CC_MD5_Init
# define MD5_Update	CC_MD5_Update
# define MD5_Final	CC_MD5_Final
# define MD5_CTX	CC_MD5_CTX
#else
# include <openssl/md5.h>
#endif

int
main(int argc, char **argv) {
	char s[64];
	unsigned char d[16];
	int p1=0, p2=0, i, len;
	MD5_CTX md5;

	if (argc == 1)
		errx(1, "need arg"); 

	for (i = 1; !p1 || !p2; i++) {
		len = snprintf(s, sizeof(s), "%s%d", argv[1], i);
		if (len<1 || len >= (int)sizeof(s))
			errx(1, "snprintf");

		MD5_Init(&md5);
		MD5_Update(&md5, (unsigned char*)s, len);
		MD5_Final(d, &md5);

		if (!p1 && !d[0] && !d[1] && !(d[2] & 0xF0))
			p1 = i;
		if (!p2 && !d[0] && !d[1] && !d[2])
			p2 = i;
	}

	printf("04: %d %d\n", p1, p2);
}
