#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATAFILE "goods.txt"

typedef struct Goods {
    int no;                 /* 商品编号，链表按编号递增有序 */
    char name[30];
    int count;
    struct Goods *next;
} Goods;

Goods head = {0, "", 0, NULL};  /* 头结点 */

/* 返回编号为 no 的商品的前驱：它的 next 要么就是该商品，要么是第一个编号更大的商品 */
Goods *FindPre(int no) {
    Goods *pre = &head;
    while (pre->next != NULL && pre->next->no < no)
        pre = pre->next;
    return pre;
}

void Clear(void) {
    Goods *p;
    while (head.next != NULL) {
        p = head.next;
        head.next = p->next;
        free(p);
    }
}

/* 1）入库：已有该商品就加数量，否则按编号插入新结点 */
void StockIn(int no, const char *name, int count) {
    Goods *pre = FindPre(no), *p = pre->next;
    if (p != NULL && p->no == no) {
        p->count += count;
    } else {
        p = (Goods *)malloc(sizeof(Goods));
        p->no = no;
        strcpy(p->name, name);
        p->count = count;
        p->next = pre->next;
        pre->next = p;
    }
    printf("入库：%d %s，现有 %d\n", p->no, p->name, p->count);
}

/* 2）出库：数量减到 0 就从表中删除 */
void StockOut(int no, int count) {
    Goods *pre = FindPre(no), *p = pre->next;
    if (p == NULL || p->no != no) {
        printf("没有编号为 %d 的商品\n", no);
        return;
    }
    if (p->count < count) {
        printf("库存不足：%d %s 只有 %d\n", p->no, p->name, p->count);
        return;
    }
    p->count -= count;
    printf("出库：%d %s，剩余 %d\n", p->no, p->name, p->count);
    if (p->count == 0) {
        pre->next = p->next;
        printf("%s 已售完，从商品表中删除\n", p->name);
        free(p);
    }
}

/* 3）查询 */
void Query(int no) {
    Goods *p = FindPre(no)->next;
    if (p != NULL && p->no == no)
        printf("查询：%d %s，现有 %d\n", p->no, p->name, p->count);
    else
        printf("没有编号为 %d 的商品\n", no);
}

void List(void) {
    Goods *p;
    printf("编号\t名称\t数量\n");
    for (p = head.next; p != NULL; p = p->next)
        printf("%d\t%s\t%d\n", p->no, p->name, p->count);
}

/* 4）营业前恢复：清空链表后从文本文件读回，文件本身有序，尾插即可 */
void Restore(void) {
    FILE *fp = fopen(DATAFILE, "r");
    Goods *tail = &head, *p, t;
    int n = 0;
    Clear();
    if (fp == NULL) {
        printf("没有找到 %s，从空库存开始营业\n", DATAFILE);
        return;
    }
    while (fscanf(fp, "%d %29s %d", &t.no, t.name, &t.count) == 3) {
        p = (Goods *)malloc(sizeof(Goods));
        *p = t;
        p->next = NULL;
        tail->next = p;
        tail = p;
        n++;
    }
    fclose(fp);
    printf("营业前恢复了 %d 种商品\n", n);
}

/* 5）营业结束保存：按编号顺序每行写一种商品 */
void Save(void) {
    FILE *fp = fopen(DATAFILE, "w");
    Goods *p;
    int n = 0;
    if (fp == NULL) {
        printf("无法写入 %s\n", DATAFILE);
        return;
    }
    for (p = head.next; p != NULL; p = p->next, n++)
        fprintf(fp, "%d %s %d\n", p->no, p->name, p->count);
    fclose(fp);
    printf("营业结束，%d 种商品已保存到 %s\n", n, DATAFILE);
}

int main(void) {
    int op, no, count;
    char name[30];
    printf("【菜单】1 入库  2 出库  3 查询  4 列表  5 营业前恢复  6 营业结束保存  0 退出\n");
    while (1) {
        printf("请选择操作：");
        if (scanf("%d", &op) != 1 || op == 0)
            break;
        switch (op) {
        case 1:
            printf("请输入编号 名称 数量：");
            scanf("%d %29s %d", &no, name, &count);
            StockIn(no, name, count);
            break;
        case 2:
            printf("请输入编号 数量：");
            scanf("%d %d", &no, &count);
            StockOut(no, count);
            break;
        case 3:
            printf("请输入编号：");
            scanf("%d", &no);
            Query(no);
            break;
        case 4: List(); break;
        case 5: Restore(); break;
        case 6: Save(); break;
        default: printf("没有这个操作\n");
        }
    }
    Clear();
    return 0;
}
