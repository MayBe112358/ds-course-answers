#include <stdio.h>
#define MAXN 100

typedef struct {
    int id;                 /* 学号 */
    char name[20];
    int score[3];           /* 语文、数学、英语 */
    int total;
} Student;

typedef struct {
    Student data[MAXN];
    int length;
} SqList;

void Input(SqList *L) {
    int i;
    printf("请输入学生人数：");
    scanf("%d", &L->length);
    for (i = 0; i < L->length; i++) {
        Student *s = &L->data[i];
        printf("请输入第 %d 位同学的学号 姓名 语文 数学 英语：", i + 1);
        scanf("%d %19s %d %d %d", &s->id, s->name, &s->score[0], &s->score[1], &s->score[2]);
        s->total = s->score[0] + s->score[1] + s->score[2];
    }
}

/* 冒泡排序，按总分从高到低；只在严格小于时交换，总分相同的保持录入顺序 */
void SortByTotal(SqList *L) {
    int i, j, swapped;
    Student t;
    for (i = 0; i < L->length - 1; i++) {
        swapped = 0;
        for (j = 0; j < L->length - 1 - i; j++) {
            if (L->data[j].total < L->data[j + 1].total) {
                t = L->data[j];
                L->data[j] = L->data[j + 1];
                L->data[j + 1] = t;
                swapped = 1;
            }
        }
        if (!swapped)           /* 一趟下来没有交换，已经有序 */
            break;
    }
}

void Output(SqList *L) {
    int i, rank = 0;
    printf("名次\t学号\t姓名\t语文\t数学\t英语\t总分\n");
    for (i = 0; i < L->length; i++) {
        Student *s = &L->data[i];
        if (i == 0 || s->total != L->data[i - 1].total)
            rank = i + 1;       /* 总分相同名次并列 */
        printf("%d\t%d\t%s\t%d\t%d\t%d\t%d\n", rank, s->id, s->name,
               s->score[0], s->score[1], s->score[2], s->total);
    }
}

int main(void) {
    SqList L;
    Input(&L);
    SortByTotal(&L);
    Output(&L);
    return 0;
}
