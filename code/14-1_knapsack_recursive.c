#include <stdio.h>
#define MAXN 50

int w[MAXN], n;
int stack[MAXN], top = -1;  /* 栈中存放已放入背包的物品下标 */
int count = 0;

void PrintSolution(void) {
    int i;
    printf("方案 %d：", ++count);
    for (i = 0; i <= top; i++)
        printf("%d ", w[stack[i]]);
    printf("\n");
}

/* 从第 k 件物品开始考虑，背包还剩 rest 的空间 */
void Knap(int k, int rest) {
    int i;
    if (rest == 0) {
        PrintSolution();
        return;
    }
    for (i = k; i < n; i++) {
        if (w[i] <= rest) {
            stack[++top] = i;           /* 放入背包：入栈 */
            Knap(i + 1, rest - w[i]);
            top--;                      /* 取出来：出栈，试下一件 */
        }
    }
}

int main(void) {
    int s, i;
    printf("请输入背包容量 s 和物品件数 n：");
    scanf("%d %d", &s, &n);
    printf("请输入 %d 件物品的重量：", n);
    for (i = 0; i < n; i++)
        scanf("%d", &w[i]);
    Knap(0, s);
    if (count == 0)
        printf("无解\n");
    else
        printf("共 %d 种方案\n", count);
    return 0;
}
