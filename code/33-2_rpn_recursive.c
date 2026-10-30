#include <stdio.h>
#include <string.h>
#define MAXSIZE 100

const char *p;              /* 当前读到的位置 */
char out[MAXSIZE];
int k;

void Expr(void);

/* 因子 = 变量 | ( 表达式 ) */
void Factor(void) {
    if (*p == '(') {
        p++;
        Expr();
        p++;                /* 跳过 ')' */
    } else {
        out[k++] = *p++;
    }
}

/* 项 = 因子 { (*|/) 因子 }：两个因子都输出后再输出运算符 */
void Term(void) {
    char op;
    Factor();
    while (*p == '*' || *p == '/') {
        op = *p++;
        Factor();
        out[k++] = op;
    }
}

/* 表达式 = 项 { (+|-) 项 } */
void Expr(void) {
    char op;
    Term();
    while (*p == '+' || *p == '-') {
        op = *p++;
        Term();
        out[k++] = op;
    }
}

int main(void) {
    char line[MAXSIZE], expr[MAXSIZE];
    int i, n;
    printf("请输入表达式（每行一个，空行结束）：");
    while (fgets(line, sizeof line, stdin) != NULL && line[0] != '\n') {
        line[strcspn(line, "\r\n")] = '\0';
        for (i = n = 0; line[i] != '\0'; i++)   /* 去掉空格 */
            if (line[i] != ' ')
                expr[n++] = line[i];
        expr[n] = '\0';
        p = expr;
        k = 0;
        Expr();
        out[k] = '\0';
        printf("%s  →  %s\n", line, out);
    }
    return 0;
}
