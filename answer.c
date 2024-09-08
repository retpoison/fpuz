#include <stdlib.h>

#include "answer.h"
#include "solver.h"
#include "utils.h"

struct fp_Answer *
fp_new_answer()
{
	struct fp_Answer *a =
	    (struct fp_Answer *)malloc(sizeof(struct fp_Answer));
	if (a == NULL)
		return NULL;
	a->len = -1;
	a->cap = 32;
	a->cells_move = NULL;
	a->empty_move = NULL;
	int s = fp_allocate_ans(a);
	if (s)
		return NULL;
	return a;
}

int
fp_allocate_ans(struct fp_Answer *a)
{
	a->cells_move =
	    (struct fp_Pos *)realloc(a->cells_move,
				     a->cap * sizeof(struct fp_Pos));
	a->empty_move =
	    (enum direction *)realloc(a->empty_move,
				      a->cap * sizeof(enum direction));
	if (a->cells_move == NULL || a->empty_move == NULL)
		return 1;
	return 0;
}

int
fp_add_moves(struct fp_Answer *a, struct fp_Pos p, enum direction d)
{
	(a->len)++;
	int s = 0;
	if (a->len == a->cap) {
		a->cap += 32;
		s = fp_allocate_ans(a);
	}
	if (s)
		return 1;
	a->cells_move[a->len].i = p.i;
	a->cells_move[a->len].j = p.j;
	a->empty_move[a->len] = d;
	return 0;
}

void
fp_clean_answer(struct fp_Answer *a)
{
	a->len = 0;
	a->cap = 0;
	free(a->cells_move);
	free(a->empty_move);
	free(a);
}
