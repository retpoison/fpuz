#include <string.h>
#include <stdio.h>

#include "fpuz.h"

static struct fp_Answer *ans = NULL;

struct fp_Answer *
fp_solve(int size, int p[size][size])
{
	FP_SIZE = size;
	if (!fp_check_solvable(p)) {
		printf("can't solve this puzzle.\n");
		return NULL;
	}
	ans = NULL;
	ans = fp_new_answer();
	if (ans == NULL)
		return NULL;
	struct fp_Cell a[FP_SIZE][FP_SIZE];
	struct fp_Pos empty_pos = { -1, -1 };
	int i, j, s;
	for (i = 0; i < FP_SIZE; i++) {
		for (j = 0; j < FP_SIZE; j++) {
			a[i][j].num = p[i][j];
			a[i][j].done = 0;
			if (a[i][j].num == FP_SIZE * FP_SIZE)
				empty_pos = (struct fp_Pos) { i, j };
		}
	}

	if (empty_pos.i == -1 && empty_pos.j == -1)
		return NULL;

	s = fp_solve_puzzle(a, (struct fp_Pos) { 0, 0 }, &empty_pos);
	if (s)
		return NULL;

	for (i = 0; i < FP_SIZE; i++)
		for (j = 0; j < FP_SIZE; j++)
			p[i][j] = a[i][j].num;
	return ans;
}

int
fp_solve_puzzle(struct fp_Cell a[FP_SIZE][FP_SIZE],
		struct fp_Pos start, struct fp_Pos *epos)
{
	if (start.i == FP_SIZE && start.j == FP_SIZE)
		return 0;

	int i, j, endr[2], endc[2];
	struct fp_Pos dstr, dstc;
	int firstrow = start.i * FP_SIZE + start.j + 1,
	    endrow = (start.i + 1) * FP_SIZE,
	    endcol = FP_SIZE * (FP_SIZE - 1) + start.i + 1;
	fp_move_cell(a, firstrow, start, epos, 1);
	if (FP_SIZE - start.i <= 3 && FP_SIZE - start.j <= 3) {
		endrow++;
		endcol += FP_SIZE;
		goto next;
	}

	fp_move_cell(a, endrow, (struct fp_Pos) { start.i, FP_SIZE - 1 },
		     epos, 1);
	fp_move_cell(a, endcol, (struct fp_Pos) { FP_SIZE - 1, start.j },
		     epos, 1);

	for (i = firstrow + 1, j = firstrow + FP_SIZE;
	     i < endrow - 2 && j < endcol - (2 * FP_SIZE); i++, j += FP_SIZE) {
		dstr.i = (i - 1) / FP_SIZE;
		dstr.j = (i % FP_SIZE == 0) ? FP_SIZE - 1 : i % FP_SIZE - 1;
		fp_move_cell(a, i, dstr, epos, 1);

		dstc.i = (j - 1) / FP_SIZE;
		dstc.j = (j % FP_SIZE == 0) ? FP_SIZE - 1 : j % FP_SIZE - 1;
		fp_move_cell(a, j, dstc, epos, 1);
	}

 next:
	endr[0] = endrow - 1, endr[1] = endrow - 2;
	endc[0] = endcol - FP_SIZE, endc[1] = endcol - (2 * FP_SIZE);

	dstr.i = (endr[0] - 1) / FP_SIZE;
	dstr.j = (endr[0] % FP_SIZE == 0) ? FP_SIZE - 1 : endr[0] % FP_SIZE - 1;
	dstc.i = (endc[0] - 1) / FP_SIZE;
	dstc.j = (endc[0] % FP_SIZE == 0) ? FP_SIZE - 1 : endc[0] % FP_SIZE - 1;

	int arr[10][3] = {
		{2, 0, 0}, {0, 0, 1}, {1, 0, 1},
		{0, -1, 1}, {0, 0, 1}, {0, 2, 0},
		{0, 0, 1}, {0, 1, 1}, {-1, 0, 1},
		{0, 0, 1},
	};

	i = 0;
	int skip = -1;
	if (FP_SIZE - start.i <= 2 && FP_SIZE - start.j <= 2) {
		i = 1;
		skip = 5;
	}
	for (; i < 10; i++) {
		if (i == skip)
			continue;
		if (i < 5) {
			fp_move_cell(a, endr[i % 2], (struct fp_Pos) {
				     dstr.i + arr[i][0],
				     dstr.j + arr[i][1]
				     },
				     epos, arr[i][2]);
		} else {
			fp_move_cell(a, endc[(i + 1) % 2], (struct fp_Pos) {
				     dstc.i + arr[i][0],
				     dstc.j + arr[i][1]
				     },
				     epos, arr[i][2]);
		}
	}

	start.i++;
	start.j++;
	if (start.i < FP_SIZE - 1 && start.j < FP_SIZE - 1)
		return fp_solve_puzzle(a, start, epos);
	return 0;
}

