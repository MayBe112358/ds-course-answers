#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {
    int data;
    struct LNode *next;
} LNode, *LinkList;

/* 尾插法建立带头结点的单链表 */
LinkList Create(int n) {
    LinkList L = (LinkList)malloc(sizeof(LNode));
    LNode *tail = L, *s;
    int i;
    printf("请输入 %d 个元素：", n);
    for (i = 0; i < n; i++) {
        s = (LNode *)malloc(sizeof(LNode));
        scanf("%d", &s->data);
        tail->next = s;
        tail = s;
    }
    tail->next = NULL;
    return L;
}

/* p 依次指向每个结点，q 从 p 出发向后扫描，把与 p 相等的后继结点删掉 */
void DeleteRepeat(LinkList L) {
    LNode *p, *q, *r;
    for (p = L->next; p != NULL; p = p->next) {
        q = p;
        while (q->next != NULL) {
            if (q->next->data == p->data) {
                r = q->next;
                q->next = r->next;
                free(r);
            } else {
                q = q->next;
            }
        }
    }
}

void Print(LinkList L) {
    LNode *p;
    printf("(");
    for (p = L->next; p != NULL; p = p->next)
        printf(p == L->next ? "%d" : ", %d", p->data);
    printf(")\n");
}

int main(void) {
    int n;
    LinkList L;
    printf("请输入元素个数：");
    scanf("%d", &n);
    L = Create(n);
    printf("删除前：");
    Print(L);
    DeleteRepeat(L);
    printf("删除后：");
    Print(L);
    return 0;
}
