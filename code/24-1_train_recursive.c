#include <stdio.h>
#define MAXN 9

/* 双向栈：调度站从数组左端往右长，输出序列从右端往左长，共用一块空间 */
int space[MAXN];
int top1 = -1, top2 = MAXN;
int n, count = 0;

void PrintSeq(void) {
    int i;
    for (i = MAXN - 1; i >= top2; i--)  /* 从栈底（右端）到栈顶就是出站顺序 */
        printf("%d", space[i]);
    printf(++count % 7 == 0 ? "\n" : "  ");
}

/* next 是下一节等待进站的车厢号 */
void Dispatch(int next) {
    int x;
    if (next > n && top1 == -1) {       /* 全部车厢都已出站 */
        PrintSeq();
        return;
    }
    if (top1 >= 0) {                    /* 选择一：调度站栈顶的车厢出站 */
        x = space[top1--];
        space[--top2] = x;
        Dispatch(next);
        top2++;                         /* 恢复现场，再试另一种选择 */
        space[++top1] = x;
    }
    if (next <= n) {                    /* 选择二：下一节车厢进站 */
        space[++top1] = next;
        Dispatch(next + 1);
        top1--;
    }
}

int main(void) {
    printf("请输入车厢数 n（1~%d）：", MAXN);
    scanf("%d", &n);
    if (n < 1 || n > MAXN) {
        printf("n 超出范围\n");
        return 1;
    }
    Dispatch(1);
    if (count % 7 != 0)
        printf("\n");
    printf("共 %d 种出站序列\n", count);
    return 0;
}
