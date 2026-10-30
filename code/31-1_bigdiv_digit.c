#include <stdio.h>
#define MAXLEN 1300

typedef struct {
    int d[MAXLEN];          /* 非负，d[0] 是个位 */
    int len;
} BigNum;

void Strip(BigNum *a) {
    while (a->len > 1 && a->d[a->len - 1] == 0)
        a->len--;
}

int IsZero(BigNum *a) { return a->len == 1 && a->d[0] == 0; }

/* 读入时符号单独返回，数值按位存 */
int Parse(BigNum *a, const char *s) {
    char buf[MAXLEN];
    int n = 0, i, sign = 1;
    for (; *s; s++) {
        if (*s == '-' && n == 0)
            sign = -1;
        else if (*s >= '0' && *s <= '9' && n < MAXLEN - 1)
            buf[n++] = *s;
    }
    for (i = 0; i < n; i++)
        a->d[i] = buf[n - 1 - i] - '0';
    a->len = n ? n : 1;
    if (n == 0)
        a->d[0] = 0;
    Strip(a);
    return sign;
}

int Cmp(BigNum *a, BigNum *b) {
    int i;
    if (a->len != b->len)
        return a->len - b->len;
    for (i = a->len - 1; i >= 0; i--)
        if (a->d[i] != b->d[i])
            return a->d[i] - b->d[i];
    return 0;
}

/* a -= b，要求 a >= b */
void Sub(BigNum *a, BigNum *b) {
    int i, borrow = 0;
    for (i = 0; i < a->len; i++) {
        a->d[i] -= borrow + (i < b->len ? b->d[i] : 0);
        borrow = a->d[i] < 0;
        if (borrow)
            a->d[i] += 10;
    }
    Strip(a);
}

/* r = r × 10 + digit */
void ShiftAdd(BigNum *r, int digit) {
    int i;
    if (IsZero(r)) {
        r->d[0] = digit;
        return;
    }
    for (i = r->len; i > 0; i--)
        r->d[i] = r->d[i - 1];
    r->d[0] = digit;
    r->len++;
}

/* 竖式除法：从最高位起每次落下一位，能减几次除数，这一位商就是几 */
void Divide(BigNum *a, BigNum *b, BigNum *q, BigNum *r) {
    int i, cnt;
    r->len = 1;
    r->d[0] = 0;
    q->len = a->len;
    for (i = a->len - 1; i >= 0; i--) {
        ShiftAdd(r, a->d[i]);
        for (cnt = 0; Cmp(r, b) >= 0; cnt++)
            Sub(r, b);
        q->d[i] = cnt;
    }
    Strip(q);
}

void Print(const char *label, BigNum *a, int sign) {
    int i;
    printf("%s = %s", label, sign < 0 && !IsZero(a) ? "-" : "");
    for (i = a->len - 1; i >= 0; i--) {
        printf("%d", a->d[i]);
        if (i > 0 && i % 4 == 0)
            printf(",");
    }
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
    Print("商", &q, sa * sb);       /* 和 C 语言一样向 0 取整，余数与被除数同号 */
    Print("余数", &r, sa);
    return 0;
}
