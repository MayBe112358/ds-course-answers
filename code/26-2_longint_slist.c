#include <stdio.h>
#include <stdlib.h>
#define BASE 10000

/* 单链表，低位组在前：相加相减时从表头开始，正好从低位算起 */
typedef struct LNode {
    int data;
    struct LNode *next;
} LNode;

typedef struct {
    LNode *low;             /* 最低位组，不带头结点 */
    int sign;
    int len;
} LongInt;

/* 在高位端（表尾）追加一组 */
void Append(LongInt *a, LNode ***tail, int v) {
    LNode *s = (LNode *)malloc(sizeof(LNode));
    s->data = v;
    s->next = NULL;
    **tail = s;
    *tail = &s->next;
    a->len++;
}

/* 去掉高位（表尾）多余的 0 组 */
void Normalize(LongInt *a) {
    LNode *p, *lastNonZero = a->low;
    int k = 1, keep = 1;
    for (p = a->low; p != NULL; p = p->next, k++)
        if (p->data != 0) {
            lastNonZero = p;
            keep = k;
        }
    p = lastNonZero->next;
    lastNonZero->next = NULL;
    while (p != NULL) {
        LNode *q = p->next;
        free(p);
        p = q;
    }
    a->len = keep;
    if (a->len == 1 && a->low->data == 0)
        a->sign = 1;
}

void Parse(LongInt *a, const char *str) {
    char digits[1200];
    int len = 0, i, k, v;
    LNode **tail = &a->low;
    a->low = NULL;
    a->sign = 1;
    a->len = 0;
    for (; *str; str++) {
        if (*str == '-' && len == 0)
            a->sign = -1;
        else if (*str >= '0' && *str <= '9' && len < 1200)
            digits[len++] = *str;
    }
    for (i = len; i > 0; i -= 4) {      /* 先取最低的四位，依次追加到表尾 */
        v = 0;
        for (k = (i >= 4 ? i - 4 : 0); k < i; k++)
            v = v * 10 + digits[k] - '0';
        Append(a, &tail, v);
    }
    if (a->len == 0)
        Append(a, &tail, 0);
    Normalize(a);
}

void Destroy(LongInt *a) {
    LNode *p = a->low, *q;
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
}

/* 组数相同时，高位在表尾，先递归比较后面的组，后面相等才比当前组 */
int CmpGroups(LNode *p, LNode *q) {
    int r;
    if (p == NULL)
        return 0;
    r = CmpGroups(p->next, q->next);
    return r != 0 ? r : p->data - q->data;
}

int CmpAbs(LongInt *a, LongInt *b) {
    if (a->len != b->len)
        return a->len - b->len;
    return CmpGroups(a->low, b->low);
}

void AddAbs(LongInt *a, LongInt *b, LongInt *c) {
    LNode *p = a->low, *q = b->low, **tail = &c->low;
    int carry = 0, sum;
    while (p != NULL || q != NULL || carry) {
        sum = carry;
        if (p != NULL) { sum += p->data; p = p->next; }
        if (q != NULL) { sum += q->data; q = q->next; }
        Append(c, &tail, sum % BASE);
        carry = sum / BASE;
    }
}

void SubAbs(LongInt *a, LongInt *b, LongInt *c) {
    LNode *p = a->low, *q = b->low, **tail = &c->low;
    int borrow = 0, d;
    for (; p != NULL; p = p->next) {
        d = p->data - borrow;
        if (q != NULL) {
            d -= q->data;
            q = q->next;
        }
        borrow = d < 0;
        Append(c, &tail, borrow ? d + BASE : d);
    }
}

void AddSub(LongInt *a, LongInt *b, LongInt *c, int sign) {
    int sb = b->sign * sign;
    c->low = NULL;
    c->len = 0;
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

/* 高位在表尾，用递归倒着输出：先输出后面的高位组，再输出当前组 */
void PrintGroups(LNode *p) {
    if (p->next == NULL) {
        printf("%d", p->data);
        return;
    }
    PrintGroups(p->next);
    printf(",%04d", p->data);
}

void Print(const char *label, LongInt *a) {
    printf("%s = %s", label, a->sign < 0 ? "-" : "");
    PrintGroups(a->low);
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
