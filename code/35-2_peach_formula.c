#include <stdio.h>
#define DAYS 10

int main(void) {
    int x = 1, d, formula, left, eat;

    /* 倒推：从第 10 天的 1 个往前推 9 天 */
    for (d = DAYS - 1; d >= 1; d--)
        x = (x + 1) * 2;

    /* 通项：x(d) + 2 = 2 × (x(d+1) + 2)，所以 x(1) + 2 = 2^9 × (1 + 2) */
    formula = 3 * (1 << (DAYS - 1)) - 2;

    printf("倒推：第 1 天共有 %d 个桃子\n", x);
    printf("公式：3 × 2^%d - 2 = %d\n", DAYS - 1, formula);

    /* 正向验证：按“吃一半再多吃一个”模拟，第 10 天应正好剩 1 个 */
    printf("正向验证：\n");
    left = x;
    for (d = 1; d < DAYS; d++) {
        eat = left / 2 + 1;
        printf("  第 %2d 天有 %4d 个，吃掉 %4d 个\n", d, left, eat);
        left -= eat;
    }
    printf("  第 %2d 天剩 %d 个\n", DAYS, left);
    return 0;
}
