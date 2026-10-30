#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *ch;               /* 按实际长度 malloc，没有 '\0' 结尾 */
    int length;
} HString;                  /* 堆分配串 */

void StrAssign(HString *S, const char *chars) {
    int len = (int)strlen(chars);
    free(S->ch);
    S->ch = len ? (char *)malloc(len) : NULL;
    memcpy(S->ch, chars, len);
    S->length = len;
}

int StrLength(HString *S) { return S->length; }

/* 联接：按两串长度之和申请新空间，没有长度上限 */
void Concat(HString *T, HString *S1, HString *S2) {
    free(T->ch);
    T->length = S1->length + S2->length;
    T->ch = (char *)malloc(T->length);
    memcpy(T->ch, S1->ch, S1->length);
    memcpy(T->ch + S1->length, S2->ch, S2->length);
}

int SubString(HString *Sub, HString *S, int pos, int len) {
    if (pos < 1 || pos > S->length || len < 0 || pos + len - 1 > S->length)
        return 0;
    free(Sub->ch);
    Sub->ch = len ? (char *)malloc(len) : NULL;
    memcpy(Sub->ch, S->ch + pos - 1, len);
    Sub->length = len;
    return 1;
}

int StrCompare(HString *S, HString *T) {
    int i;
    for (i = 0; i < S->length && i < T->length; i++)
        if (S->ch[i] != T->ch[i])
            return S->ch[i] - T->ch[i];
    return S->length - T->length;
}

/* next[j]：T[j] 失配时 j 应退回的位置，next[0] = -1 */
void GetNext(HString *T, int next[]) {
    int i = 0, j = -1;
    next[0] = -1;
    while (i < T->length - 1) {
        if (j == -1 || T->ch[i] == T->ch[j]) {
            i++;
            j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

/* KMP：主串指针 i 从不回退 */
int IndexKMP(HString *S, HString *T, int next[]) {
    int i = 0, j = 0;
    while (i < S->length && j < T->length) {
        if (j == -1 || S->ch[i] == T->ch[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    return j == T->length ? i - T->length + 1 : 0;
}

void Print(HString *S) { printf("%.*s", S->length, S->ch); }

int main(void) {
    char a[256], b[256];
    HString S = {NULL, 0}, T = {NULL, 0}, R = {NULL, 0};
    int pos, len, cmp, k, *next;
    printf("请输入主串 S 和模式串 T：");
    scanf("%255s %255s", a, b);
    printf("请输入取子串的起始位置和长度：");
    scanf("%d %d", &pos, &len);

    StrAssign(&S, a);
    StrAssign(&T, b);
    printf("赋值：S = "); Print(&S); printf("，T = "); Print(&T); printf("\n");
    printf("串长：S 长 %d，T 长 %d\n", StrLength(&S), StrLength(&T));
    Concat(&R, &S, &T);
    printf("联接：S + T = "); Print(&R); printf("\n");
    if (SubString(&R, &S, pos, len)) {
        printf("子串：S 从第 %d 个字符起长 %d 的子串是 ", pos, len);
        Print(&R);
        printf("\n");
    } else {
        printf("子串：位置或长度不合法\n");
    }
    cmp = StrCompare(&S, &T);
    printf("比较：S %s T\n", cmp > 0 ? ">" : cmp < 0 ? "<" : "=");

    next = (int *)malloc(T.length * sizeof(int));
    GetNext(&T, next);
    printf("匹配：T 的 next 数组为");
    for (k = 0; k < T.length; k++)
        printf(" %d", next[k]);
    k = IndexKMP(&S, &T, next);
    if (k)
        printf("，T 在 S 中的位置是 %d\n", k);
    else
        printf("，S 中没有 T\n");
    free(next);
    free(S.ch);
    free(T.ch);
    free(R.ch);
    return 0;
}