int
fp_find_path(struct fp_Cell a[FP_SIZE][FP_SIZE],
	     enum direction drc[FP_SIZE * FP_SIZE], int drcindx,
	     struct fp_Pos tpos, struct fp_Pos epos)
{
	if (drcindx == FP_SIZE * FP_SIZE)
		return 0;
	if (epos.i == tpos.i && epos.j == tpos.j) {
		drc[drcindx] = Stop;
		return 1;
	}
	enum direction skip = Stop;
	if (drcindx > 0)
		skip = drc[drcindx - 1];

	enum direction drcarr[4] = { Down, Up, Right, Left };
	int cord[4][4] = {
		{0, -1, -1, 0},
		{FP_SIZE - 1, -1, 1, 0},
		{-1, 0, 0, -1},
		{-1, FP_SIZE - 1, 0, 1}
	};
	int i, s = 0;
	for (i = 0; i < 4; i++) {
		if (epos.i != cord[i][0] && epos.j != cord[i][1] &&
		    a[epos.i + cord[i][2]][epos.j + cord[i][3]].done == 0 &&
		    skip != drcarr[i]) {
			drc[drcindx] = -drcarr[i];
			a[epos.i + cord[i][2]][epos.j + cord[i][3]].done = 1;
			s = fp_find_path(a, drc, drcindx + 1, tpos,
					 (struct fp_Pos) { epos.i + cord[i][2],
					 epos.j + cord[i][3]
					 });
			a[epos.i + cord[i][2]][epos.j + cord[i][3]].done = 0;
		}
		if (s)
			return 1;
	}
	return 0;
}

void
fp_move_empty_cell(struct fp_Cell a[FP_SIZE][FP_SIZE],
		   struct fp_Pos target_pos, struct fp_Pos *epos)
{
	enum direction drc[FP_SIZE * FP_SIZE];
	memset(drc, Stop, FP_SIZE * FP_SIZE * sizeof(enum direction));
	int s = 0;
	s = fp_find_path(a, drc, 0, target_pos, *epos);
	if (s == 0) {
		return;
	}
	int i = 0;
	struct fp_Pos e = { 0, 0 };
	while (i < FP_SIZE * FP_SIZE) {
		switch (drc[i]) {
		case Up:
			e = (struct fp_Pos) { -1, 0 };
			break;
		case Down:
			e = (struct fp_Pos) { 1, 0 };
			break;
		case Right:
			e = (struct fp_Pos) { 0, 1 };
			break;
		case Left:
			e = (struct fp_Pos) { 0, -1 };
			break;
		case Stop:
			return;
		}
		fp_swap_cell(&a[epos->i][epos->j],
			     &a[epos->i + e.i][epos->j + e.j]);
		epos->i += e.i;
		epos->j += e.j;
		fp_add_moves(ans, *epos, drc[i]);
		i++;
	}
}

void
fp_move_cell(struct fp_Cell a[FP_SIZE][FP_SIZE], int n,
	     struct fp_Pos dst, struct fp_Pos *epos, int done)
{
	int i, j;
	struct fp_Pos p = { 0, 0 };
	for (i = 0; i < FP_SIZE; i++)
		for (j = 0; j < FP_SIZE; j++)
			if (a[i][j].num == n) {
				p = (struct fp_Pos) { i, j };
				break;
			}

	int disti = dst.i - p.i, distj = dst.j - p.j,
	    movei = (disti > 0) ? 1 : -1, movej = (distj > 0) ? 1 : -1;

	struct fp_Pos tpos = { -1, -1 };
	a[p.i][p.j].done = 1;
	int flagij = 0;
	while (p.i != dst.i || p.j != dst.j) {
		if (disti != 0 && a[p.i + movei][p.j].done == 0) {
			flagij = 1;
			tpos = (struct fp_Pos) { p.i + movei, p.j };
		} else if (distj != 0 && a[p.i][p.j + movej].done == 0) {
			flagij = 2;
			tpos = (struct fp_Pos) { p.i, p.j + movej };
		}
		if (a[tpos.i][tpos.j].num == FP_SIZE * FP_SIZE)
			goto swap;
		fp_move_empty_cell(a, tpos, epos);
		if (a[tpos.i][tpos.j].num != FP_SIZE * FP_SIZE)
			break;
 swap:
		fp_swap_cell(&a[p.i][p.j], &a[tpos.i][tpos.j]);
		fp_add_moves(ans, p, fp_get_direction(p, tpos));
		fp_swap_pos(&p, epos);
		if (flagij == 1)
			disti = disti - movei;
		else if (flagij == 2)
			distj = distj - movej;
	}
	a[p.i][p.j].done = done;
}

enum direction
fp_get_direction(struct fp_Pos p, struct fp_Pos nextp)
{
	if (p.i == nextp.i) {
		if (nextp.j - p.j > 0)
			return Right;
		return Left;
	}
	if (p.j == nextp.j) {
		if (nextp.i - p.i > 0)
			return Down;
		return Up;
	}
	return Stop;
}
