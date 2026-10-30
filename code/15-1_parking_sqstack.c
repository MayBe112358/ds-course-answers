#include <stdio.h>
#include <stdlib.h>
#define MAXN 20

typedef struct {
    int plate;              /* 车牌号 */
    int time;               /* 进入停车场的时刻 */
} Car;

typedef struct {
    Car data[MAXN];
    int top;
} SqStack;                  /* 停车场和临时让路的栈 */

typedef struct QNode {
    Car car;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front, *rear;    /* 带头结点 */
    int len;
} LinkQueue;                /* 便道 */

int N, price;

void Push(SqStack *S, Car c) { S->data[++S->top] = c; }
Car Pop(SqStack *S) { return S->data[S->top--]; }

void EnQueue(LinkQueue *Q, Car c) {
    QNode *s = (QNode *)malloc(sizeof(QNode));
    s->car = c;
    s->next = NULL;
    Q->rear->next = s;
    Q->rear = s;
    Q->len++;
}

Car DeQueue(LinkQueue *Q) {
    QNode *p = Q->front->next;
    Car c = p->car;
    Q->front->next = p->next;
    if (Q->rear == p)
        Q->rear = Q->front;
    free(p);
    Q->len--;
    return c;
}

void Arrive(SqStack *park, LinkQueue *road, Car c) {
    if (park->top < N - 1) {
        Push(park, c);
        printf("%d 号车停在车位 %d\n", c.plate, park->top + 1);
    } else {
        EnQueue(road, c);
        printf("%d 号车在便道第 %d 位等候\n", c.plate, road->len);
    }
}

void Leave(SqStack *park, SqStack *temp, LinkQueue *road, int plate, int t) {
    Car c;
    int i, found = 0;
    for (i = 0; i <= park->top; i++)
        if (park->data[i].plate == plate)
            found = 1;
    if (!found) {
        printf("%d 号车不在停车场内\n", plate);
        return;
    }
    while (park->data[park->top].plate != plate)
        Push(temp, Pop(park));          /* 在它之后进来的车先退出去让路 */
    c = Pop(park);
    printf("%d 号车离开，停留 %d，交费 %d\n", plate, t - c.time, (t - c.time) * price);
    while (temp->top >= 0)
        Push(park, Pop(temp));          /* 让路的车按原次序开回来 */
    if (road->len > 0) {
        c = DeQueue(road);
        c.time = t;                     /* 从进入停车场这一刻开始计费 */
        Push(park, c);
        printf("%d 号车从便道进入车位 %d\n", c.plate, park->top + 1);
    }
}

int main(void) {
    SqStack park, temp;
    LinkQueue road;
    Car c;
    char op;
    park.top = temp.top = -1;
    road.front = road.rear = (QNode *)malloc(sizeof(QNode));
    road.front->next = NULL;
    road.len = 0;

    printf("请输入停车场容量 N 和单位时间停车费：");
    scanf("%d %d", &N, &price);
    if (N > MAXN)
        N = MAXN;
    while (1) {
        printf("请输入操作（A 到达，D 离开，E 结束）、车牌号、时刻：");
        if (scanf(" %c %d %d", &op, &c.plate, &c.time) != 3 || op == 'E')
            break;
        if (op == 'A')
            Arrive(&park, &road, c);
        else if (op == 'D')
            Leave(&park, &temp, &road, c.plate, c.time);
        else
            printf("操作只能是 A、D 或 E\n");
    }
    return 0;
}
