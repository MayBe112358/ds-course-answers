#include <stdio.h>
#define MAXN 19

int a[MAXN + 1][MAXN + 1];  /* 下标从 1 开始，与题目一致 */

/* 按题目给的规则填：共 n 轮，每轮 n 个数 */
void Fill(int n) {
    int i = (n + 1) / 2, j = n, k;  /* 1 放在中间一行的最后一列 */
    a[i][j] = 1;
    for (k = 2; k <= n * n; k++) {
        if ((k - 1) % n == 0) {     /* 新一轮的第一个数：同一行的左边一列 */
            j = (j == 1) ? n : j - 1;
        } else {                    /* 本轮其余的数：右下方，出界就绕到第一行或第一列 */
            i = (i == n) ? 1 : i + 1;
            j = (j == n) ? 1 : j + 1;
        }
        a[i][j] = k;
    }
}

/* 检查每行、每列和两条对角线之和是否都等于 n(n²+1)/2 */
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
