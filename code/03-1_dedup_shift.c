#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int data[MAXSIZE];
    int length;
} SqList;

/* 删除下标为 i 的元素，后面的元素依次前移 */
void ListDelete(SqList *L, int i) {
    int k;
    for (k = i; k < L->length - 1; k++)
        L->data[k] = L->data[k + 1];
    L->length--;
}

/* 对每个元素，向后查找与它相等的元素并删除 */
void DeleteRepeat(SqList *L) {
    int i, j;
    for (i = 0; i < L->length; i++) {
        j = i + 1;
        while (j < L->length) {
            if (L->data[j] == L->data[i])
                ListDelete(L, j);   /* 删除后后继元素已移到 j，j 不动 */
            else
                j++;
        }
    }
}

void PrintList(SqList *L) {
    int i;
    printf("(");
    for (i = 0; i < L->length; i++)
        printf(i ? ", %d" : "%d", L->data[i]);
    printf(")\n");
}

int main(void) {
    SqList L = {{1, 2, 3, 1, 3, 4, 3, 5}, 8};   /* 题目中的示例数据 */
    printf("删除前：");
    PrintList(&L);
    DeleteRepeat(&L);
    printf("删除后：");
    PrintList(&L);
    return 0;
}
