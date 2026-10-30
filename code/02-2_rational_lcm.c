#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long num;               /* 分子 */
    long den;               /* 分母，始终为正 */
} Rational;

/* 辗转相除的递归写法 */
long Gcd(long a, long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    return b == 0 ? a : Gcd(b, a % b);
}

Rational Make(long num, long den) {
    Rational r;
    long g;
    if (den == 0) {
        printf("分母不能为 0\n");
        exit(1);
    }
    if (den < 0) {
        num = -num;
        den = -den;
    }
    g = Gcd(num, den);
    r.num = num / g;
    r.den = den / g;
    return r;
}

/* 加减法用最小公倍数通分，分母不会变成两个分母的乘积 */
Rational AddSub(Rational a, Rational b, int sign) {
    long lcm = a.den / Gcd(a.den, b.den) * b.den;
    return Make(a.num * (lcm / a.den) + sign * b.num * (lcm / b.den), lcm);
}

Rational Add(Rational a, Rational b) { return AddSub(a, b, 1); }
Rational Sub(Rational a, Rational b) { return AddSub(a, b, -1); }

/* 乘法先交叉约分再相乘：a 的分子和 b 的分母约，b 的分子和 a 的分母约 */
Rational Mul(Rational a, Rational b) {
    long g1 = Gcd(a.num, b.den), g2 = Gcd(b.num, a.den);
    return Make((a.num / g1) * (b.num / g2), (a.den / g2) * (b.den / g1));
}

long Numerator(Rational r) { return r.num; }
long Denominator(Rational r) { return r.den; }

void Print(const char *label, Rational r) {
    if (r.den == 1)
        printf("%s = %ld\n", label, r.num);
    else
        printf("%s = %ld/%ld\n", label, r.num, r.den);
}

int main(void) {
    long a, b, c, d;
    Rational r1, r2;
    printf("请输入两个有理数的分子和分母（a b c d 表示 a/b 和 c/d）：");
    scanf("%ld %ld %ld %ld", &a, &b, &c, &d);
    r1 = Make(a, b);
    r2 = Make(c, d);
    Print("r1", r1);
    Print("r2", r2);
    Print("r1 + r2", Add(r1, r2));
    Print("r1 - r2", Sub(r1, r2));
    Print("r1 * r2", Mul(r1, r2));
    printf("r1 的分子 = %ld，分母 = %ld\n", Numerator(r1), Denominator(r1));
    printf("r2 的分子 = %ld，分母 = %ld\n", Numerator(r2), Denominator(r2));
    return 0;
}
