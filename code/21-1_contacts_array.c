#include <stdio.h>
#include <string.h>
#define MAXN 100

typedef struct {
    char name[20];
    char phone[20];
    char group[16];         /* 分组：同学、同事、家人等 */
} Contact;

typedef struct {
    Contact data[MAXN];
    int length;
} SqList;

SqList book;

/* 按姓名查找，返回下标，找不到返回 -1 */
int Locate(const char *name) {
    int i;
    for (i = 0; i < book.length; i++)
        if (strcmp(book.data[i].name, name) == 0)
            return i;
    return -1;
}

void Add(const char *name, const char *phone, const char *group) {
    Contact *c;
    if (Locate(name) >= 0) {
        printf("已经有联系人 %s\n", name);
        return;
    }
    if (book.length == MAXN) {
        printf("通讯录已满\n");
        return;
    }
    c = &book.data[book.length++];
    strcpy(c->name, name);
    strcpy(c->phone, phone);
    strcpy(c->group, group);
    printf("已添加：%s %s %s\n", name, phone, group);
}

void Query(const char *name) {
    int i = Locate(name);
    if (i < 0)
        printf("查无此人：%s\n", name);
    else
        printf("查询：%s %s %s\n", book.data[i].name, book.data[i].phone, book.data[i].group);
}

void Modify(const char *name, const char *phone, const char *group) {
    int i = Locate(name);
    if (i < 0) {
        printf("查无此人：%s\n", name);
        return;
    }
    strcpy(book.data[i].phone, phone);
    strcpy(book.data[i].group, group);
    printf("已修改：%s %s %s\n", name, phone, group);
}

/* 删除：后面的联系人依次前移 */
void Delete(const char *name) {
    int i = Locate(name);
    if (i < 0) {
        printf("查无此人：%s\n", name);
        return;
    }
    for (; i < book.length - 1; i++)
        book.data[i] = book.data[i + 1];
    book.length--;
    printf("已删除：%s\n", name);
}

/* 统计：总人数，以及每个分组的人数（按分组第一次出现的顺序） */
void Statistics(void) {
    char groups[MAXN][16];
    int count[MAXN], ng = 0, i, k;
    for (i = 0; i < book.length; i++) {
        for (k = 0; k < ng && strcmp(groups[k], book.data[i].group) != 0; k++)
            ;
        if (k == ng) {
            strcpy(groups[ng], book.data[i].group);
            count[ng++] = 0;
        }
        count[k]++;
    }
    printf("共 %d 人", book.length);
    for (k = 0; k < ng; k++)
        printf(k ? "，%s %d 人" : "：%s %d 人", groups[k], count[k]);
    printf("\n");
}

void List(void) {
    int i;
    printf("姓名\t电话\t\t分组\n");
    for (i = 0; i < book.length; i++)
        printf("%s\t%s\t%s\n", book.data[i].name, book.data[i].phone, book.data[i].group);
}

int main(void) {
    int op;
    char name[20], phone[20], group[16];
    printf("【菜单】1 添加  2 查询  3 修改  4 删除  5 统计  6 列表  0 退出\n");
    while (1) {
        printf("请选择操作：");
        if (scanf("%d", &op) != 1 || op == 0)
            break;
        switch (op) {
        case 1:
            printf("请输入姓名 电话 分组：");
            scanf("%19s %19s %15s", name, phone, group);
            Add(name, phone, group);
            break;
        case 2: printf("请输入姓名："); scanf("%19s", name); Query(name); break;
        case 3:
            printf("请输入姓名 新电话 新分组：");
            scanf("%19s %19s %15s", name, phone, group);
            Modify(name, phone, group);
            break;
        case 4: printf("请输入姓名："); scanf("%19s", name); Delete(name); break;
        case 5: Statistics(); break;
        case 6: List(); break;
        default: printf("没有这个操作\n");
        }
    }
    return 0;
}
