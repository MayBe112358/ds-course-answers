#include <stdio.h>
#define MAXP 50
#define MAXF 9
#define QSIZE (MAXP + 1)

enum { IDLE, UP, DOWN };

typedef struct {
    int id, t, from, to;    /* 编号、到达时刻、出发楼层、目标楼层 */
    int boardT, leaveT;     /* 进电梯、出电梯的时刻 */
} Passenger;

typedef struct {
    int data[QSIZE];        /* 存乘客下标 */
    int front, rear;
} SqQueue;

Passenger P[MAXP];
int np, floors;
SqQueue upQ[MAXF + 1], downQ[MAXF + 1];     /* 每层的上行、下行等候队列 */
int inCar[MAXP], nCar = 0;                  /* 电梯里的乘客 */
int cur = 1, dir = IDLE, delivered = 0;

int QEmpty(SqQueue *q) { return q->front == q->rear; }
void EnQueue(SqQueue *q, int x) { q->data[q->rear] = x; q->rear = (q->rear + 1) % QSIZE; }
int DeQueue(SqQueue *q) { int x = q->data[q->front]; q->front = (q->front + 1) % QSIZE; return x; }

/* 当前楼层之上（above 为 1）或之下是否还有要去的地方：电梯里乘客的目的地，或有人等候的楼层 */
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

/* 到达目的楼层的乘客走出电梯 */
int Unload(int t) {
    int k = 0, act = 0, x;
    while (k < nCar) {
        x = inCar[k];
        if (P[x].to == cur) {
            P[x].leaveT = t;
            printf("t=%3d  %d 楼：乘客 %d 走出电梯\n", t, cur, P[x].id);
            inCar[k] = inCar[--nCar];
            delivered++;
            act = 1;
        } else {
            k++;
        }
    }
    return act;
}

/* 本层与运行方向相同的等候者依次进入电梯 */
int Board(int t) {
    SqQueue *q = dir == UP ? &upQ[cur] : dir == DOWN ? &downQ[cur] : NULL;
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

/* 电梯在时刻 t 的一个动作：开门上下客，或者移动一层；什么也没做返回 0 */
int ElevatorStep(int t) {
    int act = Unload(t);
    if (dir == IDLE) {          /* 空闲：本层有人就按他的方向走，否则去有请求的一边 */
        if (!QEmpty(&upQ[cur])) dir = UP;
        else if (!QEmpty(&downQ[cur])) dir = DOWN;
        else if (HasRequest(1)) dir = UP;
        else if (HasRequest(0)) dir = DOWN;
    }
    act |= Board(t);
    if (!act && dir == UP && !HasRequest(1)) {          /* 上面没有请求了，掉头或停下 */
        dir = (HasRequest(0) || !QEmpty(&downQ[cur])) ? DOWN : IDLE;
        act |= Board(t);
    } else if (!act && dir == DOWN && !HasRequest(0)) {
        dir = (HasRequest(1) || !QEmpty(&upQ[cur])) ? UP : IDLE;
        act |= Board(t);
    }
    if (!act && dir != IDLE) {  /* 没有人上下，沿运行方向移动一层 */
        cur += dir == UP ? 1 : -1;
        act = 1;
    }
    return act;
}

int main(void) {
    int i, j, t, ticks = 0, next = 0;
    double wait = 0, ride = 0;
    Passenger x;
    printf("请输入楼层数（2~%d）：", MAXF);
    scanf("%d", &floors);
    printf("请输入乘客人数：");
    scanf("%d", &np);
    if (floors < 2 || floors > MAXF || np < 1 || np > MAXP) {
        printf("楼层数或人数超出范围\n");
        return 1;
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
    }
    for (i = 1; i < np; i++) {          /* 按到达时刻排序（稳定） */
        x = P[i];
        for (j = i - 1; j >= 0 && P[j].t > x.t; j--)
            P[j + 1] = P[j];
        P[j + 1] = x;
    }
    for (i = 1; i <= floors; i++)
        upQ[i].front = upQ[i].rear = downQ[i].front = downQ[i].rear = 0;

    /* 固定步长：时钟每次推进 1 个单位，每个时刻都检查一遍 */
    for (t = 0; delivered < np; t++) {
        while (next < np && P[next].t == t)
            Arrive(next++, t);
        ElevatorStep(t);
        ticks++;
    }

    for (i = 0; i < np; i++) {
        wait += P[i].boardT - P[i].t;
        ride += P[i].leaveT - P[i].boardT;
    }
    printf("全部送达。平均等候 %.1f，平均乘梯 %.1f\n", wait / np, ride / np);
    printf("时钟共推进 %d 次\n", ticks);
    return 0;
}
