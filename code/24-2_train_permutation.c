#include <stdio.h>
#define MAXN 9

int perm[MAXN], n;

/* 用双向栈模拟调度，判断 perm 能否作为出站序列：左端是调度站，右端放已出站的车厢 */
int IsValid(void) {
    int space[MAXN], top1 = -1, top2 = MAXN, next = 1, k;
    for (k = 0; k < n; k++) {
        while (next <= perm[k])         /* 要出站的车厢还没进站，就让它及之前的车厢进站 */
            space[++top1] = next++;
        if (space[top1] != perm[k])     /* 它被压在下面，出不来 */
            return 0;
        space[--top2] = space[top1--];
    }
    return 1;
}

/* 按字典序生成下一个排列，已是最后一个时返回 0 */
int NextPerm(void) {
    int i = n - 2, j, t;
    while (i >= 0 && perm[i] > perm[i + 1])
        i--;
    if (i < 0)
        return 0;
    j = n - 1;
    while (perm[j] < perm[i])
        j--;
    t = perm[i]; perm[i] = perm[j]; perm[j] = t;
    for (i++, j = n - 1; i < j; i++, j--) {
        t = perm[i]; perm[i] = perm[j]; perm[j] = t;
    }
    return 1;
}

int main(void) {
    int i, count = 0, total = 0;
    printf("请输入车厢数 n（1~%d）：", MAXN);
    scanf("%d", &n);
    if (n < 1 || n > MAXN) {
        printf("n 超出范围\n");
        return 1;
    }
    for (i = 0; i < n; i++)
        perm[i] = i + 1;
    do {
        total++;
        if (IsValid()) {
            for (i = 0; i < n; i++)
                printf("%d", perm[i]);
            printf(++count % 7 == 0 ? "\n" : "  ");
        }
    } while (NextPerm());
    if (count % 7 != 0)
        printf("\n");
    printf("检查了 %d 个排列，其中 %d 种可以作为出站序列\n", total, count);
    return 0;
}
