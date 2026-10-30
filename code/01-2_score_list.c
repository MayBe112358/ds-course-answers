#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id;
    char name[20];
    int score[3];
    int total;
    struct Node *next;
} Node;

/* 插到所有总分不低于它的结点之后，链表始终按总分从高到低有序 */
void InsertSorted(Node *head, Node *s) {
    Node *pre = head;
    while (pre->next != NULL && pre->next->total >= s->total)
        pre = pre->next;
    s->next = pre->next;
    pre->next = s;
}

int main(void) {
    Node *head = (Node *)malloc(sizeof(Node)), *s, *p;
    int n, i, rank = 0, prev = -1;
    head->next = NULL;
    printf("请输入学生人数：");
    scanf("%d", &n);
    for (i = 1; i <= n; i++) {
        s = (Node *)malloc(sizeof(Node));
        printf("请输入第 %d 位同学的学号 姓名 语文 数学 英语：", i);
        scanf("%d %19s %d %d %d", &s->id, s->name, &s->score[0], &s->score[1], &s->score[2]);
        s->total = s->score[0] + s->score[1] + s->score[2];
        InsertSorted(head, s);          /* 边录入边排序 */
    }

    printf("名次\t学号\t姓名\t语文\t数学\t英语\t总分\n");
    for (p = head->next, i = 1; p != NULL; p = p->next, i++) {
        if (p->total != prev)
            rank = i;                   /* 总分相同名次并列 */
        prev = p->total;
        printf("%d\t%d\t%s\t%d\t%d\t%d\t%d\n", rank, p->id, p->name,
               p->score[0], p->score[1], p->score[2], p->total);
    }

    while (head != NULL) {
        p = head;
        head = head->next;
        free(p);
    }
    return 0;
}
