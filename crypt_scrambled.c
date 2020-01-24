#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

// Function from Nigel Cunningham's pam-mysql fork crypto.c
// Licensed under the GNU GPL 2.0
static char *hexify(char * const result, const unsigned char *digest,
	const size_t size_result, size_t size_digest)
{
	static const char * const hexchars = "0123456789ABCDEF";
	char *result_pnt = result;

	if (size_digest <= (size_t) 0 ||
		size_result <= (size_digest * (size_t) 2U)) {
		return NULL;
	}
	do {
		*result_pnt++ = hexchars[(*digest >> 4) & 0xf];
		*result_pnt++ = hexchars[*digest & 0xf];
		digest++;
		size_digest--;
	} while (size_digest > (size_t) 0U);
	*result_pnt = 0;

	result;
}

// Implementation from commit 2db6b50c7b7c638104bd9639994f0574e8f4813c in Pure-ftp source.
// Original function name `make_scrambled_password`, under the following license:
/*
 * Copyright (c) 2001 - 2019
 * Frank Denis <j at pureftpd dot org> with help of contributors.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
void crypt_scrambled_password(char scrambled_password[42], const char password[255])
{
	SHA_CTX      ctx;
	unsigned char h0[20], h1[20];

	SHA1_Init(&ctx);
	SHA1_Update(&ctx, password, strlen(password));
	SHA1_Final(h0, &ctx);
	SHA1_Init(&ctx);
	SHA1_Update(&ctx, h0, sizeof h0);
	explicit_bzero(h0, strlen(password));
	SHA1_Final(h1, &ctx);
	*scrambled_password = '*';
	hexify(scrambled_password + 1U, h1, 42, sizeof h1);
}
