#include <stdio.h>
#include <stdlib.h>
#define BASE 10000

/* 单链表，低位组在前，每个结点四位 */
typedef struct LNode {
    int data;
    struct LNode *next;
} LNode;

typedef struct {
    LNode *low;
    int sign;
    int len;
} LongInt;

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
    LNode *p, *last = a->low, *q;
    int k = 1, keep = 1;
    for (p = a->low; p != NULL; p = p->next, k++)
        if (p->data != 0) {
            last = p;
            keep = k;
        }
    p = last->next;
    last->next = NULL;
    for (; p != NULL; p = q) {
        q = p->next;
        free(p);
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
    for (i = len; i > 0; i -= 4) {
        v = 0;
        for (k = (i >= 4 ? i - 4 : 0); k < i; k++)
            v = v * 10 + digits[k] - '0';
        Append(a, &tail, v);
    }
    if (a->len == 0)
        Append(a, &tail, 0);
    Normalize(a);
}

/* 竖式乘法：结果先建成 la+lb 个 0 结点；a 的第 i 组乘 b 的每一组，累加到结果的第 i+j 组。
   start 指向结果的第 i 组，r 从 start 出发随 b 一起后移，单链表也能对齐 */
void Multiply(LongInt *a, LongInt *b, LongInt *c) {
    LNode *p, *q, *start, *r, **tail = &c->low;
    int k, carry, t;
    c->low = NULL;
    c->len = 0;
    for (k = 0; k < a->len + b->len; k++)
        Append(c, &tail, 0);
    for (p = a->low, start = c->low; p != NULL; p = p->next, start = start->next) {
        carry = 0;
        for (q = b->low, r = start; q != NULL; q = q->next, r = r->next) {
            t = r->data + p->data * q->data + carry;
            r->data = t % BASE;
            carry = t / BASE;
        }
        r->data += carry;           /* r 是本轮最高位的下一组，此前还是 0 */
    }
    c->sign = a->sign * b->sign;
    Normalize(c);
}

void Destroy(LongInt *a) {
    LNode *p = a->low, *q;
    for (; p != NULL; p = q) {
        q = p->next;
        free(p);
    }
}

/* 高位在表尾，递归倒着输出 */
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
    Destroy(&a);
    Destroy(&b);
    Destroy(&c);
    return 0;
}
