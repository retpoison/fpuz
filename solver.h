#ifndef FP_SOLVER
#define FP_SOLVER

#include "answer.h"
#include "utils.h"

enum direction {
	Up = 1,
	Down = -1,
	Right = 2,
	Left = -2,
	Stop = 0
};

struct fp_Answer *fp_solve(int size, int p[size][size]);

int fp_solve_puzzle(struct fp_Cell a[FP_SIZE][FP_SIZE],
		    struct fp_Pos start, struct fp_Pos *epos);

int fp_find_path(struct fp_Cell a[FP_SIZE][FP_SIZE],
		 enum direction drc[FP_SIZE * FP_SIZE], int drcindx,
		 struct fp_Pos tpos, struct fp_Pos epos);

void fp_move_empty_cell(struct fp_Cell a[FP_SIZE][FP_SIZE],
			struct fp_Pos target_pos, struct fp_Pos *epos);

void fp_move_cell(struct fp_Cell a[FP_SIZE][FP_SIZE], int n,
		  struct fp_Pos dst, struct fp_Pos *epos, int done);

enum direction fp_get_direction(struct fp_Pos p, struct fp_Pos nextp);

#endif
