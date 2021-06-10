#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer()
{
    errorMessage.insert(pair<int, string>(-1, "未知错误"));
    errorMessage.insert(pair<int, string>(1, "常数说明中的=写成:="));
    errorMessage.insert(pair<int, string>(2, "常数说明中的=后应是数字"));
    errorMessage.insert(pair<int, string>(3, "常数说明中的标识符后应是="));
    errorMessage.insert(pair<int, string>(4, "const, var，procedure后应为标识符"));
    errorMessage.insert(pair<int, string>(5, "漏掉了,或;"));
    errorMessage.insert(pair<int, string>(6, "过程说明后的符号不正确(应是语句开始符, 或过程定义符)"));
    errorMessage.insert(pair<int, string>(7, "应是语句开始符"));
    errorMessage.insert(pair<int, string>(8, "程序体内语句部分的后跟符不正确"));
    errorMessage.insert(pair<int, string>(9, "程序结尾丢了句号"));
    errorMessage.insert(pair<int, string>(10, "语句之间漏了;"));
    errorMessage.insert(pair<int, string>(11, "标识符未说明"));
    errorMessage.insert(pair<int, string>(12, "赋值语句中, 赋值号左部标识符属性应是变量"));
    errorMessage.insert(pair<int, string>(13, "赋值语句左部标识符后应是赋值号:="));
    errorMessage.insert(pair<int, string>(14, "call后应为标识符"));
    errorMessage.insert(pair<int, string>(15, "call后标识符属性应为过程"));
    errorMessage.insert(pair<int, string>(16, "条件语句中丢了then"));
    errorMessage.insert(pair<int, string>(17, "丢了end或;"));
    errorMessage.insert(pair<int, string>(18, "while型循环语句中丢了do"));
    errorMessage.insert(pair<int, string>(19, "语句后的符号不正确"));
    errorMessage.insert(pair<int, string>(20, "应为关系运算符"));
    errorMessage.insert(pair<int, string>(21, "表达式内标识符属性不能是过程"));
    errorMessage.insert(pair<int, string>(22, "表达式中漏掉右括号)"));
    errorMessage.insert(pair<int, string>(23, "因子后的非法符号"));
    errorMessage.insert(pair<int, string>(24, "表达式的开始符不能是此符号"));
    errorMessage.insert(pair<int, string>(31, "数越界"));
    errorMessage.insert(pair<int, string>(32, "read语句括号中的标识符不是变量"));

}

bool SyntaxAnalyzer::analyze(const vector<Symbol>& symbols)
{
    for (auto symbol : symbols)
    {
        this->symbols.push(symbol);
    }
    int errorCode = parseProgram();

    //    1 : 常数说明中的 = 写成:: - 。
    //    2 : 常数说明中的=后应是数字。
    //    3 : 常数说明中的标识符后应是=。
    //    4 : const, var，procedure后应为标识符。
    //    5 : 漏掉了, 或;。
    //    6 : 过程说明后的符号不正确(应是语句开始符, 或过程定义符)。
    //    7 : 应是语句开始符。
    //    8 : 程序体内语句部分的后跟符不正确。
    //    9 : 程序结尾丢了句号.。
    //    10 : 语句之间漏了; 。
    //    11 : 标识符未说明。
    //    12 : 赋值语句中, 赋值号左部标识符属性应是变量。
    //    13 : 赋值语句左部标识符后应是赋值号 : = 。
    //    14 : call后应为标识符。
    //    15 : call后标识符属性应为过程。
    //    16 : 条件语句中丢了then。
    //    17 : 丢了end或; 。
    //    18 : while型循环语句中丢了do。
    //    19 : 语句后的符号不正确。
    //    20 : 应为关系运算符。
    //    21 : 表达式内标识符属性不能是过程。
    //    22 : 表达式中漏掉右括号)。
    //    23 : 因子后的非法符号。
    //    24 : 表达式的开始符不能是此符号。
    //    31 : 数越界。
    //    32 : read语句括号中的标识符不是变量。
    if (errorCode != 0)
    {
        Symbol symbol;
        if (this->symbols.empty())
        {
            symbol = symbols.back();
        }
        else
        {
            symbol = this->symbols.front();
        }
        cout << "发生错误位于" << symbol.beg.row << "行" << symbol.beg.col << "列。" << endl;
        cout << "错误信息为" << errorMessage.at(errorCode) << endl;
        return false;
    }
    else
    {
        cout << "语法分析通过！" << endl;
        return true;
    }
}

