#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PER_SESSION 5       /* 每个时间段前 5 位可以做实验 */

typedef struct QNode {
    int id;
    char name[20];
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front, *rear;    /* 带头结点的链队列 */
} LinkQueue;

LinkQueue Q[3];
const char *TIME_NAME[3] = {"时间一（周一下午）", "时间二（周三下午）", "时间三（周五下午）"};

int Length(LinkQueue *q) {
    int n = 0;
    QNode *p;
    for (p = q->front->next; p != NULL; p = p->next)
        n++;
    return n;
}

void EnQueue(LinkQueue *q, QNode *s) {
    s->next = NULL;
    q->rear->next = s;
    q->rear = s;
}

/* 在三个队列中找学号 id，返回所在时间段，pre 带回它的前驱结点 */
int Find(int id, QNode **pre) {
    int t;
    QNode *p;
    for (t = 0; t < 3; t++)
        for (p = Q[t].front; p->next != NULL; p = p->next)
            if (p->next->id == id) {
                *pre = p;
                return t;
            }
    return -1;
}

/* 1）插入：预约排到对应时间段的队尾 */
void Book(int id, const char *name, int t) {
    QNode *pre, *s;
    if (Find(id, &pre) >= 0) {
        printf("%d 已经预约过了\n", id);
        return;
    }
    s = (QNode *)malloc(sizeof(QNode));
    s->id = id;
    strcpy(s->name, name);
    EnQueue(&Q[t], s);
    printf("%s 预约了%s，排第 %d 位\n", name, TIME_NAME[t], Length(&Q[t]));
}

/* 2）删除：队头的前 5 位出队做实验 */
void DoExperiment(int t) {
    QNode *p;
    int k;
    if (Q[t].front->next == NULL) {
        printf("%s没有人预约\n", TIME_NAME[t]);
        return;
    }
    printf("%s做实验：", TIME_NAME[t]);
    for (k = 0; k < PER_SESSION && Q[t].front->next != NULL; k++) {
        p = Q[t].front->next;
        Q[t].front->next = p->next;
        if (Q[t].rear == p)
            Q[t].rear = Q[t].front;
        printf(k ? "，%d %s" : "%d %s", p->id, p->name);
        free(p);
    }
    printf("\n");
}

/* 3）查询  5）输出：按排队顺序列出名单 */
void PrintQueue(int t) {
    QNode *p;
    printf("%s：", TIME_NAME[t]);
    if (Q[t].front->next == NULL)
        printf("无预约");
    for (p = Q[t].front->next; p != NULL; p = p->next)
        printf(p == Q[t].front->next ? "%d %s" : "，%d %s", p->id, p->name);
    printf("\n");
}

/* 4）修改：从原队列中间摘下结点，排到新时间段的队尾 */
void Modify(int id, int t) {
    QNode *pre, *s;
    int old = Find(id, &pre);
    if (old < 0) {
        printf("没有找到学号 %d 的预约\n", id);
        return;
    }
    s = pre->next;
    pre->next = s->next;
    if (Q[old].rear == s)
        Q[old].rear = pre;
    EnQueue(&Q[t], s);
    printf("%s 的预约改到%s，排第 %d 位\n", s->name, TIME_NAME[t], Length(&Q[t]));
}

int ReadTime(void) {
    int t;
    scanf("%d", &t);
    if (t < 1 || t > 3) {
        printf("时间只能是 1、2、3，按 1 处理\n");
        t = 1;
    }
    return t - 1;
}

int main(void) {
    int op, id, t;
    char name[20];
    for (t = 0; t < 3; t++) {
        Q[t].front = Q[t].rear = (QNode *)malloc(sizeof(QNode));
        Q[t].front->next = NULL;
    }
    printf("【菜单】1 预约  2 做实验  3 查询  4 修改预约  5 输出名单  0 退出\n");
    while (1) {
        printf("请选择操作：");
        if (scanf("%d", &op) != 1 || op == 0)
            break;
        switch (op) {
        case 1:
            printf("请输入学号 姓名 时间(1~3)：");
            scanf("%d %19s", &id, name);
            Book(id, name, ReadTime());
            break;
        case 2: printf("请输入时间(1~3)："); DoExperiment(ReadTime()); break;
        case 3: printf("请输入时间(1~3)："); PrintQueue(ReadTime()); break;
        case 4:
            printf("请输入学号 新的时间(1~3)：");
            scanf("%d", &id);
            Modify(id, ReadTime());
            break;
        case 5:
            for (t = 0; t < 3; t++)
                PrintQueue(t);
            break;
        default: printf("没有这个操作\n");
        }
    }
    return 0;
}
