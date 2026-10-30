#include <stdio.h>
#define N 8

int pos[N];                 /* pos[i]：第 i 行皇后所在的列 */
int colUsed[N];             /* 第 j 列是否已有皇后 */
int diag1[2 * N - 1];       /* 主对角线，同一条线上 行-列 相同，下标 i-j+N-1 */
int diag2[2 * N - 1];       /* 副对角线，同一条线上 行+列 相同，下标 i+j */

void PrintBoard(void) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf(pos[i] == j ? "Q " : ". ");
        printf("\n");
    }
}

/* 从第 row 行开始放皇后，全部放好返回 1 */
int Place(int row) {
    int j;
    if (row == N)
        return 1;
    for (j = 0; j < N; j++) {
        if (colUsed[j] || diag1[row - j + N - 1] || diag2[row + j])
            continue;               /* 冲突，试下一列 */
        pos[row] = j;
        colUsed[j] = diag1[row - j + N - 1] = diag2[row + j] = 1;
        if (Place(row + 1))
            return 1;
        colUsed[j] = diag1[row - j + N - 1] = diag2[row + j] = 0;   /* 回溯 */
    }
    return 0;
}

int main(void) {
    if (Place(0))
        PrintBoard();
    return 0;
}
