run	r	运行程序
continue	c	继续执行
next	n	单步执行（不进入函数）
step	s	单步进入函数
finish	fin	执行到当前函数返回
quit	q	退出 GDB
# 运行程序
(gdb) run

# 当崩溃发生后，查看堆栈跟踪
(gdb) backtrace
(gdb) bt  # 简写

# 查看当前帧的局部变量
(gdb) info locals

# 查看寄存器的值
(gdb) info registers

# 查看崩溃位置的源代码
(gdb) list