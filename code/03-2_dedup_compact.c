#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int data[MAXSIZE];
    int length;
} SqList;

/* 一趟扫描：data[0..k-1] 是已保留的元素，没出现过的元素才往前搬 */
void DeleteRepeat(SqList *L) {
    int i, j, k = 0;
    for (i = 0; i < L->length; i++) {
        for (j = 0; j < k; j++)
            if (L->data[j] == L->data[i])
                break;
        if (j == k)                 /* 前面没有相同的，保留 */
            L->data[k++] = L->data[i];
    }
    L->length = k;
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
