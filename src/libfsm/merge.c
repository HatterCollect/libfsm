/* $Id$ */

#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include <fsm/bool.h>

#include "internal.h"

struct fsm *
fsm_merge(struct fsm *a, struct fsm *b)
{
	assert(a != NULL);
	assert(b != NULL);

	if (a->opt != b->opt) {
		errno = EINVAL;
		return NULL;
	}

	*a->tail = b->sl;
	a->tail  = b->tail;

	a->endcount += b->endcount;

	a->may_have_epsilons |= b->may_have_epsilons;

	b->sl   = NULL;
	b->tail = NULL;
	fsm_free(b);

	fsm_setstart(a, NULL);

	return a;
}

