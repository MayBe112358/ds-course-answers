#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {
    int data;
    struct LNode *next;
} LNode, *LinkList;

/* pre 从头结点出发，停在第一个不小于 x 的结点之前，x 插在 pre 后面 */
void InsertOrder(LinkList L, int x) {
    LNode *pre = L, *s;
    while (pre->next != NULL && pre->next->data < x)
        pre = pre->next;
    s = (LNode *)malloc(sizeof(LNode));
    s->data = x;
    s->next = pre->next;
    pre->next = s;
}

void Print(LinkList L) {
    LNode *p;
    for (p = L->next; p != NULL; p = p->next)
        printf("%d ", p->data);
    printf("\n");
}

int main(void) {
    LinkList L = (LinkList)malloc(sizeof(LNode));
    LNode *tail = L, *s;
    int n, i, x;
    printf("请输入元素个数：");
    scanf("%d", &n);
    printf("请输入递增有序的 %d 个元素：", n);
    for (i = 0; i < n; i++) {
        s = (LNode *)malloc(sizeof(LNode));
        scanf("%d", &s->data);
        tail->next = s;
        tail = s;
    }
    tail->next = NULL;
    printf("请输入要插入的元素 X：");
    scanf("%d", &x);
    InsertOrder(L, x);
    printf("插入后：");
    Print(L);
    return 0;
}
