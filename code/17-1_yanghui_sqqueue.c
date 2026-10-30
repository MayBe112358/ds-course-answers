#include <stdio.h>
#define MAXQSIZE 64         /* 队列里最多同时有两行，n<=30 时够用 */

typedef struct {
    int data[MAXQSIZE];
    int front, rear;        /* front 指向队头，rear 指向队尾的下一个位置 */
} SqQueue;

void InitQueue(SqQueue *Q) { Q->front = Q->rear = 0; }

void EnQueue(SqQueue *Q, int e) {
    Q->data[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAXQSIZE;
}

int DeQueue(SqQueue *Q) {
    int e = Q->data[Q->front];
    Q->front = (Q->front + 1) % MAXQSIZE;
    return e;
}

int GetHead(SqQueue *Q) { return Q->data[Q->front]; }

int main(void) {
    SqQueue Q;
    int n, k, j, s;
    printf("请输入行数 n（7<=n<=30）：");
    scanf("%d", &n);
    InitQueue(&Q);
    EnQueue(&Q, 1);                     /* 第 1 行 */
    for (k = 1; k <= n; k++) {
        printf("%*s", (n - k) * 3, ""); /* 行首空格，排成三角形 */
        if (k < n)
            EnQueue(&Q, 1);             /* 第 k+1 行开头的 1 */
        for (j = 1; j <= k; j++) {
            s = DeQueue(&Q);            /* 第 k 行第 j 个数 */
            printf("%6d", s);
            if (k < n && j < k)         /* 与右边相邻的数相加，得到下一行的数 */
                EnQueue(&Q, s + GetHead(&Q));
        }
        if (k < n)
            EnQueue(&Q, 1);             /* 第 k+1 行末尾的 1 */
        printf("\n");
    }
    return 0;
}
