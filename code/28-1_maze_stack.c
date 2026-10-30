#include <stdio.h>
#include <stdlib.h>
#define MAXR 20

int maze[MAXR + 2][MAXR + 2];   /* 四周加一圈 1 作围墙，省去越界判断；2 表示走过 */
int m, n;
int di[5] = {0, 0, 1, 0, -1};   /* 方向 1 东  2 南  3 西  4 北 */
int dj[5] = {0, 1, 0, -1, 0};

typedef struct SNode {
    int i, j, d;                /* 坐标，以及从这里走向下一格的方向 */
    struct SNode *next;
} SNode;

typedef SNode *LinkStack;       /* 链栈，栈顶在表头 */

void Push(LinkStack *S, int i, int j, int d) {
    SNode *s = (SNode *)malloc(sizeof(SNode));
    s->i = i;
    s->j = j;
    s->d = d;
    s->next = *S;
    *S = s;
}

void Pop(LinkStack *S) {
    SNode *p = *S;
    *S = p->next;
    free(p);
}

/* 非递归：栈顶格子依次试四个方向，能走就把下一格入栈，四个方向都试过就出栈退回 */
int MazePath(LinkStack *S) {
    SNode *top;
    int ni, nj;
    Push(S, 1, 1, 0);
    maze[1][1] = 2;
    while (*S != NULL) {
        top = *S;
        if (top->i == m && top->j == n)
            return 1;
        if (++top->d > 4) {
            Pop(S);
            continue;
        }
        ni = top->i + di[top->d];
        nj = top->j + dj[top->d];
        if (maze[ni][nj] == 0) {
            maze[ni][nj] = 2;
            Push(S, ni, nj, 0);
        }
    }
    return 0;
}

int main(void) {
    LinkStack S = NULL, R = NULL;
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
    if (maze[1][1] != 0 || !MazePath(&S)) {
        printf("没有通路\n");
        return 0;
    }
    S->d = 0;                   /* 出口没有下一步 */
    while (S != NULL) {         /* 栈顶是出口，倒到另一个栈里，入口就到了栈顶 */
        Push(&R, S->i, S->j, S->d);
        Pop(&S);
    }
    printf("通路：\n");
    for (; R != NULL; Pop(&R)) {
        printf("(%d,%d,%d)%s", R->i, R->j, R->d, ++k % 5 == 0 ? "\n" : " ");
        maze[R->i][R->j] = 3;
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