int SyntaxAnalyzer::parseProgram()
{
    //	<程序>→<分程序>.
    SymbolType SType = symbols.front().symbolType;
    int errorCode;
    if (SType == ST_const ||
        SType == ST_var ||
        SType == ST_procedure ||
        SType == ST_identifier ||
        SType == ST_begin ||
        SType == ST_if ||
        SType == ST_while ||
        SType == ST_call ||
        SType == ST_read ||
        SType == ST_write
        )
    {

        errorCode = parseSubroutine();
        if (errorCode != 0)
            return errorCode;
        else
        {
            SType = symbols.front().symbolType;
            if (SType == ST_period)
                return 0;
            else
                return 9; //9:程序结尾丢了句号‘.’。
        }


    }
    else if (SType == ST_period)
        return 0;
    else
        return 9;   //9:程序结尾丢了句号‘.’。

}

int SyntaxAnalyzer::parseSubroutine()
{
    //	<分程序>→<常量说明部分><变量说明部分><过程说明部分><语句>

    SymbolType SType = symbols.front().symbolType;
    int errorCode;
    if (SType == ST_const ||
        SType == ST_var ||
        SType == ST_procedure ||
        SType == ST_identifier ||
        SType == ST_begin ||
        SType == ST_if ||
        SType == ST_while ||
        SType == ST_call ||
        SType == ST_read ||
        SType == ST_write ||
        SType == ST_period ||
        SType == ST_semicolon
        )
    {
        errorCode = parseConstDescSection();
        if (errorCode != 0)
            return errorCode;

        errorCode = parseVarDescSection();
        if (errorCode != 0)
            return errorCode;

        errorCode = parseProcedureDescSection();

        if (errorCode != 0)
            return errorCode;

        errorCode = parseStatement();
        if (errorCode != 0)
            return errorCode;

        else
            return 0;
    }
    else
        return -1;
}

int SyntaxAnalyzer::parseConstDescSection()
{
    //	<常量说明部分>→(CONST)<常量定义><常量定义反复>(;)|ε
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    //<常量说明部分>→(CONST)<常量定义><常量定义反复>(;)
    if (SType == ST_const)
    {

        symbols.pop();
        errorCode = parseConstDefined();
        if (errorCode != 0)
            return errorCode;

        errorCode = parseConstDefinedLoop();
        if (errorCode != 0)
            return errorCode;

        SType = peekNextSymbolType();
        if (SType == ST_semicolon)
        {
            symbols.pop();
            return 0;
        }
        else
            return 5;//5:漏掉了,或;。
    }
    //<常量说明部分>→ε
    else if (
        SType == ST_var ||
        SType == ST_procedure ||
        SType == ST_identifier ||
        SType == ST_begin ||
        SType == ST_if ||
        SType == ST_while ||
        SType == ST_call ||
        SType == ST_read ||
        SType == ST_write ||
        SType == ST_period)

        return 0;

    else
        return -1;
}

int SyntaxAnalyzer::parseConstDefined()
{
    //	<常量定义>→(标识符) = (无符号整数)
    SymbolType SType = symbols.front().symbolType;
    int errorCode;
    if (SType == ST_identifier)
    {
        symbols.pop();
        SType = symbols.front().symbolType;
        if (SType == ST_eql)
        {
            symbols.pop();
            SType = symbols.front().symbolType;
            if (SType == ST_integer)
            {
                symbols.pop();
                return 0;
            }
            else
                return 2;   //2 : 常数说明中的“=”后应是数字。

        }
        else if (SType == ST_assign)
            return 1; //1 : 常数说明中的“ = ”写成:: - ”。
        else
            return -1;

    }
    else
    {
        return -1; //?
    }
}

int SyntaxAnalyzer::parseConstDefinedLoop()
{	
    //	<常量定义反复>→(, ) < 常量定义 > <常量定义反复> | ε

    SymbolType SType = peekNextSymbolType();
    int errorCode;
    //<常量定义反复>→(, ) < 常量定义 > <常量定义反复>
    if (SType == ST_comma)
    {
        symbols.pop();
        errorCode = parseConstDefined();
        if (errorCode != 0)
            return errorCode;

        errorCode = parseConstDefinedLoop();

        return errorCode;
    }
    //常量定义反复>→ε
    else if(SType == ST_semicolon)
    {
        return 0;
    }
    else
    {
        return 5; //5 : 漏掉了, 或﹔。
    }
}

