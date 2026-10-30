#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXSIZE 100

double opnd[MAXSIZE];       /* 操作数栈 */
char optr[MAXSIZE];         /* 运算符栈 */
int topN, topR;

int Prior(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;               /* '(' 优先级最低，只有遇到 ')' 才弹出 */
}

/* 弹出一个运算符和两个操作数，计算后压回操作数栈，并显示这一步 */
int Calc(void) {
    char op = optr[topR--];
    double b = opnd[topN--], a = opnd[topN--], r;
    if (op == '/' && b == 0) {
        printf("  错误：除数为 0\n");
        return 0;
    }
    r = op == '+' ? a + b : op == '-' ? a - b : op == '*' ? a * b : a / b;
    printf("  %g %c %g = %g\n", a, op, b, r);
    opnd[++topN] = r;
    return 1;
}

/* 算符优先法求值。expect 表示当前位置应该出现操作数（1）还是运算符（0），用来发现语法错误 */
int Evaluate(const char *s, double *result) {
    int i = 0, expect = 1;
    char *end;
    topN = topR = -1;
    while (s[i] != '\0' && s[i] != '\n' && s[i] != '\r') {
        char c = s[i];
        if (c == ' ') {
            i++;
        } else if (isdigit((unsigned char)c)) {
            if (!expect) {
                printf("  错误：第 %d 个字符前缺少运算符\n", i + 1);
                return 0;
            }
            opnd[++topN] = strtod(s + i, &end);
            i = (int)(end - s);
            expect = 0;
        } else if (c == '(') {
            if (!expect) {
                printf("  错误：第 %d 个字符前缺少运算符\n", i + 1);
                return 0;
            }
            optr[++topR] = c;
            i++;
        } else if (c == ')') {
            if (expect) {
                printf("  错误：第 %d 个字符前缺少操作数\n", i + 1);
                return 0;
            }
            while (topR >= 0 && optr[topR] != '(')
                if (!Calc())
                    return 0;
            if (topR < 0) {
                printf("  错误：第 %d 个字符是多余的右括号\n", i + 1);
                return 0;
            }
            topR--;         /* 弹出与之配对的左括号 */
            i++;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (expect) {
                printf("  错误：第 %d 个字符前缺少操作数\n", i + 1);
                return 0;
            }
            while (topR >= 0 && Prior(optr[topR]) >= Prior(c))
                if (!Calc())
                    return 0;
            optr[++topR] = c;
            expect = 1;
            i++;
        } else {
            printf("  错误：第 %d 个字符不合法\n", i + 1);
            return 0;
        }
    }
    if (expect) {
        printf("  错误：表达式不完整\n");
        return 0;
    }
    while (topR >= 0) {
        if (optr[topR] == '(') {
            printf("  错误：缺少右括号\n");
            return 0;
        }
        if (!Calc())
            return 0;
    }
    *result = opnd[topN];
    return 1;
}

int main(void) {
    char line[MAXSIZE];
    double r;
    printf("请输入算术表达式（每行一个，空行结束）：");
    while (fgets(line, sizeof line, stdin) != NULL && line[0] != '\n') {
        line[strcspn(line, "\r\n")] = '\0';
        printf("表达式：%s\n", line);
        if (Evaluate(line, &r))
            printf("  结果 = %g\n", r);
    }
    return 0;
}
