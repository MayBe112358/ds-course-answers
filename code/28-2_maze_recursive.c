#include <stdio.h>
#include <stdlib.h>
#define MAXR 20

int maze[MAXR + 2][MAXR + 2];
int m, n;
int di[5] = {0, 0, 1, 0, -1};   /* 方向 1 东  2 南  3 西  4 北 */
int dj[5] = {0, 1, 0, -1, 0};

typedef struct SNode {
    int i, j, d;
    struct SNode *next;
} SNode;

typedef SNode *LinkStack;
LinkStack path = NULL;

void Push(LinkStack *S, int i, int j, int d) {
    SNode *s = (SNode *)malloc(sizeof(SNode));
    s->i = i;
    s->j = j;
    s->d = d;
    s->next = *S;
    *S = s;
}

/* 递归：从 (i,j) 出发能否走到出口。成功后在返回途中把每一格压栈，
   先压出口、最后压入口，所以入口在栈顶，顺着栈输出就是正向路径 */
int Search(int i, int j) {
    int d;
    maze[i][j] = 2;
    if (i == m && j == n) {
        Push(&path, i, j, 0);
        return 1;
    }
    for (d = 1; d <= 4; d++) {
        if (maze[i + di[d]][j + dj[d]] == 0 && Search(i + di[d], j + dj[d])) {
            Push(&path, i, j, d);
            return 1;
        }
    }
    return 0;
}

int main(void) {
    SNode *p;
    int i, j, k = 0;
    printf("请输入迷宫的行数和列数：");
    scanf("%d %d", &m, &n);
    printf("请输入迷宫（0 通路，1 墙）：");
    for (i = 0; i <= m + 1; i++)
        for (j = 0; j <= n + 1; j++)
            maze[i][j] = 1;
    for (i = 1; i <= m; i++)
        for (j = 1; j <= n; j++)
            scanf("%d", &maze[i][j]);
    if (maze[1][1] != 0 || !Search(1, 1)) {
        printf("没有通路\n");
        return 0;
    }
    printf("通路：\n");
    while (path != NULL) {
        p = path;
        printf("(%d,%d,%d)%s", p->i, p->j, p->d, ++k % 5 == 0 ? "\n" : " ");
        maze[p->i][p->j] = 3;
        path = p->next;
        free(p);
    }
    if (k % 5 != 0)
        printf("\n");
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++)
            printf("%s", maze[i][j] == 1 ? "# " : maze[i][j] == 3 ? "* " : ". ");
        printf("\n");
    }
    return 0;
}
