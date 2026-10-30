#include <stdio.h>
#define MAXE 100            /* 指数范围 0 ~ 100 */

typedef struct {
    double coef[MAXE + 1];  /* coef[e] 就是 x^e 的系数 */
    int high;               /* 最高次数，-1 表示零多项式 */
} Poly;

void FixHigh(Poly *P) {
    P->high = MAXE;
    while (P->high >= 0 && P->coef[P->high] == 0)
        P->high--;
}

/* 系数直接累加到对应下标，同类项自动合并，也不用排序 */
void Create(Poly *P, const char *name) {
    int n, i, e;
    double c;
    for (e = 0; e <= MAXE; e++)
        P->coef[e] = 0;
    printf("请输入多项式 %s 的项数：", name);
    scanf("%d", &n);
    printf("请输入 %d 组 系数 指数：", n);
    for (i = 0; i < n; i++) {
        scanf("%lf %d", &c, &e);
        if (e < 0 || e > MAXE)
            printf("指数 %d 超出范围，忽略\n", e);
        else
            P->coef[e] += c;
    }
    FixHigh(P);
}

/* 按下标逐项相加减 */
void AddSub(Poly *A, Poly *B, Poly *C, int sign) {
    int e;
    for (e = 0; e <= MAXE; e++)
        C->coef[e] = A->coef[e] + sign * B->coef[e];
    FixHigh(C);
}

/* 从最高次往下输出非零项 */
void Print(const char *name, Poly *P) {
    int e, first = 1;
    printf("%s = ", name);
    if (P->high < 0)
        printf("0");
    for (e = P->high; e >= 0; e--) {
        double c = P->coef[e];
        if (c == 0)
            continue;
        if (!first)
            printf(c < 0 ? " - " : " + ");
        else if (c < 0)
            printf("-");
        if (c < 0)
            c = -c;
        if (c != 1 || e == 0)
            printf("%g", c);
        if (e > 0)
            printf("x");
        if (e > 1)
            printf("^%d", e);
        first = 0;
    }
    printf("\n");
}

int main(void) {
    Poly A, B, C;
    Create(&A, "A");
    Create(&B, "B");
    Print("A", &A);
    Print("B", &B);
    AddSub(&A, &B, &C, 1);
    Print("A + B", &C);
    AddSub(&A, &B, &C, -1);
    Print("A - B", &C);
    return 0;
}
