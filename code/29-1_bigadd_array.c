#include <stdio.h>
#define BASE 10000
#define MAXG 300            /* 最多 300 组，即 1200 位 */

typedef struct {
    int g[MAXG + 1];        /* g[0] 是最低的四位 */
    int len;
} BigInt;

/* 去掉逗号，从最低位起每四位存一个元素 */
void Parse(BigInt *a, const char *s) {
    char d[MAXG * 4];
    int n = 0, i, k, v;
    for (; *s; s++)
        if (*s >= '0' && *s <= '9' && n < MAXG * 4)
            d[n++] = *s;
    a->len = 0;
    for (i = n; i > 0; i -= 4) {
        v = 0;
        for (k = (i >= 4 ? i - 4 : 0); k < i; k++)
            v = v * 10 + d[k] - '0';
        a->g[a->len++] = v;
    }
    while (a->len > 1 && a->g[a->len - 1] == 0)
        a->len--;
    if (a->len == 0)
        a->g[a->len++] = 0;
}

/* 下标相同的组对齐相加，满一万向高位进一 */
void Add(BigInt *a, BigInt *b, BigInt *c) {
    int i, carry = 0, sum, n = a->len > b->len ? a->len : b->len;
    for (i = 0; i < n; i++) {
        sum = carry;
        if (i < a->len) sum += a->g[i];
        if (i < b->len) sum += b->g[i];
        c->g[i] = sum % BASE;
        carry = sum / BASE;
    }
    c->len = n;
    if (carry)
        c->g[c->len++] = carry;
}

void Print(const char *label, BigInt *a) {
    int i;
    printf("%s = %d", label, a->g[a->len - 1]);
    for (i = a->len - 2; i >= 0; i--)
        printf(",%04d", a->g[i]);
    printf("\n");
}

int main(void) {
    char s1[1300], s2[1300];
    BigInt a, b, c;
    printf("请输入第一个整数：");
    fgets(s1, sizeof s1, stdin);
    printf("请输入第二个整数：");
    fgets(s2, sizeof s2, stdin);
    Parse(&a, s1);
    Parse(&b, s2);
    Add(&a, &b, &c);
    Print("a", &a);
    Print("b", &b);
    Print("a + b", &c);
    return 0;
}
