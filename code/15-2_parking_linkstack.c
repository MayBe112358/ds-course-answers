#include <stdio.h>
#include <stdlib.h>
#define QSIZE 21            /* 循环队列，最多 20 辆车在便道等候 */

typedef struct {
    int plate;
    int time;
} Car;

typedef struct SNode {
    Car car;
    struct SNode *next;
} SNode;

typedef struct {
    SNode *top;             /* 链栈，栈顶在表头 */
    int len;
} LinkStack;

typedef struct {
    Car data[QSIZE];
    int front, rear;
} SqQueue;

int N, price;

void Push(LinkStack *S, Car c) {
    SNode *s = (SNode *)malloc(sizeof(SNode));
    s->car = c;
    s->next = S->top;
    S->top = s;
    S->len++;
}

Car Pop(LinkStack *S) {
    SNode *p = S->top;
    Car c = p->car;
    S->top = p->next;
    free(p);
    S->len--;
    return c;
}

int QueueLength(SqQueue *Q) { return (Q->rear - Q->front + QSIZE) % QSIZE; }

void Arrive(LinkStack *park, SqQueue *road, Car c) {
    if (park->len < N) {
        Push(park, c);
        printf("%d 号车停在车位 %d\n", c.plate, park->len);
    } else if ((road->rear + 1) % QSIZE == road->front) {
        printf("便道已满，%d 号车无法等候\n", c.plate);
    } else {
        road->data[road->rear] = c;
        road->rear = (road->rear + 1) % QSIZE;
        printf("%d 号车在便道第 %d 位等候\n", c.plate, QueueLength(road));
    }
}

/* 只用栈操作找车：不是要找的车就挪到临时栈，找到或停车场空了为止 */
void Leave(LinkStack *park, LinkStack *temp, SqQueue *road, int plate, int t) {
    Car c;
    int found = 0;
    while (park->len > 0) {
        c = Pop(park);
        if (c.plate == plate) {
            found = 1;
            break;
        }
        Push(temp, c);
    }
    if (found)
        printf("%d 号车离开，停留 %d，交费 %d\n", plate, t - c.time, (t - c.time) * price);
    while (temp->len > 0)
        Push(park, Pop(temp));
    if (!found) {
        printf("%d 号车不在停车场内\n", plate);
        return;
    }
    if (QueueLength(road) > 0) {
        c = road->data[road->front];
        road->front = (road->front + 1) % QSIZE;
        c.time = t;
        Push(park, c);
        printf("%d 号车从便道进入车位 %d\n", c.plate, park->len);
    }
}

int main(void) {
    LinkStack park = {NULL, 0}, temp = {NULL, 0};
    SqQueue road;
    Car c;
    char op;
    road.front = road.rear = 0;

    printf("请输入停车场容量 N 和单位时间停车费：");
    scanf("%d %d", &N, &price);
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
    while (park.len > 0)
        Pop(&park);
    return 0;
}
