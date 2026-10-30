# 课程设计答案库

数据结构课程设计（一）线性结构设计题目 36 道，每题两种 C 语言解法。

- 网站：https://maybe112358.github.io/ds-course-answers/
- 题目原文：[课程设计（一）选题.md](<课程设计（一）选题.md>)
- 一键下载全部代码：[ds-course-answers-code.zip](ds-course-answers-code.zip)
- 全部代码都按 C99 用 gcc 编译通过（`-Wall -Wextra` 无警告），并用示例数据运行验证。

| 题号 | 题目 | 解法一 | 解法二 |
| :-: | --- | --- | --- |
| 01 | 学生成绩统计 | [顺序表 + 冒泡排序](code/01-1_score_array.c) | [单链表 + 有序插入](code/01-2_score_list.c) |
| 02 | 有理数计算器 | [运算后统一约分](code/02-1_rational_basic.c) | [先约后算，减少溢出](code/02-2_rational_lcm.c) |
| 03 | 顺序表中重复元素的删除 | [逐个删除，后继前移](code/03-1_dedup_shift.c) | [一趟扫描，原地压缩](code/03-2_dedup_compact.c) |
| 04 | 有序顺序表的指定元素的保序插入 | [从后往前比较并后移](code/04-1_insert_shift.c) | [折半查找插入位置](code/04-2_insert_binary.c) |
| 05 | 单链表中重复元素的删除 | [双重循环删除](code/05-1_dedup_list.c) | [标记数组一趟扫描](code/05-2_dedup_mark.c) |
| 06 | 有序单链表的指定元素的保序插入 | [带头结点，前驱指针](code/06-1_list_insert_head.c) | [不带头结点，二级指针](code/06-2_list_insert_pp.c) |
| 07 | 集合运算演示 | [有序顺序表归并](code/07-1_set_sorted.c) | [位向量](code/07-2_set_bits.c) |
| 08 | 约瑟夫环问题 | [循环单链表](code/08-1_josephus_list.c) | [顺序表 + 下标取余](code/08-2_josephus_array.c) |
| 09 | 一元稀疏多项式计算器 | [单链表，有序插入建表](code/09-1_sparse_poly_list.c) | [顺序表，排序后合并](code/09-2_sparse_poly_array.c) |
| 10 | 库房管理 | [有序单链表 + 文本文件](code/10-1_store_list.c) | [有序顺序表 + 折半查找 + 二进制文件](code/10-2_store_array.c) |
| 11 | 顺序栈模拟器 | [静态顺序栈](code/11-1_sqstack_static.c) | [动态顺序栈，满了扩容](code/11-2_sqstack_dynamic.c) |
| 12 | 表达式括号匹配判断器 | [顺序栈](code/12-1_match_stack.c) | [计数器代替栈](code/12-2_match_counter.c) |
| 13 | 八皇后问题 | [递归回溯 + 标记数组](code/13-1_queens_recursive.c) | [非递归，数组模拟栈](code/13-2_queens_stack.c) |
| 14 | 背包问题 | [递归 + 栈记录方案](code/14-1_knapsack_recursive.c) | [非递归，栈模拟回溯](code/14-2_knapsack_stack.c) |
| 15 | 停车场管理 | [顺序栈 + 链队列](code/15-1_parking_sqstack.c) | [链栈 + 循环队列](code/15-2_parking_linkstack.c) |
| 16 | 实验室管理 | [三个链队列](code/16-1_lab_linkqueue.c) | [三个循环队列](code/16-2_lab_sqqueue.c) |
| 17 | 打印杨辉三角 | [循环队列，边出边生成](code/17-1_yanghui_sqqueue.c) | [链队列 + 0 作分隔符](code/17-2_yanghui_linkqueue.c) |
| 18 | 运动会的日程安排 | [队列划分子集](code/18-1_sports_queue.c) | [回溯求最少时间段](code/18-2_sports_backtrack.c) |
| 19 | 串基本操作演示器 | [定长顺序串 + 朴素匹配](code/19-1_string_fixed.c) | [堆分配串 + KMP 匹配](code/19-2_string_heap_kmp.c) |
| 20 | 魔方矩阵 | [按题目规则逐轮填写](code/20-1_magic_rounds.c) | [楼梯法](code/20-2_magic_siamese.c) |
| 21 | 通讯录管理 | [顺序表](code/21-1_contacts_array.c) | [带头结点单链表](code/21-2_contacts_list.c) |
| 22 | 稀疏矩阵运算器 | [行逻辑链接三元组](code/22-1_sparse_rls.c) | [普通三元组，逐个扫描](code/22-2_sparse_triple.c) |
| 23 | 算术表达式求解 | [算符优先，双栈直接求值](code/23-1_expr_two_stacks.c) | [先转后缀式，再求值](code/23-2_expr_postfix.c) |
| 24 | 车厢调度问题 | [递归 + 双向栈](code/24-1_train_recursive.c) | [全排列 + 逐个检验](code/24-2_train_permutation.c) |
| 25 | 广义表表头、表尾识别器 | [头尾链表存储](code/25-1_glist_headtail.c) | [扩展线性链表存储](code/25-2_glist_sibling.c) |
| 26 | 长整数的加法运算 | [双向循环链表](code/26-1_longint_dlist.c) | [单链表，低位在前](code/26-2_longint_slist.c) |
| 27 | 一元多项式计算 | [单链表归并](code/27-1_poly_list.c) | [数组下标即指数](code/27-2_poly_array.c) |
| 28 | 迷宫问题 | [链栈，非递归](code/28-1_maze_stack.c) | [递归，链栈记录路径](code/28-2_maze_recursive.c) |
| 29 | 任意长的整数加法运算 | [顺序表，万进制](code/29-1_bigadd_array.c) | [字符栈，逐位相加](code/29-2_bigadd_stack.c) |
| 30 | 任意长的整数乘法运算 | [单链表，万进制竖式乘法](code/30-1_bigmul_list.c) | [顺序表，十进制，最后统一进位](code/30-2_bigmul_array.c) |
| 31 | 任意长的整数除法运算 | [十进制竖式，减法试商](code/31-1_bigdiv_digit.c) | [万进制竖式，二分试商](code/31-2_bigdiv_binary.c) |
| 32 | 稀疏矩阵相乘运算 | [快速转置后按行归并](code/32-1_spmul_transpose.c) | [rpos 定位，逐行累加](code/32-2_spmul_rpos.c) |
| 33 | 逆波兰式 | [运算符栈](code/33-1_rpn_stack.c) | [递归下降](code/33-2_rpn_recursive.c) |
| 34 | 文本处理 | [顺序串，逐次前移删除](code/34-1_text_shift.c) | [双指针一趟完成](code/34-2_text_two_pointers.c) |
| 35 | 猴子吃桃子问题 | [链表、栈、递归三种实现](code/35-1_peach_three_ways.c) | [循环倒推 + 通项公式](code/35-2_peach_formula.c) |
| 36 | 电梯模拟问题 | [固定步长时钟](code/36-1_elevator_tick.c) | [事件表驱动（下一事件推进）](code/36-2_elevator_event.c) |