int SyntaxAnalyzer::parseVarDescSection()
{
    // <变量说明部分>→(VAR) (标识符) < 标识符反复 > (;) | ε
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    //   <变量说明部分>→(VAR) (标识符) < 标识符反复 > (;)
    if (SType == ST_var)
    {
        symbols.pop();
        SType = peekNextSymbolType();
        if (SType != ST_identifier)
            return 4; //4 : const, var，procedure后应为标识符。
        symbols.pop();

        errorCode = parseIdentifierLoop();
        if (errorCode != 0)
            return errorCode;

        SType = peekNextSymbolType();
        if (SType == ST_semicolon)
        {
            symbols.pop();
            return 0;
        }
        else
            return 5;   //5 : 漏掉了, 或；。

    }
    //<<变量说明部分>→ε

    else if (SType == ST_procedure ||
        SType == ST_identifier ||
        SType == ST_begin ||
        SType == ST_if ||
        SType == ST_while ||
        SType == ST_call ||
        SType == ST_read ||
        SType == ST_write ||
        SType == ST_period)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
int SyntaxAnalyzer::parseIdentifierLoop()
{
    //	<标识符反复>→(,) (标识符)<标识符反复>|ε
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_comma)
    {
        SType = getNextSymbolType();
        if (SType != ST_identifier)
            return -1;

        symbols.pop();
        errorCode = parseIdentifierLoop();
        return errorCode;
    }
    else if (
        SType == ST_semicolon ||
        SType == ST_rparen)
    {
        return 0;
    }
    else
    {
        return 5; // 5 : 漏掉了, 或﹔。
    }
}
int SyntaxAnalyzer::parseProcedureDescSection()
{
    //	<过程说明部分>→<过程首部><分程序>; <过程说明部分> | ε

    SymbolType SType = symbols.front().symbolType;
    int errorCode;
    //<过程说明部分>→<过程首部><分程序>; <过程说明部分>
    if (SType == ST_procedure)
    {
        errorCode = parseProcedureHeader();
        if (errorCode != 0)
            return errorCode;
        
        errorCode = parseSubroutine();
        
        if (errorCode != 0)
            return errorCode;
        SType = peekNextSymbolType();

        if (SType != ST_semicolon)
            return 5; //5 : 漏掉了, 或;。

        symbols.pop();
        errorCode = parseProcedureDescSection();
        
        return errorCode;

    }
    //	<常量定义反复2>→(, ) < 常量定义反复1 >
    //<过程说明部分>→ε
    else if (SType == ST_identifier ||
        SType == ST_begin ||
        SType == ST_if ||
        SType == ST_while ||
        SType == ST_call ||
        SType == ST_read ||
        SType == ST_write ||
        SType == ST_period)
    {
        return 0;
    }
    else
    {
        return -1; //
    }
}
int SyntaxAnalyzer::parseProcedureHeader()
{
    //	<过程首部>→(PROCEDURE)(标识符);

    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_procedure)
    {
        SType = getNextSymbolType();
        if (SType == ST_identifier)
        {
            symbols.pop();
            return 0;
        }
        else
            return 4; //4 : const, var，procedure后应为标识符。
    }
    else
    {
        return -1; //我真不知道都调用这函数了怎么会出问题
    }
}
int SyntaxAnalyzer::parseStatement()
{	
    //	<语句>→<赋值语句> | <条件语句> | <当型循环语句> | <过程调用语句> | <读语句> | <写语句> | <复合语句> | ε

    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_comma ||
        SType == ST_semicolon ||
        SType == ST_end||
        SType == ST_period)
    {
        return 0;
    }
    else if(
        SType == ST_identifier||
        SType == ST_if||
        SType == ST_while||
        SType == ST_procedure||
        SType == ST_read||
        SType == ST_write||
        SType == ST_begin||
        SType == ST_call
        )
    {
        //<语句>→<赋值语句>
        if (SType == ST_identifier)
        {
            errorCode = parseAssignmentStatement();
        }
        //<语句>→<条件语句>
        else if (SType == ST_if)
        {
            errorCode = parseConditionStatement();
        }
        //<语句>→<当型循环语句>

        else if (SType == ST_while)
        {
            errorCode = parseWhileLoopStatement();
        }
        //<语句>→<过程调用语句>

        else if (SType == ST_call)
        {
            errorCode = parseCallStatement();
        }
        //<语句>→<读语句>

        else if (SType == ST_read)
        {
            errorCode = parseReadStatement();
        }
        //<语句>→<写语句>

        else if (SType == ST_write)
        {
            errorCode = parseWriteStatement();
        }
        //<语句>→<复合语句>

        else if (SType == ST_begin)
        {
            errorCode = parseCompoundStatement();
        }
        else
        {
            errorCode = 0;
        }
        return errorCode;
    }
    else
    {
        return 7; //7 : 应是语句开始符。
    }

}
int SyntaxAnalyzer::parseAssignmentStatement()
{
    //<赋值语句>→(标识符)(:=)<表达式>
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_identifier)
    {
        SType = getNextSymbolType();
        if (SType != ST_assign)
            return 13;//13 : 赋值语句左部标识符后应是赋值号‘ : = ’。
        symbols.pop();
        errorCode = parseExpression();
        return errorCode;
    }
    else
        return -1; //这也能错？
}
int SyntaxAnalyzer::parseExpression()
{
    //	<表达式>→+<项><项反复> |-<项><项反复> | <项><项反复>
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_plus || SType == ST_minus)
    {
        symbols.pop();
        errorCode = parseItem();
        if (errorCode != 0)
            return errorCode;

        errorCode = parseItemLoop();
        return errorCode;
    }
    else if (
        SType == ST_identifier ||
        SType == ST_integer||
        SType == ST_lparen)
    {
        errorCode = parseItem();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseItemLoop();
        return errorCode;
    }
    else
    {
        return 24; //24:表达式的开始符不能是此符号。
    }
}
int SyntaxAnalyzer::parseItemLoop()
{	
    // <项反复>→+<项><项反复> | -<项><项反复> | ε
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    //<项反复2>→ <项反复>→+<项><项反复> | -<项><项反复>
    if (
        SType == ST_plus ||
        SType == ST_minus
        )
    {
        symbols.pop();
        errorCode = parseItem();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseItemLoop();
        return errorCode;

    }
    //<项反复2>→ ε
    else if (
        SType == ST_period ||
        SType == ST_semicolon ||
        SType == ST_rparen ||
        isROP(SType) ||
        SType == ST_end ||
        SType == ST_then ||
        SType == ST_do
        )
    {
        return 0;
    }
    else
    {
        return 23;  //23 : 因子后的非法符号。
    }

    return 0;
}
int SyntaxAnalyzer::parseItem()
{
    // <项>→<因子><因子反复> 
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_identifier ||
        SType == ST_integer ||
        SType == ST_lparen)
    {
        errorCode = parseFactor();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseFactorLoop();
        return errorCode;
    }
    else
    {
        return -1; //在这之前应该就被杀掉了
    }
}
int SyntaxAnalyzer::parseFactor()
{
    //	<因子>→(标识符) | (无符号整数) | '('表达式')'

    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_identifier || SType == ST_integer)
    {
        symbols.pop();
        return 0;
    }
    else if (SType == ST_lparen)
    {
        symbols.pop();
        errorCode = parseExpression();
        SType = peekNextSymbolType();
        if (SType != ST_rparen)
            return 22;//    22 : 表达式中漏掉右括号‘)’。
        symbols.pop();
        return 0;
    }
    else
    {
        return -1; //难以置信进来这里了还有什么错误。
    }
}
int SyntaxAnalyzer::parseFactorLoop()
{
    // <因子反复> →*<因子><因子反复> | /<因子><因子反复> | ε

    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_times ||
        SType == ST_slash)
    {
        symbols.pop();
        errorCode = parseFactor();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseFactorLoop();
        return errorCode;
    }
    else if (SType == ST_period ||
        SType == ST_semicolon ||
        SType == ST_rparen ||
        isROP(SType) ||
        SType == ST_plus||
        SType == ST_minus ||
        SType == ST_end ||
        SType == ST_then ||
        SType == ST_do)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
