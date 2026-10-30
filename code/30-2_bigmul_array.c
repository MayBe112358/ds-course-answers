#include <stdio.h>
#define MAXLEN 1200

typedef struct {
    int d[2 * MAXLEN];      /* 每个元素一位，d[0] 是个位 */
    int len;
    int sign;
} BigInt;

void Parse(BigInt *a, const char *s) {
    char buf[MAXLEN];
    int n = 0, i;
    a->sign = 1;
    for (; *s; s++) {
        if (*s == '-' && n == 0)
            a->sign = -1;
        else if (*s >= '0' && *s <= '9' && n < MAXLEN)
            buf[n++] = *s;
    }
    for (i = 0; i < n; i++)
        a->d[i] = buf[n - 1 - i] - '0';
    a->len = n;
    while (a->len > 1 && a->d[a->len - 1] == 0)
        a->len--;
    if (a->len == 0)
        a->d[a->len++] = 0;
    if (a->len == 1 && a->d[0] == 0)
        a->sign = 1;
}

/* 先把 a[i]·b[j] 全部累加到 c[i+j]，不进位；最后从低位到高位统一处理一次进位 */
void Multiply(BigInt *a, BigInt *b, BigInt *c) {
    int i, j, carry = 0;
    c->len = a->len + b->len;
    for (i = 0; i < c->len; i++)
        c->d[i] = 0;
    for (i = 0; i < a->len; i++)
        for (j = 0; j < b->len; j++)
            c->d[i + j] += a->d[i] * b->d[j];
    for (i = 0; i < c->len; i++) {
        c->d[i] += carry;
        carry = c->d[i] / 10;
        c->d[i] %= 10;
    }
    while (c->len > 1 && c->d[c->len - 1] == 0)
        c->len--;
    c->sign = (c->len == 1 && c->d[0] == 0) ? 1 : a->sign * b->sign;
}

/* 从最高位输出，下标是 4 的倍数的位后面加逗号 */
void Print(const char *label, BigInt *a) {
    int i;
    printf("%s = %s", label, a->sign < 0 ? "-" : "");
    for (i = a->len - 1; i >= 0; i--) {
        printf("%d", a->d[i]);
        if (i > 0 && i % 4 == 0)
            printf(",");
    }
    printf("\n");
}

int main(void) {
    char s1[1300], s2[1300];
    static BigInt a, b, c;
    printf("请输入第一个整数：");
    fgets(s1, sizeof s1, stdin);
    printf("请输入第二个整数：");
    fgets(s2, sizeof s2, stdin);
    Parse(&a, s1);
    Parse(&b, s2);
    Multiply(&a, &b, &c);
    Print("a", &a);
    Print("b", &b);
    Print("a × b", &c);
    return 0;
}
