#include <stdio.h>
#define MAXN 100

int main(void) {
    int no[MAXN], pwd[MAXN];
    int n, m, len, pos = 0, i;
    printf("请输入人数 n 和初始报数上限 m：");
    scanf("%d %d", &n, &m);
    for (i = 0; i < n; i++) {
        no[i] = i + 1;
        printf("请输入第 %d 个人的密码：", i + 1);
        scanf("%d", &pwd[i]);
    }
    printf("出列顺序：");
    for (len = n; len > 0; len--) {
        pos = (pos + m - 1) % len;  /* 从 pos 开始报数，报到 m 的人的位置 */
        printf("%d ", no[pos]);
        m = pwd[pos];
        for (i = pos; i < len - 1; i++) {   /* 删除 pos，后面的人前移 */
            no[i] = no[i + 1];
            pwd[i] = pwd[i + 1];
        }
        /* 前移后 pos 正好是下一个人；若删的是最后一个，下一个是 0 号 */
        if (pos == len - 1)
            pos = 0;
    }
    printf("\n");
    return 0;
}
