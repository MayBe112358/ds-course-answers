#include <stdio.h>
#define MAXTERMS 100

typedef struct {
    double coef;
    int expn;
} Term;

typedef struct {
    Term data[MAXTERMS];    /* 按指数降序存放 */
    int length;
} Poly;

/* 1）读入各项后按指数降序做直接插入排序，再合并同类项、去掉系数为 0 的项 */
void Create(Poly *P, const char *name) {
    int n, i, j, k;
    Term t;
    printf("请输入多项式 %s 的项数：", name);
    scanf("%d", &n);
    printf("请输入 %d 组 系数 指数：", n);
    for (i = 0; i < n; i++) {
        scanf("%lf %d", &t.coef, &t.expn);
        for (j = i - 1; j >= 0 && P->data[j].expn < t.expn; j--)
            P->data[j + 1] = P->data[j];
        P->data[j + 1] = t;
    }
    k = 0;
    for (i = 0; i < n; i++) {
        if (k > 0 && P->data[k - 1].expn == P->data[i].expn)
            P->data[k - 1].coef += P->data[i].coef;
        else
            P->data[k++] = P->data[i];
        if (P->data[k - 1].coef == 0)
            k--;
    }
    P->length = k;
}

/* 3）相加：i、j 分别指向 A、B 的当前项，指数大的先放进 C */
void Add(Poly *A, Poly *B, Poly *C) {
    int i = 0, j = 0;
    Term t;
    C->length = 0;
    while (i < A->length || j < B->length) {
        if (j >= B->length || (i < A->length && A->data[i].expn > B->data[j].expn)) {
            t = A->data[i++];
        } else if (i >= A->length || B->data[j].expn > A->data[i].expn) {
            t = B->data[j++];
        } else {
            t = A->data[i++];
            t.coef += B->data[j++].coef;
        }
        if (t.coef != 0)
            C->data[C->length++] = t;
    }
}

void Print(const char *name, Poly *P) {
    int i;
    printf("%s = ", name);
    if (P->length == 0)
        printf("0");
    for (i = 0; i < P->length; i++) {
        double c = P->data[i].coef;
        int e = P->data[i].expn;
        if (i > 0)
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
    }
    printf("\n");
}

int main(void) {
    Poly A, B, C;
    Create(&A, "A");
    Create(&B, "B");
    Add(&A, &B, &C);
    Print("A", &A);
    Print("B", &B);
    Print("A + B", &C);
    return 0;
}
