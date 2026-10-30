#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long num;               /* 分子 */
    long den;               /* 分母，始终为正 */
} Rational;

long Gcd(long a, long b) {
    long r;
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* 1）由分子和分母生成有理数：符号放在分子上，并约成最简分数 */
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
    g = Gcd(num, den);              /* num 为 0 时 g 等于 den，结果是 0/1 */
    r.num = num / g;
    r.den = den / g;
    return r;
}

/* 2）3）4）和、差、积：直接交叉相乘，交给 Make 约分 */
Rational Add(Rational a, Rational b) { return Make(a.num * b.den + b.num * a.den, a.den * b.den); }
Rational Sub(Rational a, Rational b) { return Make(a.num * b.den - b.num * a.den, a.den * b.den); }
Rational Mul(Rational a, Rational b) { return Make(a.num * b.num, a.den * b.den); }

/* 5）6）分子、分母 */
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
