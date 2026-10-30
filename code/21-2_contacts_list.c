#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char name[20];
    char phone[20];
    char group[16];
    struct Node *next;
} Node;

Node head = {"", "", "", NULL};     /* 头结点 */

/* 返回姓名为 name 的结点的前驱，找不到返回 NULL */
Node *FindPre(const char *name) {
    Node *pre;
    for (pre = &head; pre->next != NULL; pre = pre->next)
        if (strcmp(pre->next->name, name) == 0)
            return pre;
    return NULL;
}

/* 添加：尾插，保持录入顺序 */
void Add(const char *name, const char *phone, const char *group) {
    Node *tail = &head, *s;
    if (FindPre(name) != NULL) {
        printf("已经有联系人 %s\n", name);
        return;
    }
    while (tail->next != NULL)
        tail = tail->next;
    s = (Node *)malloc(sizeof(Node));
    strcpy(s->name, name);
    strcpy(s->phone, phone);
    strcpy(s->group, group);
    s->next = NULL;
    tail->next = s;
    printf("已添加：%s %s %s\n", name, phone, group);
}

void Query(const char *name) {
    Node *pre = FindPre(name);
    if (pre == NULL)
        printf("查无此人：%s\n", name);
    else
        printf("查询：%s %s %s\n", pre->next->name, pre->next->phone, pre->next->group);
}

void Modify(const char *name, const char *phone, const char *group) {
    Node *pre = FindPre(name);
    if (pre == NULL) {
        printf("查无此人：%s\n", name);
        return;
    }
    strcpy(pre->next->phone, phone);
    strcpy(pre->next->group, group);
    printf("已修改：%s %s %s\n", name, phone, group);
}

/* 删除：找到前驱后改一个指针，不需要移动其他联系人 */
void Delete(const char *name) {
    Node *pre = FindPre(name), *p;
    if (pre == NULL) {
        printf("查无此人：%s\n", name);
        return;
    }
    p = pre->next;
    pre->next = p->next;
    free(p);
    printf("已删除：%s\n", name);
}

/* 统计：对每个分组，只在它第一次出现时数一遍同组人数 */
void Statistics(void) {
    Node *p, *q;
    int total = 0, first = 1, k;
    for (p = head.next; p != NULL; p = p->next)
        total++;
    printf("共 %d 人", total);
    for (p = head.next; p != NULL; p = p->next) {
        for (q = head.next; q != p && strcmp(q->group, p->group) != 0; q = q->next)
            ;
        if (q != p)                 /* 前面已经出现过这个分组 */
            continue;
        for (k = 0, q = p; q != NULL; q = q->next)
            if (strcmp(q->group, p->group) == 0)
                k++;
        printf(first ? "：%s %d 人" : "，%s %d 人", p->group, k);
        first = 0;
    }
    printf("\n");
}

void List(void) {
    Node *p;
    printf("姓名\t电话\t\t分组\n");
    for (p = head.next; p != NULL; p = p->next)
        printf("%s\t%s\t%s\n", p->name, p->phone, p->group);
}

int main(void) {
    int op;
    char name[20], phone[20], group[16];
    Node *p;
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
    while (head.next != NULL) {
        p = head.next;
        head.next = p->next;
        free(p);
    }
    return 0;
}
