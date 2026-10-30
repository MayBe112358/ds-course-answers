#include <stdio.h>

typedef unsigned int Set;       /* 第 k 位为 1 表示字母 'a'+k 在集合中 */

Set CreateSet(const char *str) {
    Set s = 0;
    for (; *str != '\0'; str++)
        if (*str >= 'a' && *str <= 'z')
            s |= 1u << (*str - 'a');    /* 重复的字母自然只占一位 */
    return s;
}

Set Union(Set a, Set b) { return a | b; }
Set Intersection(Set a, Set b) { return a & b; }
Set Difference(Set a, Set b) { return a & ~b; }

void Print(const char *name, Set s) {
    int k, first = 1;
    printf("%s = {", name);
    for (k = 0; k < 26; k++) {
        if (s >> k & 1u) {
            printf(first ? "%c" : ", %c", 'a' + k);
            first = 0;
        }
    }
    printf("}\n");
}

int main(void) {
    char s1[100], s2[100];
    Set A, B;
    printf("请输入集合 A 的元素：");
    fgets(s1, sizeof s1, stdin);
    printf("请输入集合 B 的元素：");
    fgets(s2, sizeof s2, stdin);
    A = CreateSet(s1);
    B = CreateSet(s2);
    Print("A", A);
    Print("B", B);
    Print("A ∪ B", Union(A, B));
    Print("A ∩ B", Intersection(A, B));
    Print("A - B", Difference(A, B));
    return 0;
}
