#include <stdio.h>
#define MAXLEN 1300

typedef struct {
    char data[MAXLEN];      /* 每个元素是一位数字字符 */
    int top;
} Stack;

void Push(Stack *S, char c) { S->data[++S->top] = c; }
char Pop(Stack *S) { return S->data[S->top--]; }
int Empty(Stack *S) { return S->top == -1; }

/* 从高位到低位依次压栈，栈顶就是个位 */
void ReadNumber(Stack *S, const char *s) {
    S->top = -1;
    for (; *s; s++)
        if (*s >= '0' && *s <= '9' && S->top < MAXLEN - 2)
            Push(S, *s);
}

int main(void) {
    char s1[MAXLEN], s2[MAXLEN];
    Stack A, B, C;
    int carry = 0, sum, n;
    printf("请输入第一个整数：");
    fgets(s1, sizeof s1, stdin);
    printf("请输入第二个整数：");
    fgets(s2, sizeof s2, stdin);
    ReadNumber(&A, s1);
    ReadNumber(&B, s2);

    C.top = -1;
    while (!Empty(&A) || !Empty(&B) || carry) {    /* 两个栈顶都是当前的最低位 */
        sum = carry;
        if (!Empty(&A)) sum += Pop(&A) - '0';
        if (!Empty(&B)) sum += Pop(&B) - '0';
        Push(&C, sum % 10 + '0');                   /* 和从低位往高位压栈，栈顶是最高位 */
        carry = sum / 10;
    }
    while (C.top > 0 && C.data[C.top] == '0')      /* 去掉高位多余的 0 */
        C.top--;
    if (Empty(&C))
        Push(&C, '0');

    printf("a + b = ");
    n = C.top + 1;                                  /* 还剩几位没输出，决定在哪里加逗号 */
    while (!Empty(&C)) {
        putchar(Pop(&C));
        if (--n > 0 && n % 4 == 0)
            putchar(',');
    }
    printf("\n");
    return 0;
}
