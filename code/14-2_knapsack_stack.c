#include <stdio.h>
#define MAXN 50

int main(void) {
    int w[MAXN], stack[MAXN], top = -1;
    int s, n, i, k = 0, rest, count = 0;
    printf("请输入背包容量 s 和物品件数 n：");
    scanf("%d %d", &s, &n);
    printf("请输入 %d 件物品的重量：", n);
    for (i = 0; i < n; i++)
        scanf("%d", &w[i]);

    rest = s;
    while (1) {
        while (rest > 0 && k < n) {     /* 从第 k 件起，放得下的就放进去 */
            if (w[k] <= rest) {
                stack[++top] = k;
                rest -= w[k];
            }
            k++;
        }
        if (rest == 0) {                /* 正好装满，栈中就是一组解 */
            printf("方案 %d：", ++count);
            for (i = 0; i <= top; i++)
                printf("%d ", w[stack[i]]);
            printf("\n");
        }
        if (top < 0)                    /* 栈空且所有物品都试过了 */
            break;
        k = stack[top--];               /* 取出最后放入的物品，从它的下一件接着试 */
        rest += w[k];
        k++;
    }
    if (count == 0)
        printf("无解\n");
    else
        printf("共 %d 种方案\n", count);
    return 0;
}
