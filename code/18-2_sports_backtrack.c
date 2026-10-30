#include <stdio.h>
#define MAXN 20

int conflict[MAXN][MAXN];
int color[MAXN];            /* color[i]：项目 i 安排在第几个时间段 */
int n;

void ReadInput(void) {
    int m, k, i, a, b, item[MAXN];
    printf("请输入比赛项目数：");
    scanf("%d", &n);
    printf("请输入运动员人数：");
    scanf("%d", &m);
    for (i = 0; i < m; i++) {
        printf("请输入第 %d 位运动员参加的项目数和项目号：", i + 1);
        scanf("%d", &k);
        for (a = 0; a < k; a++)
            scanf("%d", &item[a]);
        for (a = 0; a < k; a++)
            for (b = 0; b < k; b++)
                if (a != b)
                    conflict[item[a]][item[b]] = 1;
    }
}

/* 用 k 个时间段给第 i 个及以后的项目排期，成功返回 1 */
int Try(int i, int k) {
    int c, j;
    if (i == n)
        return 1;
    for (c = 1; c <= k; c++) {
        for (j = 0; j < i; j++)
            if (conflict[i][j] && color[j] == c)
                break;
        if (j == i) {               /* 与前面同时间段的项目都不冲突 */
            color[i] = c;
            if (Try(i + 1, k))
                return 1;
        }
    }
    return 0;                       /* 回溯：第 i 个项目放哪里都不行 */
}

int main(void) {
    int k, c, i;
    ReadInput();
    for (k = 1; k <= n; k++)        /* 时间段数从 1 开始试，第一个成功的就是最少的 */
        if (Try(0, k))
            break;
    for (c = 1; c <= k; c++) {
        printf("第 %d 时间段：", c);
        for (i = 0; i < n; i++)
            if (color[i] == c)
                printf("%d ", i);
        printf("\n");
    }
    printf("共需 %d 个时间段（最少）\n", k);
    return 0;
}
