#include <stdio.h>
#include <stdlib.h>
#define MAXV 1000           /* 假设元素值在 0 ~ 999 之间 */

typedef struct LNode {
    int data;
    struct LNode *next;
} LNode, *LinkList;

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

/* 一趟扫描：seen[v] 记录值 v 是否已经出现过，出现过的结点直接删除 */
void DeleteRepeat(LinkList L) {
    int seen[MAXV] = {0};
    LNode *pre = L, *p = L->next;
    while (p != NULL) {
        if (seen[p->data]) {
            pre->next = p->next;
            free(p);
        } else {
            seen[p->data] = 1;
            pre = p;
        }
        p = pre->next;
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
