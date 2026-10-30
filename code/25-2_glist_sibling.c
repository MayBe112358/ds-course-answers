#include <stdio.h>
#include <stdlib.h>

typedef enum { ATOM, LIST } ElemTag;

/* 扩展线性链表：每个元素一个结点，tp 指向同一层的下一个元素；
   子表结点的 hp 指向子表的第一个元素，空表是一个 hp 为 NULL 的子表结点 */
typedef struct GLNode {
    ElemTag tag;
    char atom;
    struct GLNode *hp, *tp;
} GLNode, *GList;

const char *s;

GList CreateElem(void) {
    GList p = (GList)malloc(sizeof(GLNode)), *pp;
    p->tp = p->hp = NULL;
    if (*s != '(') {
        p->tag = ATOM;
        p->atom = *s++;
        return p;
    }
    p->tag = LIST;
    s++;
    if (*s == ')') {
        s++;
        return p;
    }
    pp = &p->hp;
    while (1) {
        *pp = CreateElem();
        pp = &(*pp)->tp;
        if (*s++ != ',')
            break;
    }
    return p;
}

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

/* 只输出这个元素本身，不跟着 tp 输出后面的兄弟 */
void Print(GList e) {
    GList p;
    if (e->tag == ATOM) {
        printf("%c", e->atom);
        return;
    }
    printf("(");
    for (p = e->hp; p != NULL; p = p->tp) {
        Print(p);
        if (p->tp != NULL)
            printf(",");
    }
    printf(")");
}

/* 表头就是第一个元素 */
GList GetHead(GList L) { return L->hp; }

/* 表尾：新建一个子表结点，让它的 hp 指向第二个元素，与原表共享后面的结点 */
GList GetTail(GList L) {
    GList t = (GList)malloc(sizeof(GLNode));
    t->tag = LIST;
    t->hp = L->hp->tp;
    t->tp = NULL;
    return t;
}

int Find(GList e, char x) {
    GList p;
    if (e->tag == ATOM)
        return e->atom == x;
    for (p = e->hp; p != NULL; p = p->tp)
        if (Find(p, x))
            return 1;
    return 0;
}

int main(void) {
    char str[200], x;
    GList L, T;
    printf("请输入广义表：");
    scanf("%199s", str);
    printf("请输入要查找的原子：");
    scanf(" %c", &x);
    if (!Valid(str)) {
        printf("广义表格式不正确\n");
        return 1;
    }
    s = str;
    L = CreateElem();
    printf("广义表：");
    Print(L);
    printf("\n");
    if (L->hp == NULL) {
        printf("空表没有表头和表尾\n");
    } else {
        printf("表头：");
        Print(GetHead(L));
        T = GetTail(L);
        printf("\n表尾：");
        Print(T);
        printf("\n");
        if (T->hp != NULL) {
            printf("表尾的表头：");
            Print(GetHead(T));
            printf("\n");
        }
        free(T);
    }
    printf("查找 %c：%s\n", x, Find(L, x) ? "找到了" : "没有找到");
    return 0;
}
