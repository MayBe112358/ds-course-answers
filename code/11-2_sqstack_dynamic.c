#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACK_INIT_SIZE 2
#define STACKINCREMENT 2

/* 动态顺序栈：空间用 malloc 分配，满了用 realloc 扩充 */
typedef struct {
    int *base;
    int *top;               /* 指向栈顶元素的下一个位置 */
    int stacksize;
} SqStack;

/* 两栈共享一块动态空间，满了就扩大一倍，并把 2 号栈搬到新空间的右端 */
typedef struct {
    int *base;
    int size;
    int top1;               /* 1 号栈的元素个数，也是下一个空位 */
    int top2;               /* 2 号栈栈顶元素的下标，空时等于 size */
} DqStack;

void InitStack(SqStack *S) {
    S->base = (int *)malloc(STACK_INIT_SIZE * sizeof(int));
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
}

void DestroyStack(SqStack *S) {
    free(S->base);
    S->base = S->top = NULL;
}

void ClearStack(SqStack *S) { S->top = S->base; }

void Push(SqStack *S, int x) {
    if (S->top - S->base == S->stacksize) {
        S->base = (int *)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(int));
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
        printf("栈满，空间扩充到 %d\n", S->stacksize);
    }
    *S->top++ = x;
    printf("%d 入栈\n", x);
}

void Pop(SqStack *S) {
    if (S->top == S->base) {
        printf("栈空，无法出栈\n");
        return;
    }
    printf("出栈：%d\n", *--S->top);
}

void PrintStack(SqStack *S) {
    int *p;
    printf("栈底 → 栈顶：");
    if (S->top == S->base)
        printf("（空）");
    for (p = S->base; p < S->top; p++)
        printf("%d ", *p);
    printf("\n");
}

void DqInit(DqStack *S) {
    S->size = STACK_INIT_SIZE;
    S->base = (int *)malloc(S->size * sizeof(int));
    S->top1 = 0;
    S->top2 = S->size;
}

void DqPush(DqStack *S, int k, int x) {
    if (S->top1 == S->top2) {
        int n2 = S->size - S->top2, newsize = S->size * 2;
        S->base = (int *)realloc(S->base, newsize * sizeof(int));
        memmove(S->base + newsize - n2, S->base + S->top2, n2 * sizeof(int));
        S->top2 = newsize - n2;
        S->size = newsize;
        printf("共享空间已满，扩充到 %d\n", newsize);
    }
    if (k == 1)
        S->base[S->top1++] = x;
    else
        S->base[--S->top2] = x;
    printf("%d 进入 %d 号栈\n", x, k);
}

void DqPop(DqStack *S, int k) {
    if ((k == 1 && S->top1 == 0) || (k == 2 && S->top2 == S->size)) {
        printf("%d 号栈空\n", k);
        return;
    }
    if (k == 1)
        printf("1 号栈出栈：%d\n", S->base[--S->top1]);
    else
        printf("2 号栈出栈：%d\n", S->base[S->top2++]);
}

void DqPrint(DqStack *S) {
    int i;
    printf("1 号栈：");
    for (i = 0; i < S->top1; i++)
        printf("%d ", S->base[i]);
    printf("\n2 号栈：");
    for (i = S->size - 1; i >= S->top2; i--)
        printf("%d ", S->base[i]);
    printf("\n");
}

int main(void) {
    SqStack S;
    DqStack D;
    int op, x, k;
    InitStack(&S);
    DqInit(&D);
    printf("【菜单】1 初始化  2 置空  3 入栈  4 出栈  5 打印  6 共享栈入栈  7 共享栈出栈  8 打印共享栈  0 退出\n");
    while (1) {
        printf("请选择操作：");
        if (scanf("%d", &op) != 1 || op == 0)
            break;
        switch (op) {
        case 1:
            DestroyStack(&S);
            InitStack(&S);
            printf("栈已初始化，容量 %d\n", S.stacksize);
            break;
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
    DestroyStack(&S);
    free(D.base);
    return 0;
}
