#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { ATOM, LIST } ElemTag;

/* 头尾链表：表结点的 hp 指向表头元素，tp 指向表尾（剩下的元素组成的表）。空表用 NULL 表示 */
typedef struct GLNode {
    ElemTag tag;
    char atom;
    struct GLNode *hp, *tp;
} GLNode, *GList;

const char *s;              /* 解析位置 */

GList CreateElem(void);

/* 解析一个表，s 指向 '(' */
GList CreateList(void) {
    GList head = NULL, *pp = &head, p;
    s++;
    if (*s == ')') {
        s++;
        return NULL;
    }
    while (1) {
        p = (GList)malloc(sizeof(GLNode));
        p->tag = LIST;
        p->hp = CreateElem();
        p->tp = NULL;
        *pp = p;
        pp = &p->tp;
        if (*s++ != ',')    /* 读到 ')'，这一层结束 */
            break;
    }
    return head;
}

GList CreateElem(void) {
    GList p;
    if (*s == '(')
        return CreateList();
    p = (GList)malloc(sizeof(GLNode));
    p->tag = ATOM;
    p->atom = *s++;
    p->hp = p->tp = NULL;
    return p;
}

/* 检查括号配对、只含字母、逗号位置正确 */
int Valid(const char *str) {
    int depth = 0;
    char prev = 0;
    if (str[0] != '(')
        return 0;
    for (; *str; prev = *str++) {
        if (*str == '(') {
            if (prev && prev != '(' && prev != ',') return 0;
            depth++;
        } else if (*str == ')') {
            if (prev == ',' || --depth < 0) return 0;
        } else if (*str == ',') {
            if (prev == '(' || prev == ',' || depth == 0) return 0;
        } else if ((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z')) {
            if (prev != '(' && prev != ',') return 0;
        } else {
            return 0;
        }
        if (depth == 0 && str[1] != '\0') return 0;
    }
    return depth == 0;
}

void PrintList(GList L);

void PrintElem(GList e) {
    if (e == NULL)
        printf("()");
    else if (e->tag == ATOM)
        printf("%c", e->atom);
    else
        PrintList(e);
}

void PrintList(GList L) {
    printf("(");
    for (; L != NULL; L = L->tp) {
        PrintElem(L->hp);
        if (L->tp != NULL)
            printf(",");
    }
    printf(")");
}

GList GetHead(GList L) { return L->hp; }
GList GetTail(GList L) { return L->tp; }

int Find(GList L, char x) {
    if (L == NULL)
        return 0;
    if (L->tag == ATOM)
        return L->atom == x;
    return Find(L->hp, x) || Find(L->tp, x);
}

int main(void) {
    char str[200], x;
    GList L;
    printf("请输入广义表：");
    scanf("%199s", str);
    printf("请输入要查找的原子：");
    scanf(" %c", &x);
    if (!Valid(str)) {
        printf("广义表格式不正确\n");
        return 1;
    }
    s = str;
    L = CreateList();
    printf("广义表：");
    PrintList(L);
    printf("\n");
    if (L == NULL) {
        printf("空表没有表头和表尾\n");
    } else {
        printf("表头：");
        PrintElem(GetHead(L));
        printf("\n表尾：");
        PrintList(GetTail(L));
        printf("\n");
        if (GetTail(L) != NULL) {
            printf("表尾的表头：");
            PrintElem(GetHead(GetTail(L)));
            printf("\n");
        }
    }
    printf("查找 %c：%s\n", x, Find(L, x) ? "找到了" : "没有找到");
    return 0;
}
