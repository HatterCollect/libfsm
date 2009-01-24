/* $Id$ */

#ifndef INTERNAL_H
#define INTERNAL_H

struct fsm_state {
	unsigned int id;

	unsigned int end;	/* <= INT_MAX */

	struct fsm_edge *edges;
};

struct fsm_edge {
	struct label_list *label;
	struct fsm_state *state;

	struct fsm_edge *next;

	/* TODO: union for literal etc, and an enum */
};


/* global registry of all states */
struct state_list {
	struct fsm_state state;
	struct state_list *next;
};

/* global registry of all labels */
struct label_list {
	char *label;
	struct label_list *next;
};

struct fsm {
	struct state_list *sl;
	struct label_list *ll;
	struct fsm_state *start;
	struct fsm_options options;
};

#endif
