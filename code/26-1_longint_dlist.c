#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 10000

/* 双向循环链表，每个结点存四位（0 ~ 9999）。head->next 是最高位组，head->prior 是最低位组 */
typedef struct DNode {
    int data;
    struct DNode *prior, *next;
} DNode;

typedef struct {
    DNode *head;
    int sign;               /* 1 或 -1 */
    int len;                /* 组数 */
} LongInt;

void Init(LongInt *a) {
    a->head = (DNode *)malloc(sizeof(DNode));
    a->head->prior = a->head->next = a->head;
    a->sign = 1;
    a->len = 0;
}

/* 在最高位之前插入一组 */
void InsertHigh(LongInt *a, int v) {
    DNode *s = (DNode *)malloc(sizeof(DNode));
    s->data = v;
    s->prior = a->head;
    s->next = a->head->next;
    a->head->next->prior = s;
    a->head->next = s;
    a->len++;
}

/* 去掉高位多余的 0 组；值为 0 时符号为正 */
void Normalize(LongInt *a) {
    DNode *p;
    while (a->len > 1 && a->head->next->data == 0) {
        p = a->head->next;
        a->head->next = p->next;
        p->next->prior = a->head;
        free(p);
        a->len--;
    }
    if (a->len == 0)
        InsertHigh(a, 0);
    if (a->len == 1 && a->head->next->data == 0)
        a->sign = 1;
}

/* 读入 "-2345,6789,3211"：逗号只起分隔作用，从最低位起每四位组成一个结点 */
void Parse(LongInt *a, const char *str) {
    char digits[1200];
    int len = 0, i, k, v;
    Init(a);
    for (; *str; str++) {
        if (*str == '-' && len == 0)
            a->sign = -1;
        else if (*str >= '0' && *str <= '9' && len < 1200)
            digits[len++] = *str;
    }
    for (i = len; i > 0; i -= 4) {
        v = 0;
        for (k = (i >= 4 ? i - 4 : 0); k < i; k++)
            v = v * 10 + digits[k] - '0';
        InsertHigh(a, v);
    }
    Normalize(a);
}

void Destroy(LongInt *a) {
    DNode *p = a->head->next, *q;
    while (p != a->head) {
        q = p->next;
        free(p);
        p = q;
    }
    free(a->head);
}

/* 比较绝对值：组数多的大；组数相同从最高位组往低位比 */
int CmpAbs(LongInt *a, LongInt *b) {
    DNode *p, *q;
    if (a->len != b->len)
        return a->len - b->len;
    for (p = a->head->next, q = b->head->next; p != a->head; p = p->next, q = q->next)
        if (p->data != q->data)
            return p->data - q->data;
    return 0;
}

/* |a| + |b|：从最低位组（prior 方向）开始逐组相加 */
void AddAbs(LongInt *a, LongInt *b, LongInt *c) {
    DNode *p = a->head->prior, *q = b->head->prior;
    int carry = 0, sum;
    while (p != a->head || q != b->head || carry) {
        sum = carry;
        if (p != a->head) { sum += p->data; p = p->prior; }
        if (q != b->head) { sum += q->data; q = q->prior; }
        InsertHigh(c, sum % BASE);
        carry = sum / BASE;
    }
}

/* |a| - |b|，要求 |a| >= |b| */
void SubAbs(LongInt *a, LongInt *b, LongInt *c) {
    DNode *p = a->head->prior, *q = b->head->prior;
    int borrow = 0, d;
    while (p != a->head) {
        d = p->data - borrow;
        p = p->prior;
        if (q != b->head) {
            d -= q->data;
            q = q->prior;
        }
        borrow = d < 0;
        InsertHigh(c, borrow ? d + BASE : d);
    }
}

/* c = a + sign·b：符号相同时绝对值相加，不同时大减小，符号跟绝对值大的走 */
void AddSub(LongInt *a, LongInt *b, LongInt *c, int sign) {
    int sb = b->sign * sign;
    Init(c);
    if (a->sign == sb) {
        AddAbs(a, b, c);
        c->sign = a->sign;
    } else if (CmpAbs(a, b) >= 0) {
        SubAbs(a, b, c);
        c->sign = a->sign;
    } else {
        SubAbs(b, a, c);
        c->sign = sb;
    }
    Normalize(c);
}

/* 按习惯每四位一组输出，最高位组不补 0 */
void Print(const char *label, LongInt *a) {
    DNode *p = a->head->next;
    printf("%s = %s%d", label, a->sign < 0 ? "-" : "", p->data);
    for (p = p->next; p != a->head; p = p->next)
        printf(",%04d", p->data);
    printf("\n");
}

int main(void) {
    char s1[1300], s2[1300];
    LongInt a, b, c;
    printf("请输入第一个长整数：");
    fgets(s1, sizeof s1, stdin);
    printf("请输入第二个长整数：");
    fgets(s2, sizeof s2, stdin);
    Parse(&a, s1);
    Parse(&b, s2);
    Print("a", &a);
    Print("b", &b);
    AddSub(&a, &b, &c, 1);
    Print("a + b", &c);
    Destroy(&c);
    AddSub(&a, &b, &c, -1);
    Print("a - b", &c);
    Destroy(&c);
    Destroy(&a);
    Destroy(&b);
    return 0;
}
