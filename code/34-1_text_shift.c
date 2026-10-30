#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXLEN 1000

typedef struct {
    char ch[MAXLEN + 1];
    int length;
} SqString;

void Statistics(SqString *S) {
    int i, letters = 0, digits = 0, spaces = 0, others = 0;
    for (i = 0; i < S->length; i++) {
        unsigned char c = S->ch[i];
        if (isalpha(c)) letters++;
        else if (isdigit(c)) digits++;
        else if (c == ' ') spaces++;
        else others++;
    }
    printf("英文字母 %d 个，数字 %d 个，空格 %d 个，其他字符 %d 个\n", letters, digits, spaces, others);
}

/* 从位置 i 开始是否与 t 相同 */
int MatchAt(SqString *S, int i, const char *t, int m) {
    int k;
    for (k = 0; k < m; k++)
        if (S->ch[i + k] != t[k])
            return 0;
    return 1;
}

/* 统计出现次数：匹配成功后跳过整个子串，不重叠计数 */
int Count(SqString *S, const char *t) {
    int i = 0, m = (int)strlen(t), cnt = 0;
    while (i + m <= S->length) {
        if (MatchAt(S, i, t, m)) {
            cnt++;
            i += m;
        } else {
            i++;
        }
    }
    return cnt;
}

/* 删除：每找到一处，就把它后面的字符整体前移 m 位 */
void Delete(SqString *S, const char *t) {
    int i = 0, k, m = (int)strlen(t);
    while (i + m <= S->length) {
        if (MatchAt(S, i, t, m)) {
            for (k = i; k + m < S->length; k++)
                S->ch[k] = S->ch[k + m];
            S->length -= m;
        } else {
            i++;
        }
    }
    S->ch[S->length] = '\0';
}

int main(void) {
    SqString S;
    char t[MAXLEN + 1];
    printf("请输入一行文本：");
    fgets(S.ch, sizeof S.ch, stdin);
    S.ch[strcspn(S.ch, "\r\n")] = '\0';
    S.length = (int)strlen(S.ch);
    printf("请输入要统计和删除的字符串：");
    fgets(t, sizeof t, stdin);
    t[strcspn(t, "\r\n")] = '\0';
    if (t[0] == '\0') {
        printf("要查找的字符串不能为空\n");
        return 1;
    }
    Statistics(&S);
    printf("\"%s\" 出现了 %d 次\n", t, Count(&S, t));
    Delete(&S, t);
    printf("删除后：%s\n", S.ch);
    return 0;
}
