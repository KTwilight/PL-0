# PL-0
# 编译原理的作业，语法分析采用LL(1)文法 
# 改写后如下所示：

<程序>→<分程序>

<分程序>→<常量说明部分><变量说明部分><过程说明部分><语句>

<常量说明部分>→(CONST) <常量定义><常量定义反复>(;)|ε

<常量定义>→(标识符)=(无符号整数)

<常量定义反复>→(,)<常量定义><常量定义反复>|ε

<变量说明部分>→(VAR) (标识符)<标识符反复>(;)|ε

<标识符反复>→(,) (标识符)<标识符反复>|ε

<过程说明部分>→<过程首部><分程序>;<过程说明部分>|ε

<过程首部>→(PROCEDURE)(标识符);

<语句>→<赋值语句>|<条件语句>|<当型循环语句>|<过程调用语句>|<读语句>|<写语句>|<复合语句>|ε

<赋值语句>→(标识符)(:=)<表达式>

<表达式>→(+)<项><项反复> | (-)<项><项反复> | <项><项反复>

<项反复>→(+)<项><项反复> | (-)<项><项反复> | ε

<项>→<因子><因子反复> 

<因子>→(标识符)|(无符号整数)|'('表达式')'

<因子反复> →(*)<因子> | (/)<因子> | ε

<复合语句>→(BEGIN)<语句><语句反复>(END)

<语句反复>→(;)<语句><语句反复>|ε

<条件语句>→(IF)<条件>(THEN)<语句>

<条件>→<表达式><关系运算符><表达式>|(ODD)<表达式>

<当型循环语句>→(WHILE)<条件>(THEN)<语句>

<过程调用语句>→(CALL)(标识符)

<读语句>→(READ)'(' <标识符><标识符反复> ')'

<写语句>→(WRITE)'(' <表达式><表达式反复>')'

<表达式反复>→(,)<表达式><表达式反复>|ε

其余的代码中应该写得很详尽了，因此不作赘述。
