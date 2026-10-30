#include <stdio.h>
#include <string.h>
#define MAXN 200
#define DATAFILE "goods.dat"

typedef struct {
    int no;
    char name[30];
    int count;
} Goods;

Goods table[MAXN];          /* 按编号递增有序的顺序表 */
int len = 0;

/* 折半查找：找到返回下标；找不到返回 -1，pos 带回应插入的位置 */
int Search(int no, int *pos) {
    int low = 0, high = len - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (table[mid].no == no) {
            *pos = mid;
            return mid;
        }
        if (table[mid].no < no)
            low = mid + 1;
        else
            high = mid - 1;
    }
    *pos = low;
    return -1;
}

void StockIn(int no, const char *name, int count) {
    int pos, i;
    if (Search(no, &pos) < 0) {
        if (len == MAXN) {
            printf("商品表已满\n");
            return;
        }
        for (i = len; i > pos; i--)     /* 后面的商品后移，腾出 pos */
            table[i] = table[i - 1];
        table[pos].no = no;
        strcpy(table[pos].name, name);
        table[pos].count = 0;
        len++;
    }
    table[pos].count += count;
    printf("入库：%d %s，现有 %d\n", no, table[pos].name, table[pos].count);
}

void StockOut(int no, int count) {
    int pos, i;
    Goods *g;
    if (Search(no, &pos) < 0) {
        printf("没有编号为 %d 的商品\n", no);
        return;
    }
    g = &table[pos];
    if (g->count < count) {
        printf("库存不足：%d %s 只有 %d\n", g->no, g->name, g->count);
        return;
    }
    g->count -= count;
    printf("出库：%d %s，剩余 %d\n", g->no, g->name, g->count);
    if (g->count == 0) {
        printf("%s 已售完，从商品表中删除\n", g->name);
        for (i = pos; i < len - 1; i++)
            table[i] = table[i + 1];
        len--;
    }
}

void Query(int no) {
    int pos;
    if (Search(no, &pos) >= 0)
        printf("查询：%d %s，现有 %d\n", no, table[pos].name, table[pos].count);
    else
        printf("没有编号为 %d 的商品\n", no);
}

void List(void) {
    int i;
    printf("编号\t名称\t数量\n");
    for (i = 0; i < len; i++)
        printf("%d\t%s\t%d\n", table[i].no, table[i].name, table[i].count);
}

/* 二进制文件：先写商品种数，再把整个顺序表一次写入 */
void Restore(void) {
    FILE *fp = fopen(DATAFILE, "rb");
    len = 0;
    if (fp == NULL) {
        printf("没有找到 %s，从空库存开始营业\n", DATAFILE);
        return;
    }
    if (fread(&len, sizeof len, 1, fp) != 1 || len < 0 || len > MAXN ||
        fread(table, sizeof(Goods), len, fp) != (size_t)len)
        len = 0;
    fclose(fp);
    printf("营业前恢复了 %d 种商品\n", len);
}

void Save(void) {
    FILE *fp = fopen(DATAFILE, "wb");
    if (fp == NULL) {
        printf("无法写入 %s\n", DATAFILE);
        return;
    }
    fwrite(&len, sizeof len, 1, fp);
    fwrite(table, sizeof(Goods), len, fp);
    fclose(fp);
    printf("营业结束，%d 种商品已保存到 %s\n", len, DATAFILE);
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
    return 0;
}
