#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    char data[MAXSIZE];
    int top;                /* 栈顶下标，-1 表示空栈 */
} SqStack;

void InitStack(SqStack *S) { S->top = -1; }
int StackEmpty(SqStack *S) { return S->top == -1; }

int Push(SqStack *S, char e) {
    if (S->top == MAXSIZE - 1) return 0;
    S->data[++S->top] = e;
    return 1;
}

int Pop(SqStack *S, char *e) {
    if (StackEmpty(S)) return 0;
    *e = S->data[S->top--];
    return 1;
}

/* 返回 1 表示配对，0 表示不配对 */
int Match(const char *exp) {
    SqStack S;
    char e;
    int i;
    InitStack(&S);
    for (i = 0; exp[i] != '\0'; i++) {
        if (exp[i] == '(') {
            Push(&S, '(');
        } else if (exp[i] == ')') {
            if (!Pop(&S, &e))       /* 栈空却读到右括号 */
                return 0;
        }
    }
    return StackEmpty(&S);          /* 表达式结束时栈空才算配对 */
}

int main(void) {
    char exp[MAXSIZE];
    printf("请输入算术表达式：");
    fgets(exp, sizeof exp, stdin);
    printf(Match(exp) ? "括号配对\n" : "括号不配对\n");
    return 0;
}
