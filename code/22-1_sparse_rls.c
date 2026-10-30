#include <stdio.h>
#define MAXSIZE 100         /* 非零元个数上限 */
#define MAXRC 20            /* 行数、列数上限 */

typedef struct {
    int i, j;               /* 行号、列号，从 1 开始 */
    int e;
} Triple;

typedef struct {
    Triple data[MAXSIZE + 1];   /* data[1..tu]，按行序、列序递增 */
    int rpos[MAXRC + 1];        /* rpos[r]：第 r 行第一个非零元在 data 中的位置 */
    int mu, nu, tu;
} RLSMatrix;

/* 先数出每行的非零元个数，再累加得到每行的起始位置 */
void SetRpos(RLSMatrix *M) {
    int num[MAXRC + 1] = {0}, r, t;
    for (t = 1; t <= M->tu; t++)
        num[M->data[t].i]++;
    M->rpos[1] = 1;
    for (r = 2; r <= M->mu; r++)
        M->rpos[r] = M->rpos[r - 1] + num[r - 1];
}

/* 输入三元组，用直接插入排序整理成行序，允许任意顺序输入 */
void Input(RLSMatrix *M, const char *name) {
    int t, k;
    Triple x;
    printf("请输入矩阵 %s 的行数 列数 非零元个数：", name);
    scanf("%d %d %d", &M->mu, &M->nu, &M->tu);
    printf("请输入 %d 个非零元的 行 列 值：", M->tu);
    for (t = 1; t <= M->tu; t++) {
        scanf("%d %d %d", &x.i, &x.j, &x.e);
        for (k = t - 1; k >= 1 && (M->data[k].i > x.i || (M->data[k].i == x.i && M->data[k].j > x.j)); k--)
            M->data[k + 1] = M->data[k];
        M->data[k + 1] = x;
    }
    SetRpos(M);
}

/* 加减：两个三元组表都按行列有序，像归并一样一趟扫描；sign 为 1 是加，-1 是减 */
void AddSub(RLSMatrix *A, RLSMatrix *B, RLSMatrix *C, int sign) {
    int p = 1, q = 1, cmp;
    Triple t;
    C->mu = A->mu;
    C->nu = A->nu;
    C->tu = 0;
    while (p <= A->tu || q <= B->tu) {
        if (p > A->tu)
            cmp = 1;
        else if (q > B->tu)
            cmp = -1;
        else if (A->data[p].i != B->data[q].i)
            cmp = A->data[p].i - B->data[q].i;
        else
            cmp = A->data[p].j - B->data[q].j;
        if (cmp < 0) {
            t = A->data[p++];
        } else if (cmp > 0) {
            t = B->data[q++];
            t.e *= sign;
        } else {
            t = A->data[p++];
            t.e += sign * B->data[q++].e;
        }
        if (t.e != 0)               /* 相加后为 0 的元素不保留 */
            C->data[++C->tu] = t;
    }
    SetRpos(C);
}

/* 乘法：借助 rpos 直接找到 N 中第 brow 行的非零元，每行结果先累加在 ctemp 中 */
int Mult(RLSMatrix *M, RLSMatrix *N, RLSMatrix *Q) {
    int arow, brow, ccol, p, q, tp, t;
    int ctemp[MAXRC + 1];
    if (M->nu != N->mu)
        return 0;
    Q->mu = M->mu;
    Q->nu = N->nu;
    Q->tu = 0;
    for (arow = 1; arow <= M->mu; arow++) {
        for (ccol = 1; ccol <= Q->nu; ccol++)
            ctemp[ccol] = 0;
        tp = (arow < M->mu) ? M->rpos[arow + 1] : M->tu + 1;
        for (p = M->rpos[arow]; p < tp; p++) {
            brow = M->data[p].j;
            t = (brow < N->mu) ? N->rpos[brow + 1] : N->tu + 1;
            for (q = N->rpos[brow]; q < t; q++)
                ctemp[N->data[q].j] += M->data[p].e * N->data[q].e;
        }
        for (ccol = 1; ccol <= Q->nu; ccol++) {
            if (ctemp[ccol] != 0) {
                Q->tu++;
                Q->data[Q->tu].i = arow;
                Q->data[Q->tu].j = ccol;
                Q->data[Q->tu].e = ctemp[ccol];
            }
        }
    }
    SetRpos(Q);
    return 1;
}

/* 以通常的阵列形式输出 */
void Print(const char *title, RLSMatrix *M) {
    int r, c, t = 1;
    printf("%s =\n", title);
    for (r = 1; r <= M->mu; r++) {
        for (c = 1; c <= M->nu; c++) {
            if (t <= M->tu && M->data[t].i == r && M->data[t].j == c)
                printf("%5d", M->data[t++].e);
            else
                printf("%5d", 0);
        }
        printf("\n");
    }
}

int main(void) {
    RLSMatrix A, B, C;
    Input(&A, "A");
    Input(&B, "B");
    if (A.mu == B.mu && A.nu == B.nu) {
        AddSub(&A, &B, &C, 1);
        Print("A + B", &C);
        AddSub(&A, &B, &C, -1);
        Print("A - B", &C);
    } else {
        printf("A、B 行列数不同，不能相加减\n");
    }
    if (Mult(&A, &B, &C))
        Print("A × B", &C);
    else
        printf("A 的列数不等于 B 的行数，不能相乘\n");
    return 0;
}
