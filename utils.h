#ifndef FP_UTIL
#define FP_UTIL

extern int FP_SIZE;

struct fp_Cell {
	int num, done;
};

struct fp_Pos {
	int i, j;
};

int fp_abs(int n);
void fp_swap_cell(struct fp_Cell *a, struct fp_Cell *b);
void fp_swap_pos(struct fp_Pos *a, struct fp_Pos *b);
void fp_print(int size, int a[size][size]);
int fp_check_solvable(int a[FP_SIZE][FP_SIZE]);

#endif
