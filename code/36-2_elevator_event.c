#include <stdio.h>
#include <stdlib.h>
#define MAXP 50
#define MAXF 9

enum { IDLE, UP, DOWN };
enum { ARRIVE, STEP };      /* 事件类型：乘客到达、电梯动作 */

typedef struct {
    int id, t, from, to;
    int boardT, leaveT;
} Passenger;

typedef struct QNode {
    int who;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front, *rear;    /* 带头结点的链队列 */
} LinkQueue;

typedef struct Event {
    int time, type, who;
    struct Event *next;
} Event;

Passenger P[MAXP];
int np, floors;
LinkQueue upQ[MAXF + 1], downQ[MAXF + 1];
int inCar[MAXP], nCar = 0;
int cur = 1, dir = IDLE;
Event *events = NULL;       /* 事件表，按时刻有序 */

void InitQueue(LinkQueue *q) {
    q->front = q->rear = (QNode *)malloc(sizeof(QNode));
    q->front->next = NULL;
}

int QEmpty(LinkQueue *q) { return q->front == q->rear; }

void EnQueue(LinkQueue *q, int x) {
    QNode *s = (QNode *)malloc(sizeof(QNode));
    s->who = x;
    s->next = NULL;
    q->rear->next = s;
    q->rear = s;
}

int DeQueue(LinkQueue *q) {
    QNode *p = q->front->next;
    int x = p->who;
    q->front->next = p->next;
    if (q->rear == p)
        q->rear = q->front;
    free(p);
    return x;
}

/* 按（时刻，类型，乘客）有序插入事件表；同一时刻乘客到达排在电梯动作之前 */
void Schedule(int time, int type, int who) {
    Event **pp = &events, *e = (Event *)malloc(sizeof(Event));
    e->time = time;
    e->type = type;
    e->who = who;
    while (*pp != NULL && ((*pp)->time < time ||
           ((*pp)->time == time && ((*pp)->type < type || ((*pp)->type == type && (*pp)->who <= who)))))
        pp = &(*pp)->next;
    e->next = *pp;
    *pp = e;
}

int HasRequest(int above) {
    int k, f;
    for (k = 0; k < nCar; k++)
        if (above ? P[inCar[k]].to > cur : P[inCar[k]].to < cur)
            return 1;
    for (f = 1; f <= floors; f++)
        if ((above ? f > cur : f < cur) && (!QEmpty(&upQ[f]) || !QEmpty(&downQ[f])))
            return 1;
    return 0;
}

void Arrive(int k, int t) {
    if (P[k].to > P[k].from)
        EnQueue(&upQ[P[k].from], k);
    else
        EnQueue(&downQ[P[k].from], k);
    printf("t=%3d  乘客 %d 在 %d 楼等候，要去 %d 楼\n", t, P[k].id, P[k].from, P[k].to);
}

int Unload(int t) {
    int k = 0, act = 0, x;
    while (k < nCar) {
        x = inCar[k];
        if (P[x].to == cur) {
            P[x].leaveT = t;
            printf("t=%3d  %d 楼：乘客 %d 走出电梯\n", t, cur, P[x].id);
            inCar[k] = inCar[--nCar];
            act = 1;
        } else {
            k++;
        }
    }
    return act;
}

int Board(int t) {
    LinkQueue *q = dir == UP ? &upQ[cur] : dir == DOWN ? &downQ[cur] : NULL;
    int act = 0, x;
    while (q != NULL && !QEmpty(q)) {
        x = DeQueue(q);
        P[x].boardT = t;
        inCar[nCar++] = x;
        printf("t=%3d  %d 楼：乘客 %d 进入电梯（去 %d 楼）\n", t, cur, P[x].id, P[x].to);
        act = 1;
    }
    return act;
}

/* 电梯的调度规则与解法一相同 */
int ElevatorStep(int t) {
    int act = Unload(t);
    if (dir == IDLE) {
        if (!QEmpty(&upQ[cur])) dir = UP;
        else if (!QEmpty(&downQ[cur])) dir = DOWN;
        else if (HasRequest(1)) dir = UP;
        else if (HasRequest(0)) dir = DOWN;
    }
    act |= Board(t);
    if (!act && dir == UP && !HasRequest(1)) {
        dir = (HasRequest(0) || !QEmpty(&downQ[cur])) ? DOWN : IDLE;
        act |= Board(t);
    } else if (!act && dir == DOWN && !HasRequest(0)) {
        dir = (HasRequest(1) || !QEmpty(&upQ[cur])) ? UP : IDLE;
        act |= Board(t);
    }
    if (!act && dir != IDLE) {
        cur += dir == UP ? 1 : -1;
        act = 1;
    }
    return act;
}

int main(void) {
    int i, now, stepPending = 0, count = 0;
    double wait = 0, ride = 0;
    Event *e;
    printf("请输入楼层数（2~%d）：", MAXF);
    scanf("%d", &floors);
    printf("请输入乘客人数：");
    scanf("%d", &np);
    if (floors < 2 || floors > MAXF || np < 1 || np > MAXP) {
        printf("楼层数或人数超出范围\n");
        return 1;
    }
    for (i = 1; i <= floors; i++) {
        InitQueue(&upQ[i]);
        InitQueue(&downQ[i]);
    }
    for (i = 0; i < np; i++) {
        printf("请输入第 %d 位乘客的到达时刻 出发楼层 目标楼层：", i + 1);
        scanf("%d %d %d", &P[i].t, &P[i].from, &P[i].to);
        P[i].id = i + 1;
        if (P[i].t < 0 || P[i].from < 1 || P[i].from > floors || P[i].to < 1 ||
            P[i].to > floors || P[i].from == P[i].to) {
            printf("第 %d 位乘客的数据不合法\n", i + 1);
            return 1;
        }
        Schedule(P[i].t, ARRIVE, i);
    }

    /* 下一事件推进：时钟直接跳到事件表中最早的事件。电梯有事可做时，
       每个动作结束后预约下一时刻的动作；空闲时不预约，等有人到达再唤醒 */
    while (events != NULL) {
        e = events;
        events = e->next;
        now = e->time;
        count++;
        if (e->type == ARRIVE) {
            Arrive(e->who, now);
            if (!stepPending) {
                Schedule(now, STEP, 0);
                stepPending = 1;
            }
        } else {
            stepPending = 0;
            if (ElevatorStep(now)) {
                Schedule(now + 1, STEP, 0);
                stepPending = 1;
            }
        }
        free(e);
    }

    for (i = 0; i < np; i++) {
        wait += P[i].boardT - P[i].t;
        ride += P[i].leaveT - P[i].boardT;
    }
    printf("全部送达。平均等候 %.1f，平均乘梯 %.1f\n", wait / np, ride / np);
    printf("共处理 %d 个事件\n", count);
    return 0;
}
