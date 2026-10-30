#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXSIZE 100

int Prior(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;               /* '(' */
}

/* 变量直接输出；运算符入栈前，先弹出栈顶优先级不低于它的运算符；
   左括号入栈，右括号弹出到左括号为止 */
void ToRPN(const char *s, char *out) {
    char stack[MAXSIZE];
    int top = -1, k = 0;
    for (; *s != '\0'; s++) {
        char c = *s;
        if (isalpha((unsigned char)c)) {
            out[k++] = c;
        } else if (c == '(') {
            stack[++top] = c;
        } else if (c == ')') {
            while (stack[top] != '(')
                out[k++] = stack[top--];
            top--;
        } else if (strchr("+-*/", c)) {
            while (top >= 0 && Prior(stack[top]) >= Prior(c))
                out[k++] = stack[top--];
            stack[++top] = c;
        }                   /* 空格等其他字符忽略 */
    }
    while (top >= 0)
        out[k++] = stack[top--];
    out[k] = '\0';
}

int main(void) {
    char line[MAXSIZE], out[MAXSIZE];
    printf("请输入表达式（每行一个，空行结束）：");
    while (fgets(line, sizeof line, stdin) != NULL && line[0] != '\n') {
        line[strcspn(line, "\r\n")] = '\0';
        ToRPN(line, out);
        printf("%s  →  %s\n", line, out);
    }
    return 0;
}