int SyntaxAnalyzer::parseCompoundStatement()
{	
    //	< 复合语句 > →(BEGIN) <语句>< 语句反复> (END)

    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_begin)
    {
        symbols.pop();
        errorCode = parseStatement();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseStatementLoop();
        if (errorCode != 0)
            return errorCode;
        SType = peekNextSymbolType();
        if (SType != ST_end)
            return 17;//    17 : 丢了end或;。
        symbols.pop();
        return 0;
    }
    else
    {
        return -1; //难以置信进来这里了还有什么错误。
    }
}
int SyntaxAnalyzer::parseStatementLoop()
{
    //	<语句反复>→;<语句><语句反复>|ε
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_semicolon)
    {
        symbols.pop();
        errorCode = parseStatement();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseStatementLoop();
        return errorCode;
    }
    else if (SType == ST_end)
    {
        return 0;
    }
    else
    {
        return 10; //10 : 语句之间漏了; 。
    }

}
int SyntaxAnalyzer::parseConditionStatement()
{
    //	<条件语句>→(IF) <条件> (THEN) <语句>
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_if)
    {
        symbols.pop();
        errorCode = parseCondition();
        if (errorCode != 0)
            return errorCode;
        
        SType = peekNextSymbolType();
        if (SType != ST_then)
            return 16;  //16 : 条件语句中丢了then。
        
        symbols.pop();
        errorCode = parseStatement();
        return errorCode;
    }
    else
    {
        return -1; //别的时候你进不来吧！？
    }
}

