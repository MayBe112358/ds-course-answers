#include <stdio.h>
#define MAXSIZE 50
#define SHARESIZE 6

typedef struct {
    int data[MAXSIZE];
    int top;                /* 栈顶下标，-1 表示空栈 */
} SqStack;

/* 两栈共享一段空间：1 号栈从左往右长，2 号栈从右往左长 */
typedef struct {
    int data[SHARESIZE];
    int top1, top2;         /* 空时 top1 = -1，top2 = SHARESIZE */
} DqStack;

/* 1）初始化  2）置空：顺序栈只要把 top 归位 */
void InitStack(SqStack *S) { S->top = -1; }
void ClearStack(SqStack *S) { S->top = -1; }

/* 3）入栈 */
void Push(SqStack *S, int x) {
    if (S->top == MAXSIZE - 1) {
        printf("栈满，%d 无法入栈\n", x);
        return;
    }
    S->data[++S->top] = x;
    printf("%d 入栈\n", x);
}

/* 4）出栈 */
void Pop(SqStack *S) {
    if (S->top == -1) {
        printf("栈空，无法出栈\n");
        return;
    }
    printf("出栈：%d\n", S->data[S->top--]);
}

/* 5）打印，从栈底到栈顶 */
void PrintStack(SqStack *S) {
    int i;
    printf("栈底 → 栈顶：");
    if (S->top == -1)
        printf("（空）");
    for (i = 0; i <= S->top; i++)
        printf("%d ", S->data[i]);
    printf("\n");
}

/* 6）两栈共享空间：两个栈顶相邻时空间才算用完 */
void DqPush(DqStack *S, int k, int x) {
    if (S->top1 + 1 == S->top2) {
        printf("共享空间已满，%d 无法入栈\n", x);
        return;
    }
    if (k == 1)
        S->data[++S->top1] = x;
    else
        S->data[--S->top2] = x;
    printf("%d 进入 %d 号栈\n", x, k);
}

void DqPop(DqStack *S, int k) {
    if ((k == 1 && S->top1 == -1) || (k == 2 && S->top2 == SHARESIZE)) {
        printf("%d 号栈空\n", k);
        return;
    }
    if (k == 1)
        printf("1 号栈出栈：%d\n", S->data[S->top1--]);
    else
        printf("2 号栈出栈：%d\n", S->data[S->top2++]);
}

void DqPrint(DqStack *S) {
    int i;
    printf("1 号栈：");
    for (i = 0; i <= S->top1; i++)
        printf("%d ", S->data[i]);
    printf("\n2 号栈：");
    for (i = SHARESIZE - 1; i >= S->top2; i--)
        printf("%d ", S->data[i]);
    printf("\n");
}

int main(void) {
    SqStack S;
    DqStack D = {{0}, -1, SHARESIZE};
    int op, x, k;
    InitStack(&S);
    printf("【菜单】1 初始化  2 置空  3 入栈  4 出栈  5 打印  6 共享栈入栈  7 共享栈出栈  8 打印共享栈  0 退出\n");
    while (1) {
        printf("请选择操作：");
        if (scanf("%d", &op) != 1 || op == 0)
            break;
        switch (op) {
        case 1: InitStack(&S); printf("栈已初始化\n"); break;
        case 2: ClearStack(&S); printf("栈已置空\n"); break;
        case 3: printf("请输入入栈元素："); scanf("%d", &x); Push(&S, x); break;
        case 4: Pop(&S); break;
        case 5: PrintStack(&S); break;
        case 6: printf("请输入栈号(1/2)和元素："); scanf("%d %d", &k, &x); DqPush(&D, k, x); break;
        case 7: printf("请输入栈号(1/2)："); scanf("%d", &k); DqPop(&D, k); break;
        case 8: DqPrint(&D); break;
        default: printf("没有这个操作\n");
        }
    }
    return 0;
}
