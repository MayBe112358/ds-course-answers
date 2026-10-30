#include <stdio.h>
#include <stdlib.h>
#define N 8

int pos[N];     /* 当栈用：pos[0..top] 是已放好的各行皇后的列号 */

/* 第 row 行放在第 col 列是否与前面各行冲突 */
int Safe(int row, int col) {
    int i;
    for (i = 0; i < row; i++)
        if (pos[i] == col || abs(pos[i] - col) == row - i)
            return 0;
    return 1;
}

int main(void) {
    int top = 0, i, j;
    pos[0] = -1;                    /* 栈顶这一行从第 0 列开始试 */
    while (top >= 0 && top < N) {
        pos[top]++;
        while (pos[top] < N && !Safe(top, pos[top]))
            pos[top]++;
        if (pos[top] < N) {         /* 这一行放好了，下一行入栈 */
            top++;
            if (top < N)
                pos[top] = -1;
        } else {
            top--;                  /* 这一行无处可放，出栈回到上一行 */
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf(pos[i] == j ? "Q " : ". ");
        printf("\n");
    }
    return 0;
}
