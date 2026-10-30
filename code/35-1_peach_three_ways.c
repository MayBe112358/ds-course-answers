#include <stdio.h>
#include <stdlib.h>
#define DAYS 10

typedef struct Node {
    int day, peach;         /* 第几天，这天开始时有几个桃子 */
    struct Node *next;
} Node;

/* 前一天的桃子数 = (后一天的桃子数 + 1) × 2 */

/* 1）链表：从第 10 天往前推，每推出一天就在表头插入结点，最后表头是第 1 天 */
Node *ByList(void) {
    Node *head = NULL, *s;
    int d, x = 1;
    for (d = DAYS; d >= 1; d--) {
        s = (Node *)malloc(sizeof(Node));
        s->day = d;
        s->peach = x;
        s->next = head;
        head = s;
        x = (x + 1) * 2;
    }
    return head;
}

/* 2）栈：第 10 天的 1 个先入栈，每次由栈顶推出前一天并入栈，最后栈顶就是第 1 天 */
int ByStack(void) {
    int stack[DAYS], top = -1, d;
    stack[++top] = 1;
    for (d = DAYS - 1; d >= 1; d--) {
        stack[top + 1] = (stack[top] + 1) * 2;
        top++;
    }
    return stack[top];
}

/* 3）递归：Peach(d) = (Peach(d + 1) + 1) × 2，第 10 天是出口 */
int ByRecursion(int d) {
    if (d == DAYS)
        return 1;
    return (ByRecursion(d + 1) + 1) * 2;
}

int main(void) {
    Node *head = ByList(), *p;
    printf("链表法：第 1 天共有 %d 个桃子\n", head->peach);
    printf("栈　法：第 1 天共有 %d 个桃子\n", ByStack());
    printf("递归法：第 1 天共有 %d 个桃子\n", ByRecursion(1));
    printf("每天开始时的桃子数：");
    for (p = head; p != NULL; p = p->next)
        printf("%d ", p->peach);
    printf("\n");
    while (head != NULL) {
        p = head;
        head = head->next;
        free(p);
    }
    return 0;
}
