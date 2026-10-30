#include <stdio.h>
#include <stdlib.h>

typedef struct PNode {
    double coef;
    int expn;
    struct PNode *next;
} PNode, *Polynomial;

/* 按指数降序插入，指数相同则合并 */
void InsertTerm(Polynomial P, double coef, int expn) {
    PNode *pre = P, *s;
    while (pre->next != NULL && pre->next->expn > expn)
        pre = pre->next;
    if (pre->next != NULL && pre->next->expn == expn) {
        pre->next->coef += coef;
        if (pre->next->coef == 0) {
            s = pre->next;
            pre->next = s->next;
            free(s);
        }
        return;
    }
    if (coef == 0)
        return;
    s = (PNode *)malloc(sizeof(PNode));
    s->coef = coef;
    s->expn = expn;
    s->next = pre->next;
    pre->next = s;
}

Polynomial Create(const char *name) {
    Polynomial P = (Polynomial)malloc(sizeof(PNode));
    int n, i, e;
    double c;
    P->next = NULL;
    printf("请输入多项式 %s 的项数：", name);
    scanf("%d", &n);
    printf("请输入 %d 组 系数 指数：", n);
    for (i = 0; i < n; i++) {
        scanf("%lf %d", &c, &e);
        InsertTerm(P, c, e);
    }
    return P;
}

/* C = A + sign·B：两个降序链表归并，减法就是把 B 的系数取反再加 */
Polynomial AddSub(Polynomial A, Polynomial B, int sign) {
    Polynomial C = (Polynomial)malloc(sizeof(PNode));
    PNode *pa = A->next, *pb = B->next, *tail = C, *s;
    double c;
    int e;
    while (pa != NULL || pb != NULL) {
        if (pb == NULL || (pa != NULL && pa->expn > pb->expn)) {
            c = pa->coef; e = pa->expn; pa = pa->next;
        } else if (pa == NULL || pb->expn > pa->expn) {
            c = sign * pb->coef; e = pb->expn; pb = pb->next;
        } else {
            c = pa->coef + sign * pb->coef; e = pa->expn;
            pa = pa->next; pb = pb->next;
        }
        if (c != 0) {
            s = (PNode *)malloc(sizeof(PNode));
            s->coef = c;
            s->expn = e;
            tail->next = s;
            tail = s;
        }
    }
    tail->next = NULL;
    return C;
}

void Print(const char *name, Polynomial P) {
    PNode *p = P->next;
    printf("%s = ", name);
    if (p == NULL)
        printf("0");
    for (; p != NULL; p = p->next) {
        double c = p->coef;
        if (p != P->next)
            printf(c < 0 ? " - " : " + ");
        else if (c < 0)
            printf("-");
        if (c < 0)
            c = -c;
        if (c != 1 || p->expn == 0)
            printf("%g", c);
        if (p->expn > 0)
            printf("x");
        if (p->expn > 1)
            printf("^%d", p->expn);
    }
    printf("\n");
}

int main(void) {
    Polynomial A = Create("A"), B = Create("B");
    Print("A", A);
    Print("B", B);
    Print("A + B", AddSub(A, B, 1));
    Print("A - B", AddSub(A, B, -1));
    return 0;
}
