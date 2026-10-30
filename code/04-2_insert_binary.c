#include <stdio.h>
#define MAXSIZE 100

typedef struct {
    int data[MAXSIZE];
    int length;
} SqList;

/* 折半查找第一个大于 x 的位置 low，把 low 及以后的元素后移，再放入 x */
int InsertOrder(SqList *L, int x) {
    int low = 0, high = L->length - 1, mid, i;
    if (L->length == MAXSIZE)
        return 0;
    while (low <= high) {
        mid = (low + high) / 2;
        if (L->data[mid] <= x)
            low = mid + 1;          /* 相等时插在后面 */
        else
            high = mid - 1;
    }
    for (i = L->length - 1; i >= low; i--)
        L->data[i + 1] = L->data[i];
    L->data[low] = x;
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