int SyntaxAnalyzer::parseCondition()
{
    //	<条件>→<表达式><关系运算符><表达式> | (ODD) < 表达式 >
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_plus ||
        SType == ST_minus ||
        SType == ST_identifier ||
        SType == ST_integer ||
        SType == ST_lparen)
    {
        errorCode = parseExpression();
        if (errorCode != 0)
            return errorCode;
        SType = peekNextSymbolType();
        if (!isROP(SType))
            return 20;
        symbols.pop();
        errorCode = parseExpression();
        return errorCode;
    }
    else if (SType == ST_odd)
    {
        symbols.pop();
        errorCode = parseExpression();
        return errorCode;
    }
    else
    {
        return -1; //条件里啥也不是
    }
}

int SyntaxAnalyzer::parseWhileLoopStatement()
{
    //	<当型循环语句>→(WHILE) < 条件 > do<语句>
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_while)
    {
        symbols.pop();
        errorCode = parseCondition();
        SType = peekNextSymbolType();
        if (SType != ST_do)
            return 18;// 18 : while型循环语句中丢了do。
        symbols.pop();
        errorCode = parseStatement();
        return errorCode;
    }
    else
    {
        return -1; //?
    }
}

int SyntaxAnalyzer::parseCallStatement()
{
    //	<过程调用语句>→(CALL)(标识符)    
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_call)
    {
        SType = getNextSymbolType();
        if (SType != ST_identifier)
            return 14; //14 : call后应为标识符。
        else
        {
            symbols.pop();
            return 0;
        }
    }
    else
    {
        return -1;
    }

}

int SyntaxAnalyzer::parseReadStatement()
{
    //  <读语句>→(READ)'(' < 标识符 > < 标识符反复> ')'
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_read)
    {
        SType = getNextSymbolType();
        if (SType != ST_lparen)
            return 19;//19 : 语句后的符号不正确。
        SType = getNextSymbolType();
        if (SType != ST_identifier)
            return 19; //19 : 语句后的符号不正确。
        symbols.pop();
        errorCode = parseIdentifierLoop();
        if (errorCode != 0)
            return errorCode;
        
        SType = peekNextSymbolType();
        if (SType != ST_rparen)
            return 22; //22 : 表达式中漏掉右括号)。
        symbols.pop();
        return 0;   
    }
    else
    {
        return -1;
    }
}

int SyntaxAnalyzer::parseWriteStatement()
{
	// <写语句>→(WRITE)'(' <表达式><表达式反复>')'
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_write)
    {
        SType = getNextSymbolType();
        if (SType != ST_lparen)
            return 19;//19 : 语句后的符号不正确。
        
        symbols.pop();
        errorCode = parseExpression();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseExpressionLoop();
        if (errorCode != 0)
            return errorCode;
        SType = peekNextSymbolType();

        if (SType != ST_rparen)
            return 22; //22 : 表达式中漏掉右括号)。
        symbols.pop();
        return 0;
    }
    else
    {
        return -1;
    }
}

int SyntaxAnalyzer::parseExpressionLoop()
{
    //	<表达式反复>→, <表达式><表达式反复> | ε

    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_comma)
    {
        symbols.pop();
        errorCode = parseExpression();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseExpressionLoop();
        return errorCode;
    }
    else if (SType == ST_rparen)
    {
        return 0;
    }
    else
    {
        return 22;//22 : 表达式中漏掉右括号)。
    }

    return 0;
}

const SymbolType& SyntaxAnalyzer::peekNextSymbolType()
{
    if (symbols.empty())
        return ST_null;
    else
        return symbols.front().symbolType;
}
const SymbolType& SyntaxAnalyzer::getNextSymbolType()
{
    if (!symbols.empty())
        symbols.pop();
    return peekNextSymbolType();
}
bool SyntaxAnalyzer::isROP(const SymbolType& SType)
{
    return (SType >= ST_eql && SType <= ST_moreOrEql);
}


