#include <stdio.h>
#include <string.h>

#include "answer.h"
#include "generator.h"
#include "solver.h"
#include "utils.h"

int
main()
{
	struct fp_Answer *ans;
	int size = 4;
	int a[4][4] = {
		{14, 1, 11, 9},
		{6, 3, 8, 10},
		{7, 5, 16, 12},
		{15, 2, 13, 4}
	};
	printf("4x4:\n");
	fp_print(size, a);
	ans = fp_solve(size, a);
	if (ans == NULL) {
		printf("An error occurred.\n");
		return 1;
	}
	printf("solved:\n");
	fp_print(size, a);

	fp_clean_answer(ans);
	ans = NULL;

	int b[3][3] = {
		{5, 2, 8},
		{4, 1, 7},
		{9, 3, 6},
	};
	printf("\n3x3:\n");
	ans = fp_solve(3, b);
	if (ans == NULL) {
		printf("An error occurred.\n");
		return 1;
	}
	fp_print(3, b);

	fp_clean_answer(ans);
	ans = NULL;

	printf("\ngenerating:\n");
	fp_generate(size, a);
	fp_print(size, a);
	ans = fp_solve(size, a);
	if (ans == NULL) {
		printf("An error occurred.\n");
		return 1;
	}
	printf("solved:\n");
	fp_print(size, a);
	fp_clean_answer(ans);
	ans = NULL;
	return 0;
}
