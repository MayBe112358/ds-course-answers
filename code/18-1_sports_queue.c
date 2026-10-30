#include <stdio.h>
#define MAXN 20

int conflict[MAXN][MAXN];   /* conflict[i][j] = 1 表示项目 i 和 j 有同一位运动员参加 */
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

int main(void) {
    int queue[MAXN + 1], front = 0, rear = 0;   /* 循环队列 */
    int clash[MAXN];        /* clash[j]：项目 j 与本时间段已排项目的冲突数 */
    int group[MAXN], g = 0, pre, i, j;
    ReadInput();
    for (i = 0; i < n; i++)
        queue[rear++] = i;
    pre = n;
    while (front != rear) {
        i = queue[front];
        front = (front + 1) % (MAXN + 1);
        if (i <= pre) {             /* 序号变小说明队列绕完一圈，开始新的时间段 */
            g++;
            for (j = 0; j < n; j++)
                clash[j] = 0;
        }
        if (clash[i] == 0) {        /* 和本时间段的项目都不冲突，排进来 */
            group[i] = g;
            for (j = 0; j < n; j++)
                clash[j] += conflict[i][j];
        } else {                    /* 有冲突，重新入队等下一个时间段 */
            queue[rear] = i;
            rear = (rear + 1) % (MAXN + 1);
        }
        pre = i;
    }
    for (j = 1; j <= g; j++) {
        printf("第 %d 时间段：", j);
        for (i = 0; i < n; i++)
            if (group[i] == j)
                printf("%d ", i);
        printf("\n");
    }
    printf("共需 %d 个时间段\n", g);
    return 0;
}
