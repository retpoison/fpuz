#include <stdio.h>
#include <stdlib.h>

#include "fpuz.h"

int
fp_abs(int n)
{
	return (n > 0) ? n : -n;
}

void
fp_swap_cell(struct fp_Cell *a, struct fp_Cell *b)
{
	struct fp_Cell tmp = *a;
	*a = *b;
	*b = tmp;
}

void
fp_swap_pos(struct fp_Pos *a, struct fp_Pos *b)
{
	struct fp_Pos tmp = *a;
	*a = *b;
	*b = tmp;
}

void
fp_print(int size, int a[size][size])
{
	FP_SIZE = size;
	int i, j;
	for (i = 0; i < FP_SIZE; i++) {
		for (j = 0; j < FP_SIZE; j++) {
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}
}

int
fp_is_solvable(int a[FP_SIZE][FP_SIZE])
{
	int i1, j1, i2, j2, inversions = 0, epos = 0;
	for (i1 = 0; i1 < FP_SIZE; i1++) {
		for (j1 = 0; j1 < FP_SIZE; j1++) {
			if (a[i1][j1] == FP_SIZE * FP_SIZE) {
				epos = FP_SIZE - i1;
				continue;
			}
			for (i2 = i1; i2 < FP_SIZE; i2++) {
				for (j2 = j1; j2 < FP_SIZE; j2++) {
					if (a[i1][j1] > a[i2][j2])
						inversions++;
				}
			}
		}
	}
	if (FP_SIZE & 1)
		return !(inversions & 1);
	if (epos & 1)
		return !(inversions & 1);
	else
		return inversions & 1;
}
