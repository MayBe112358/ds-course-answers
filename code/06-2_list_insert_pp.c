#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {
    int data;
    struct LNode *next;
} LNode;

/* 不带头结点。pp 指向“要被修改的那个指针”：开始是 head 本身，
   之后是某个结点的 next 域，所以插在表头和插在中间是同一段代码 */
void InsertOrder(LNode **head, int x) {
    LNode **pp = head, *s;
    while (*pp != NULL && (*pp)->data < x)
        pp = &(*pp)->next;
    s = (LNode *)malloc(sizeof(LNode));
    s->data = x;
    s->next = *pp;
    *pp = s;
}

void Print(LNode *head) {
    for (; head != NULL; head = head->next)
        printf("%d ", head->data);
    printf("\n");
}

int main(void) {
    LNode *head = NULL, **tail = &head, *s;
    int n, i, x;
    printf("请输入元素个数：");
    scanf("%d", &n);
    printf("请输入递增有序的 %d 个元素：", n);
    for (i = 0; i < n; i++) {
        s = (LNode *)malloc(sizeof(LNode));
        scanf("%d", &s->data);
        *tail = s;                  /* 建表也用二级指针做尾插 */
        tail = &s->next;
    }
    *tail = NULL;
    printf("请输入要插入的元素 X：");
    scanf("%d", &x);
    InsertOrder(&head, x);
    printf("插入后：");
    Print(head);
    return 0;
}
