#include <stdio.h>
#define MAXSIZE 100
#define MAXRC 20

typedef struct {
    int i, j, e;
} Triple;

typedef struct {
    Triple data[MAXSIZE + 1];   /* data[1..tu]，按行序、列序递增 */
    int mu, nu, tu;
} TSMatrix;

void Input(TSMatrix *M, const char *name) {
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
}

/* 快速转置：先数出每列的非零元个数，得到每列在转置结果中的起始位置 cpot，再一趟放到位 */
void FastTranspose(TSMatrix *M, TSMatrix *T) {
    int num[MAXRC + 1] = {0}, cpot[MAXRC + 1], col, p, q;
    T->mu = M->nu;
    T->nu = M->mu;
    T->tu = M->tu;
    for (p = 1; p <= M->tu; p++)
        num[M->data[p].j]++;
    cpot[1] = 1;
    for (col = 2; col <= M->nu; col++)
        cpot[col] = cpot[col - 1] + num[col - 1];
    for (p = 1; p <= M->tu; p++) {
        col = M->data[p].j;
        q = cpot[col]++;
        T->data[q].i = M->data[p].j;
        T->data[q].j = M->data[p].i;
        T->data[q].e = M->data[p].e;
    }
}

/* start[r]：第 r 行第一个非零元的位置，start[mu+1] = tu+1 */
void RowStart(TSMatrix *M, int start[]) {
    int r, t = 1;
    for (r = 1; r <= M->mu + 1; r++) {
        while (t <= M->tu && M->data[t].i < r)
            t++;
        start[r] = t;
    }
}

/* C[i][j] 是 A 第 i 行与 B 第 j 列的点积。B 转置后，第 j 列变成 BT 的第 j 行，
   两行都按列号有序，像归并一样找列号相同的元素相乘 */
int Multiply(TSMatrix *A, TSMatrix *B, TSMatrix *C) {
    TSMatrix BT;
    int sa[MAXRC + 2], sb[MAXRC + 2], i, j, p, q, sum;
    if (A->nu != B->mu)
        return 0;
    FastTranspose(B, &BT);
    RowStart(A, sa);
    RowStart(&BT, sb);
    C->mu = A->mu;
    C->nu = B->nu;
    C->tu = 0;
    for (i = 1; i <= A->mu; i++) {
        for (j = 1; j <= BT.mu; j++) {
            sum = 0;
            p = sa[i];
            q = sb[j];
            while (p < sa[i + 1] && q < sb[j + 1]) {
                if (A->data[p].j < BT.data[q].j)
                    p++;
                else if (A->data[p].j > BT.data[q].j)
                    q++;
                else
                    sum += A->data[p++].e * BT.data[q++].e;
            }
            if (sum != 0) {
                C->tu++;
                C->data[C->tu].i = i;
                C->data[C->tu].j = j;
                C->data[C->tu].e = sum;
            }
        }
    }
    return 1;
}

void Print(TSMatrix *M) {
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
    TSMatrix A, B, C;
    Input(&A, "A");
    Input(&B, "B");
    if (!Multiply(&A, &B, &C)) {
        printf("A 的列数不等于 B 的行数，不能相乘\n");
        return 1;
    }
    Print(&C);
    return 0;
}
