#include <stdio.h>
#include <string.h>
#define MAXLEN 1000

/* 用 ASCII 范围直接判断字符类别 */
void Statistics(const char *s) {
    int letters = 0, digits = 0, spaces = 0, others = 0;
    for (; *s; s++) {
        if ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z')) letters++;
        else if (*s >= '0' && *s <= '9') digits++;
        else if (*s == ' ') spaces++;
        else others++;
    }
    printf("英文字母 %d 个，数字 %d 个，空格 %d 个，其他字符 %d 个\n", letters, digits, spaces, others);
}

/* 双指针一趟完成计数和删除：i 读、w 写。i 处匹配就跳过整个子串并计数，否则把字符搬到 w。
   每个字符最多搬一次，不会像逐次前移那样反复移动后面的内容 */
int CountAndDelete(char *s, const char *t) {
    int i = 0, w = 0, m = (int)strlen(t), n = (int)strlen(s), cnt = 0;
    while (i < n) {
        if (i + m <= n && strncmp(s + i, t, m) == 0) {
            cnt++;
            i += m;
        } else {
            s[w++] = s[i++];
        }
    }
    s[w] = '\0';
    return cnt;
}

int main(void) {
    char s[MAXLEN + 1], t[MAXLEN + 1];
    int cnt;
    printf("请输入一行文本：");
    fgets(s, sizeof s, stdin);
    s[strcspn(s, "\r\n")] = '\0';
    printf("请输入要统计和删除的字符串：");
    fgets(t, sizeof t, stdin);
    t[strcspn(t, "\r\n")] = '\0';
    if (t[0] == '\0') {
        printf("要查找的字符串不能为空\n");
        return 1;
    }
    Statistics(s);
    cnt = CountAndDelete(s, t);
    printf("\"%s\" 出现了 %d 次\n", t, cnt);
    printf("删除后：%s\n", s);
    return 0;
}
