#include <stdio.h>
#define MAXSIZE 100

/* depth 是还没配对的左括号个数，相当于只记录栈的高度 */
int Match(const char *exp) {
    int depth = 0, i;
    for (i = 0; exp[i] != '\0'; i++) {
        if (exp[i] == '(') {
            depth++;
        } else if (exp[i] == ')') {
            if (depth == 0)         /* 右括号前面没有可配对的左括号 */
                return 0;
            depth--;
        }
    }
    return depth == 0;
}

int main(void) {
    char exp[MAXSIZE];
    printf("请输入算术表达式：");
    fgets(exp, sizeof exp, stdin);
    printf(Match(exp) ? "括号配对\n" : "括号不配对\n");
    return 0;
}
