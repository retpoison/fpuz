#ifndef FP_ANSWER
#define FP_ANSWER

#include "solver.h"
#include "utils.h"

struct fp_Answer {
	int len, cap;
	struct fp_Pos *cells_move;
	int *empty_move;
};

struct fp_Answer *fp_new_answer();
int fp_allocate_ans(struct fp_Answer *a);
int fp_add_moves(struct fp_Answer *a, struct fp_Pos p, int d);
void fp_clean_answer(struct fp_Answer *a);

#endif
