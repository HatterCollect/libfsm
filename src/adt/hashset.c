/*
 * Copyright 2018-2019 Shannon F. Stewman
 *
 * See LICENCE for the full copyright terms.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <fsm/fsm.h>

#include <adt/hashset.h>

/* XXX: cheesing around uint8_t here */
extern int
siphash(const unsigned char *in, const size_t inlen, const unsigned char *k,
            unsigned char *out, const size_t outlen);

/* random key read from /dev/random */
/* XXX: replace with a seed read from /dev/random at startup... */
static const unsigned char hashk[] = {
	0x14, 0xa8, 0xff, 0x36, 0x15, 0x16, 0x2c, 0xf7, 0xf4, 0xce, 0xb8, 0x66, 0x74, 0xf4, 0x3d, 0x64,
};

unsigned long
hashstates(const fsm_state_t *p, size_t n)
{
	unsigned long h = 0;
	unsigned char ha[sizeof h];

	siphash((const void *) p, n * sizeof *p, hashk, &ha[0], sizeof ha);
	memcpy(&h, &ha[0], sizeof h);

	return h;
}

