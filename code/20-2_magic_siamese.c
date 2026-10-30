#include <stdio.h>
#define MAXN 19

int a[MAXN + 1][MAXN + 1];

/* 楼梯法：1 放在第一行中间，之后每个数放在右上方，
   出界就绕到另一边；右上方已经有数，就改放到正下方 */
void Fill(int n) {
    int i = 1, j = (n + 1) / 2, k, ni, nj;
    a[i][j] = 1;
    for (k = 2; k <= n * n; k++) {
        ni = (i == 1) ? n : i - 1;
        nj = (j == n) ? 1 : j + 1;
        if (a[ni][nj] != 0) {
            ni = (i == n) ? 1 : i + 1;
            nj = j;
        }
        i = ni;
        j = nj;
        a[i][j] = k;
    }
}

int Check(int n) {
    int target = n * (n * n + 1) / 2, i, j, r, c, d1 = 0, d2 = 0;
    for (i = 1; i <= n; i++) {
        r = c = 0;
        for (j = 1; j <= n; j++) {
            r += a[i][j];
            c += a[j][i];
        }
        if (r != target || c != target)
            return 0;
        d1 += a[i][i];
        d2 += a[i][n + 1 - i];
    }
    return d1 == target && d2 == target;
}

int main(void) {
    int n, i, j;
    printf("请输入奇数 n：");
    scanf("%d", &n);
    if (n < 1 || n > MAXN || n % 2 == 0) {
        printf("n 必须是 1 ~ %d 之间的奇数\n", MAXN);
        return 1;
    }
    Fill(n);
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++)
            printf("%4d", a[i][j]);
        printf("\n");
    }
    if (Check(n))
        printf("每行、每列和两条对角线之和都是 %d\n", n * (n * n + 1) / 2);
    else
        printf("不是魔方阵\n");
    return 0;
}
