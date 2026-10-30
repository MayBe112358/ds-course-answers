#include <stdio.h>
#define BASE 10000
#define MAXG 330

typedef struct {
    int g[MAXG + 2];        /* 非负，g[0] 是最低的四位 */
    int len;
} BigNum;

void Strip(BigNum *a) {
    while (a->len > 1 && a->g[a->len - 1] == 0)
        a->len--;
}

int IsZero(BigNum *a) { return a->len == 1 && a->g[0] == 0; }

int Parse(BigNum *a, const char *s) {
    char d[MAXG * 4];
    int n = 0, i, k, v, sign = 1;
    for (; *s; s++) {
        if (*s == '-' && n == 0)
            sign = -1;
        else if (*s >= '0' && *s <= '9' && n < MAXG * 4)
            d[n++] = *s;
    }
    a->len = 0;
    for (i = n; i > 0; i -= 4) {
        v = 0;
        for (k = (i >= 4 ? i - 4 : 0); k < i; k++)
            v = v * 10 + d[k] - '0';
        a->g[a->len++] = v;
    }
    if (a->len == 0)
        a->g[a->len++] = 0;
    Strip(a);
    return sign;
}

int Cmp(BigNum *a, BigNum *b) {
    int i;
    if (a->len != b->len)
        return a->len - b->len;
    for (i = a->len - 1; i >= 0; i--)
        if (a->g[i] != b->g[i])
            return a->g[i] - b->g[i];
    return 0;
}

void Sub(BigNum *a, BigNum *b) {
    int i, borrow = 0;
    for (i = 0; i < a->len; i++) {
        a->g[i] -= borrow + (i < b->len ? b->g[i] : 0);
        borrow = a->g[i] < 0;
        if (borrow)
            a->g[i] += BASE;
    }
    Strip(a);
}

/* t = b × k，k 小于 BASE */
void MulSmall(BigNum *b, int k, BigNum *t) {
    int i, carry = 0, x;
    for (i = 0; i < b->len; i++) {
        x = b->g[i] * k + carry;
        t->g[i] = x % BASE;
        carry = x / BASE;
    }
    t->len = b->len;
    if (carry)
        t->g[t->len++] = carry;
    Strip(t);
}

/* r = r × BASE + v */
void ShiftAdd(BigNum *r, int v) {
    int i;
    if (IsZero(r)) {
        r->g[0] = v;
        return;
    }
    for (i = r->len; i > 0; i--)
        r->g[i] = r->g[i - 1];
    r->g[0] = v;
    r->len++;
}

/* 万进制竖式除法：每次落下一组（四位），这一组的商在 0 ~ 9999 之间，用二分法找出最大的 q 使 b·q ≤ r */
void Divide(BigNum *a, BigNum *b, BigNum *q, BigNum *r) {
    BigNum t;
    int i, lo, hi, mid;
    r->len = 1;
    r->g[0] = 0;
    q->len = a->len;
    for (i = a->len - 1; i >= 0; i--) {
        ShiftAdd(r, a->g[i]);
        lo = 0;
        hi = BASE - 1;
        while (lo < hi) {
            mid = (lo + hi + 1) / 2;
            MulSmall(b, mid, &t);
            if (Cmp(&t, r) <= 0)
                lo = mid;
            else
                hi = mid - 1;
        }
        q->g[i] = lo;
        MulSmall(b, lo, &t);
        Sub(r, &t);
    }
    Strip(q);
}

void Print(const char *label, BigNum *a, int sign) {
    int i;
    printf("%s = %s%d", label, sign < 0 && !IsZero(a) ? "-" : "", a->g[a->len - 1]);
    for (i = a->len - 2; i >= 0; i--)
        printf(",%04d", a->g[i]);
    printf("\n");
}

int main(void) {
    char s1[1400], s2[1400];
    static BigNum a, b, q, r;
    int sa, sb;
    printf("请输入被除数：");
    fgets(s1, sizeof s1, stdin);
    printf("请输入除数：");
    fgets(s2, sizeof s2, stdin);
    sa = Parse(&a, s1);
    sb = Parse(&b, s2);
    if (IsZero(&b)) {
        printf("除数不能为 0\n");
        return 1;
    }
    Divide(&a, &b, &q, &r);
    Print("a", &a, sa);
    Print("b", &b, sb);
    Print("商", &q, sa * sb);
    Print("余数", &r, sa);
    return 0;
}
