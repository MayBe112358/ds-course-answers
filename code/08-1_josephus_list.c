#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int no;                 /* 编号 */
    int pwd;                /* 密码 */
    struct Node *next;
} Node;

/* 建立 n 个结点的循环单链表，返回尾结点（尾结点的 next 是 1 号） */
Node *CreateRing(int n) {
    Node *head = NULL, *tail = NULL, *s;
    int i;
    for (i = 1; i <= n; i++) {
        s = (Node *)malloc(sizeof(Node));
        s->no = i;
        printf("请输入第 %d 个人的密码：", i);
        scanf("%d", &s->pwd);
        if (head == NULL)
            head = s;
        else
            tail->next = s;
        tail = s;
    }
    tail->next = head;
    return tail;
}

void Josephus(Node *tail, int n, int m) {
    Node *p = tail, *q;     /* p 始终指向下一个报数者的前驱 */
    int left = n, i, step;
    printf("出列顺序：");
    while (left > 0) {
        step = (m - 1) % left;      /* 绕满整圈没有意义，取余减少空转 */
        for (i = 0; i < step; i++)
            p = p->next;
        q = p->next;                /* q 报到 m，出列 */
        printf("%d ", q->no);
        m = q->pwd;
        p->next = q->next;
        free(q);
        left--;
    }
    printf("\n");
}

int main(void) {
    int n, m;
    printf("请输入人数 n 和初始报数上限 m：");
    scanf("%d %d", &n, &m);
    Josephus(CreateRing(n), n, m);
    return 0;
}
