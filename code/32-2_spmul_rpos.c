#include <stdio.h>
#define MAXSIZE 100
#define MAXRC 20

typedef struct {
    int i, j, e;
} Triple;

typedef struct {
    Triple data[MAXSIZE + 1];
    int rpos[MAXRC + 2];    /* rpos[r]：第 r 行第一个非零元的位置，rpos[mu+1] = tu+1 */
    int mu, nu, tu;
} RLSMatrix;

void SetRpos(RLSMatrix *M) {
    int num[MAXRC + 1] = {0}, r, t;
    for (t = 1; t <= M->tu; t++)
        num[M->data[t].i]++;
    M->rpos[1] = 1;
    for (r = 2; r <= M->mu + 1; r++)
        M->rpos[r] = M->rpos[r - 1] + num[r - 1];
}

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

/* 逐行求 C：A 第 arow 行的每个非零元 (arow, k) 与 B 第 k 行的全部非零元相乘，
   乘积按列号累加到 ctemp，这一行算完再压缩成三元组 */
int Multiply(RLSMatrix *A, RLSMatrix *B, RLSMatrix *C) {
    int arow, brow, col, p, q, ctemp[MAXRC + 1];
    if (A->nu != B->mu)
        return 0;
    C->mu = A->mu;
    C->nu = B->nu;
    C->tu = 0;
    for (arow = 1; arow <= A->mu; arow++) {
        for (col = 1; col <= C->nu; col++)
            ctemp[col] = 0;
        for (p = A->rpos[arow]; p < A->rpos[arow + 1]; p++) {
            brow = A->data[p].j;
            for (q = B->rpos[brow]; q < B->rpos[brow + 1]; q++)
                ctemp[B->data[q].j] += A->data[p].e * B->data[q].e;
        }
        for (col = 1; col <= C->nu; col++) {
            if (ctemp[col] != 0) {
                C->tu++;
                C->data[C->tu].i = arow;
                C->data[C->tu].j = col;
                C->data[C->tu].e = ctemp[col];
            }
        }
    }
    SetRpos(C);
    return 1;
}

void Print(RLSMatrix *M) {
    int r, c, t;
    printf("C 的三元组表：");
    for (t = 1; t <= M->tu; t++)
        printf("(%d,%d,%d) ", M->data[t].i, M->data[t].j, M->data[t].e);
    printf("\nC =\n");
    for (r = 1, t = 1; r <= M->mu; r++) {
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
    if (!Multiply(&A, &B, &C)) {
        printf("A 的列数不等于 B 的行数，不能相乘\n");
        return 1;
    }
    Print(&C);
    return 0;
}
