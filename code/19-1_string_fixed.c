#include <stdio.h>
#define MAXLEN 255

typedef struct {
    char ch[MAXLEN + 1];
    int length;
} SString;                  /* 定长顺序串 */

/* 1）赋值 */
void StrAssign(SString *S, const char *chars) {
    int i;
    for (i = 0; chars[i] != '\0' && i < MAXLEN; i++)
        S->ch[i] = chars[i];
    S->ch[i] = '\0';
    S->length = i;
}

/* 2）求串长 */
int StrLength(SString *S) { return S->length; }

/* 3）联接：超过最大长度的部分截断 */
void Concat(SString *T, SString *S1, SString *S2) {
    int i, k = 0;
    for (i = 0; i < S1->length; i++)
        T->ch[k++] = S1->ch[i];
    for (i = 0; i < S2->length && k < MAXLEN; i++)
        T->ch[k++] = S2->ch[i];
    T->ch[k] = '\0';
    T->length = k;
}

/* 4）求子串：从第 pos 个字符起长度为 len，pos 从 1 开始 */
int SubString(SString *Sub, SString *S, int pos, int len) {
    int i;
    if (pos < 1 || pos > S->length || len < 0 || pos + len - 1 > S->length)
        return 0;
    for (i = 0; i < len; i++)
        Sub->ch[i] = S->ch[pos - 1 + i];
    Sub->ch[len] = '\0';
    Sub->length = len;
    return 1;
}

/* 5）比较：S > T 返回正数，相等返回 0，S < T 返回负数 */
int StrCompare(SString *S, SString *T) {
    int i;
    for (i = 0; i < S->length && i < T->length; i++)
        if (S->ch[i] != T->ch[i])
            return S->ch[i] - T->ch[i];
    return S->length - T->length;
}

/* 6）朴素模式匹配：返回 T 在 S 中第一次出现的位置，没有返回 0 */
int Index(SString *S, SString *T) {
    int i = 0, j = 0;
    while (i < S->length && j < T->length) {
        if (S->ch[i] == T->ch[j]) {
            i++;
            j++;
        } else {
            i = i - j + 1;          /* 主串回到本趟起点的下一个字符 */
            j = 0;
        }
    }
    return j == T->length ? i - T->length + 1 : 0;
}

int main(void) {
    char a[MAXLEN + 1], b[MAXLEN + 1];
    SString S, T, R;
    int pos, len, cmp, k;
    printf("请输入主串 S 和模式串 T：");
    scanf("%255s %255s", a, b);
    printf("请输入取子串的起始位置和长度：");
    scanf("%d %d", &pos, &len);

    StrAssign(&S, a);
    StrAssign(&T, b);
    printf("赋值：S = %s，T = %s\n", S.ch, T.ch);
    printf("串长：S 长 %d，T 长 %d\n", StrLength(&S), StrLength(&T));
    Concat(&R, &S, &T);
    printf("联接：S + T = %s\n", R.ch);
    if (SubString(&R, &S, pos, len))
        printf("子串：S 从第 %d 个字符起长 %d 的子串是 %s\n", pos, len, R.ch);
    else
        printf("子串：位置或长度不合法\n");
    cmp = StrCompare(&S, &T);
    printf("比较：S %s T\n", cmp > 0 ? ">" : cmp < 0 ? "<" : "=");
    k = Index(&S, &T);
    if (k)
        printf("匹配：T 在 S 中的位置是 %d\n", k);
    else
        printf("匹配：S 中没有 T\n");
    return 0;
}
