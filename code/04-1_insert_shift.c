#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int data[MAXSIZE];
    int length;
} SqList;

/* 从表尾往前比较，比 x 大的元素依次后移一位，空出来的位置放 x */
int InsertOrder(SqList *L, int x) {
    int i;
    if (L->length == MAXSIZE)
        return 0;
    for (i = L->length - 1; i >= 0 && L->data[i] > x; i--)
        L->data[i + 1] = L->data[i];
    L->data[i + 1] = x;
    L->length++;
    return 1;
}

void PrintList(SqList *L) {
    int i;
    for (i = 0; i < L->length; i++)
        printf("%d ", L->data[i]);
    printf("\n");
}

int main(void) {
    SqList L;
    int i, x;
    printf("请输入元素个数：");
    scanf("%d", &L.length);
    printf("请输入递增有序的 %d 个元素：", L.length);
    for (i = 0; i < L.length; i++)
        scanf("%d", &L.data[i]);
    printf("请输入要插入的元素 X：");
    scanf("%d", &x);
    if (InsertOrder(&L, x)) {
        printf("插入后：");
        PrintList(&L);
    } else {
        printf("表已满，无法插入\n");
    }
    return 0;
}
