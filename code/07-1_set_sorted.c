#include <stdio.h>
#define MAXSIZE 26

typedef struct {
    char elem[MAXSIZE];         /* 按字母顺序存放，不重复 */
    int length;
} Set;

/* 由字符串建立集合：只取小写字母，插入时保持有序并去掉重复 */
void CreateSet(Set *S, const char *str) {
    int i, j;
    S->length = 0;
    for (; *str != '\0'; str++) {
        char c = *str;
        if (c < 'a' || c > 'z')
            continue;
        for (i = 0; i < S->length && S->elem[i] < c; i++)
            ;
        if (i < S->length && S->elem[i] == c)
            continue;               /* 已经有了 */
        for (j = S->length; j > i; j--)
            S->elem[j] = S->elem[j - 1];
        S->elem[i] = c;
        S->length++;
    }
}

/* 并、交、差都是对两个有序表做一趟归并 */
void Union(Set *A, Set *B, Set *C) {
    int i = 0, j = 0;
    C->length = 0;
    while (i < A->length && j < B->length) {
        if (A->elem[i] < B->elem[j])
            C->elem[C->length++] = A->elem[i++];
        else if (A->elem[i] > B->elem[j])
            C->elem[C->length++] = B->elem[j++];
        else {
            C->elem[C->length++] = A->elem[i++];
            j++;
        }
    }
    while (i < A->length) C->elem[C->length++] = A->elem[i++];
    while (j < B->length) C->elem[C->length++] = B->elem[j++];
}

void Intersection(Set *A, Set *B, Set *C) {
    int i = 0, j = 0;
    C->length = 0;
    while (i < A->length && j < B->length) {
        if (A->elem[i] < B->elem[j])
            i++;
        else if (A->elem[i] > B->elem[j])
            j++;
        else {
            C->elem[C->length++] = A->elem[i++];
            j++;
        }
    }
}

void Difference(Set *A, Set *B, Set *C) {
    int i = 0, j = 0;
    C->length = 0;
    while (i < A->length) {
        if (j >= B->length || A->elem[i] < B->elem[j])
            C->elem[C->length++] = A->elem[i++];
        else if (A->elem[i] > B->elem[j])
            j++;
        else {
            i++;
            j++;
        }
    }
}

void Print(const char *name, Set *S) {
    int i;
    printf("%s = {", name);
    for (i = 0; i < S->length; i++)
        printf(i ? ", %c" : "%c", S->elem[i]);
    printf("}\n");
}

int main(void) {
    char s1[100], s2[100];
    Set A, B, C;
    printf("请输入集合 A 的元素：");
    fgets(s1, sizeof s1, stdin);
    printf("请输入集合 B 的元素：");
    fgets(s2, sizeof s2, stdin);
    CreateSet(&A, s1);
    CreateSet(&B, s2);
    Print("A", &A);
    Print("B", &B);
    Union(&A, &B, &C);
    Print("A ∪ B", &C);
    Intersection(&A, &B, &C);
    Print("A ∩ B", &C);
    Difference(&A, &B, &C);
    Print("A - B", &C);
    return 0;
}
