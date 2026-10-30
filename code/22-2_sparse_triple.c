#include <stdio.h>
#define MAXSIZE 100
#define MAXRC 20

typedef struct {
    int i, j;
    int e;
} Triple;

typedef struct {
    Triple data[MAXSIZE + 1];   /* 普通三元组表，不带 rpos */
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

void AddSub(TSMatrix *A, TSMatrix *B, TSMatrix *C, int sign) {
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
        if (t.e != 0)
            C->data[++C->tu] = t;
    }
}

/* 乘法：没有 rpos，A 的每个非零元 (i,k) 都要扫描 B 的全部非零元，挑出第 k 行的相乘 */
int Mult(TSMatrix *A, TSMatrix *B, TSMatrix *C) {
    int p, q, row, col, ctemp[MAXRC + 1];
    if (A->nu != B->mu)
        return 0;
    C->mu = A->mu;
    C->nu = B->nu;
    C->tu = 0;
    p = 1;
    for (row = 1; row <= A->mu; row++) {
        for (col = 1; col <= C->nu; col++)
            ctemp[col] = 0;
        for (; p <= A->tu && A->data[p].i == row; p++)     /* A 按行有序，第 row 行是连续的一段 */
            for (q = 1; q <= B->tu; q++)
                if (B->data[q].i == A->data[p].j)
                    ctemp[B->data[q].j] += A->data[p].e * B->data[q].e;
        for (col = 1; col <= C->nu; col++) {
            if (ctemp[col] != 0) {
                C->tu++;
                C->data[C->tu].i = row;
                C->data[C->tu].j = col;
                C->data[C->tu].e = ctemp[col];
            }
        }
    }
    return 1;
}

void Print(const char *title, TSMatrix *M) {
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
    TSMatrix A, B, C;
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
