#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXSIZE 100

typedef struct {
    int isNum;
    double value;
    char op;
} Token;

int Prior(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

/* 第一步：中缀转后缀，同时检查语法；成功返回后缀式的长度，失败返回 -1 */
int ToPostfix(const char *s, Token post[]) {
    char stack[MAXSIZE];
    int top = -1, n = 0, i = 0, expect = 1;
    char *end;
    while (s[i] != '\0') {
        char c = s[i];
        if (c == ' ') {
            i++;
            continue;
        }
        if (isdigit((unsigned char)c)) {
            if (!expect) { printf("  错误：第 %d 个字符前缺少运算符\n", i + 1); return -1; }
            post[n].isNum = 1;
            post[n++].value = strtod(s + i, &end);
            i = (int)(end - s);
            expect = 0;
            continue;
        }
        if (c == '(') {
            if (!expect) { printf("  错误：第 %d 个字符前缺少运算符\n", i + 1); return -1; }
            stack[++top] = c;
        } else if (c == ')') {
            if (expect) { printf("  错误：第 %d 个字符前缺少操作数\n", i + 1); return -1; }
            while (top >= 0 && stack[top] != '(') {
                post[n].isNum = 0;
                post[n++].op = stack[top--];
            }
            if (top < 0) { printf("  错误：第 %d 个字符是多余的右括号\n", i + 1); return -1; }
            top--;
        } else if (strchr("+-*/", c)) {
            if (expect) { printf("  错误：第 %d 个字符前缺少操作数\n", i + 1); return -1; }
            while (top >= 0 && Prior(stack[top]) >= Prior(c)) {
                post[n].isNum = 0;
                post[n++].op = stack[top--];
            }
            stack[++top] = c;
            expect = 1;
        } else {
            printf("  错误：第 %d 个字符不合法\n", i + 1);
            return -1;
        }
        i++;
    }
    if (expect) { printf("  错误：表达式不完整\n"); return -1; }
    while (top >= 0) {
        if (stack[top] == '(') { printf("  错误：缺少右括号\n"); return -1; }
        post[n].isNum = 0;
        post[n++].op = stack[top--];
    }
    return n;
}

/* 第二步：后缀式求值，数字入栈，遇到运算符弹出两个数计算 */
int EvalPostfix(Token post[], int n, double *result) {
    double stack[MAXSIZE], a, b, r;
    int top = -1, k;
    for (k = 0; k < n; k++) {
        if (post[k].isNum) {
            stack[++top] = post[k].value;
            continue;
        }
        b = stack[top--];
        a = stack[top--];
        if (post[k].op == '/' && b == 0) {
            printf("  错误：除数为 0\n");
            return 0;
        }
        switch (post[k].op) {
        case '+': r = a + b; break;
        case '-': r = a - b; break;
        case '*': r = a * b; break;
        default:  r = a / b;
        }
        printf("  %g %c %g = %g\n", a, post[k].op, b, r);
        stack[++top] = r;
    }
    *result = stack[top];
    return 1;
}

int main(void) {
    char line[MAXSIZE];
    Token post[MAXSIZE];
    int n, k;
    double r;
    printf("请输入算术表达式（每行一个，空行结束）：");
    while (fgets(line, sizeof line, stdin) != NULL && line[0] != '\n') {
        line[strcspn(line, "\r\n")] = '\0';
        printf("表达式：%s\n", line);
        n = ToPostfix(line, post);
        if (n < 0)
            continue;
        printf("  后缀式：");
        for (k = 0; k < n; k++) {
            if (post[k].isNum)
                printf("%g ", post[k].value);
            else
                printf("%c ", post[k].op);
        }
        printf("\n");
        if (EvalPostfix(post, n, &r))
            printf("  结果 = %g\n", r);
    }
    return 0;
}
