/*
 * Copyright 2008-2017 Katherine Flavel
 *
 * See LICENCE for the full copyright terms.
 */

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>

#include <adt/set.h>

#include <fsm/fsm.h>
#include <fsm/pred.h>

#include "internal.h"

struct mapping {
	struct fsm_state *old;
	struct fsm_state *new;

	unsigned int done:1;

	struct mapping *next;
};

static struct mapping *
mapping_ensure(struct fsm *fsm, struct mapping **head, struct fsm_state *old)
{
	struct mapping *m;
	struct alloc_closure *alloc;

	assert(fsm != NULL);
	assert(head != NULL);
	assert(old != NULL);

	alloc = &fsm->alloc;

	/* Find an existing mapping */
	for (m = *head; m != NULL; m = m->next) {
		if (m->old == old) {
			return m;
		}
	}

	/* Otherwise, make a new one */
	{
		m = alloc->cb(NULL, sizeof *m, alloc->opaque);
		if (m == NULL) {
			return 0;
		}

		m->new = fsm_addstate(fsm);
		if (m->new == NULL) {
			(void)alloc->cb(m, 0, alloc->opaque);
			return 0;
		}

		fsm_setend(fsm, m->new, fsm_isend(fsm, old));

		m->old  = old;
		m->done = 0;

		m->next = *head;
		*head = m;
	}

	return m;
}

static void
mapping_free(struct alloc_closure *alloc, struct mapping *mapping)
{
	struct mapping *next;
	struct mapping *m;

	for (m = mapping; m != NULL; m = next) {
		next = m->next;

		(void)alloc->cb(m, 0, alloc->opaque);
	}
}

static struct mapping *
getnextnotdone(struct mapping *mapping)
{
	struct mapping *m;

	assert(mapping != NULL);

	for (m = mapping; m != NULL; m = m->next) {
		if (!m->done) {
			return m;
		}
	}

	return NULL;
}

struct fsm_state *
fsm_state_duplicatesubgraph(struct fsm *fsm, struct fsm_state *state)
{
	assert(fsm != NULL);
	assert(state != NULL);

	return fsm_state_duplicatesubgraphx(fsm, state, NULL);
}

struct fsm_state *
fsm_state_duplicatesubgraphx(struct fsm *fsm, struct fsm_state *state,
	struct fsm_state **x)
{
	struct mapping *mappings;
	struct mapping *m;
	struct mapping *start;
	struct fsm_state *res;
	struct alloc_closure *alloc;

	assert(fsm != NULL);
	assert(state != NULL);

	alloc = &fsm->alloc;

	mappings = NULL;

	/* Start off the working list by populating it with the given state */
	start = mapping_ensure(fsm, &mappings, state);
	if (start == NULL) {
		return 0;
	}

	/* TODO: does this traversal algorithim have a name? */
	/* TODO: errors leave fsm in a questionable state */

	while (m = getnextnotdone(mappings), m != NULL) {
		struct set_iter it, jt;
		struct fsm_state *s;
		struct fsm_edge *e;

		if (x != NULL && m->old == *x) {
			*x = m->new;
		}

		for (e = set_first(m->old->edges, &it); e != NULL; e = set_next(&it)) {
			for (s = set_first(e->sl, &jt); s != NULL; s = set_next(&jt)) {
				struct mapping *to;

				assert(s != NULL);

				to = mapping_ensure(fsm, &mappings, s);
				if (to == NULL) {
					mapping_free(alloc, mappings);
					return NULL;
				}

				if (!fsm_addedge(alloc, m->new, to->new, e->symbol)) {
					return NULL;
				}
			}
		}

		m->done = 1;
	}

	res = start->new;
	mapping_free(alloc, mappings);

	return res;
}

