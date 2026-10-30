#include <stdio.h>
#include <stdlib.h>

typedef struct QNode {
    int data;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front, *rear;    /* 带头结点，front 指向头结点 */
} LinkQueue;

void InitQueue(LinkQueue *Q) {
    Q->front = Q->rear = (QNode *)malloc(sizeof(QNode));
    Q->front->next = NULL;
}

void EnQueue(LinkQueue *Q, int e) {
    QNode *s = (QNode *)malloc(sizeof(QNode));
    s->data = e;
    s->next = NULL;
    Q->rear->next = s;
    Q->rear = s;
}

int DeQueue(LinkQueue *Q) {
    QNode *p = Q->front->next;
    int e = p->data;
    Q->front->next = p->next;
    if (Q->rear == p)
        Q->rear = Q->front;
    free(p);
    return e;
}

int main(void) {
    LinkQueue Q;
    int n, k, s, prev;
    printf("请输入行数 n（n>=7）：");
    scanf("%d", &n);
    InitQueue(&Q);
    EnQueue(&Q, 1);
    EnQueue(&Q, 0);                     /* 0 是每一行末尾的分隔符 */
    for (k = 1; k <= n; k++) {
        printf("%*s", (n - k) * 3, "");
        prev = 0;                       /* 把行首左边看成也有一个 0 */
        do {
            s = DeQueue(&Q);
            EnQueue(&Q, prev + s);      /* 相邻两数之和就是下一行的数 */
            if (s != 0)
                printf("%6d", s);
            prev = s;
        } while (s != 0);
        EnQueue(&Q, 0);                 /* 新的一行同样以 0 结尾 */
        printf("\n");
    }
    return 0;
}
