#include <stdio.h>
#include <string.h>
#define QSIZE 31            /* 循环队列，每个时间段最多 30 人 */
#define PER_SESSION 5

typedef struct {
    int id;
    char name[20];
} Student;

typedef struct {
    Student data[QSIZE];
    int front, rear;
} SqQueue;

SqQueue Q[3];
const char *TIME_NAME[3] = {"时间一（周一下午）", "时间二（周三下午）", "时间三（周五下午）"};

int Length(SqQueue *q) { return (q->rear - q->front + QSIZE) % QSIZE; }

int EnQueue(SqQueue *q, Student s) {
    if ((q->rear + 1) % QSIZE == q->front)
        return 0;
    q->data[q->rear] = s;
    q->rear = (q->rear + 1) % QSIZE;
    return 1;
}

/* 找学号 id：返回时间段，k 带回它在队列中的序号（0 表示队头） */
int Find(int id, int *k) {
    int t, i;
    for (t = 0; t < 3; t++)
        for (i = 0; i < Length(&Q[t]); i++)
            if (Q[t].data[(Q[t].front + i) % QSIZE].id == id) {
                *k = i;
                return t;
            }
    return -1;
}

void Book(int id, const char *name, int t) {
    Student s;
    int k;
    if (Find(id, &k) >= 0) {
        printf("%d 已经预约过了\n", id);
        return;
    }
    s.id = id;
    strcpy(s.name, name);
    if (!EnQueue(&Q[t], s)) {
        printf("%s已约满\n", TIME_NAME[t]);
        return;
    }
    printf("%s 预约了%s，排第 %d 位\n", name, TIME_NAME[t], Length(&Q[t]));
}

void DoExperiment(int t) {
    int k;
    Student s;
    if (Length(&Q[t]) == 0) {
        printf("%s没有人预约\n", TIME_NAME[t]);
        return;
    }
    printf("%s做实验：", TIME_NAME[t]);
    for (k = 0; k < PER_SESSION && Length(&Q[t]) > 0; k++) {
        s = Q[t].data[Q[t].front];
        Q[t].front = (Q[t].front + 1) % QSIZE;
        printf(k ? "，%d %s" : "%d %s", s.id, s.name);
    }
    printf("\n");
}

void PrintQueue(int t) {
    int i;
    Student *s;
    printf("%s：", TIME_NAME[t]);
    if (Length(&Q[t]) == 0)
        printf("无预约");
    for (i = 0; i < Length(&Q[t]); i++) {
        s = &Q[t].data[(Q[t].front + i) % QSIZE];
        printf(i ? "，%d %s" : "%d %s", s->id, s->name);
    }
    printf("\n");
}

/* 修改：把排在第 k 位的学生取出，后面的人依次前移一位，再排到新队列的队尾 */
void Modify(int id, int t) {
    int k, old = Find(id, &k), i, n;
    Student s;
    if (old < 0) {
        printf("没有找到学号 %d 的预约\n", id);
        return;
    }
    if (old != t && (Q[t].rear + 1) % QSIZE == Q[t].front) {
        printf("%s已约满\n", TIME_NAME[t]);
        return;
    }
    n = Length(&Q[old]);
    s = Q[old].data[(Q[old].front + k) % QSIZE];
    for (i = k; i < n - 1; i++)
        Q[old].data[(Q[old].front + i) % QSIZE] = Q[old].data[(Q[old].front + i + 1) % QSIZE];
    Q[old].rear = (Q[old].rear - 1 + QSIZE) % QSIZE;
    EnQueue(&Q[t], s);
    printf("%s 的预约改到%s，排第 %d 位\n", s.name, TIME_NAME[t], Length(&Q[t]));
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
    for (t = 0; t < 3; t++)
        Q[t].front = Q[t].rear = 0;
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
