#pragma once
#include "LexCommon.h"
#include <iostream>
#include <vector>
#include <queue>
#include <map>
using namespace std;

class SyntaxAnalyzer
{
public:

	SyntaxAnalyzer();
	bool analyze(const vector<Symbol>& symbols);


private:
	queue<Symbol> symbols;
	map<int, string> errorMessage;

	//	<程序>→<分程序>.
	int parseProgram();

	//	<分程序>→<常量说明部分><变量说明部分><过程说明部分><语句>
	int parseSubroutine();

	//	<常量说明部分>→(CONST) <常量定义><常量定义反复>(;)|ε
	int parseConstDescSection();

	//	<常量定义>→(标识符) = (无符号整数)
	int parseConstDefined();

	//	<常量定义反复>→(, ) < 常量定义 > <常量定义反复> | ε
	int parseConstDefinedLoop();

	//	< 常量定义反复1 > →<常量定义><常量定义反复2> 已被废除
	//	int parseConstDefinedLoop_1();

	//	<常量定义反复2>→(, ) < 常量定义反复1 > | ε 已被废除
	//	int parseConstDefinedLoop_2();

	//	<变量说明部分>→(VAR) < 标识符反复1 > (;) | ε 已被废除
	// <变量说明部分>→(VAR) (标识符)<标识符反复>(;)|ε
	int parseVarDescSection();

	//	<标识符反复>→(,) (标识符)<标识符反复>|ε
	int parseIdentifierLoop();

	//	<标识符反复1>→(标识符) < 标识符反复2 > 已被废除
	//	int parseIdentifierLoop_1();

	//	<标识符反复2>→(, ) < 标识符反复1 > | ε 已被废除
	//	int parseIdentifierLoop_2();
	
	//	<过程说明部分>→<过程首部><分程序>; <过程说明部分> | ε
	int parseProcedureDescSection();

	//	<过程首部>→(PROCEDURE)(标识符);
	int parseProcedureHeader();
	
	//	<语句>→<赋值语句> | <条件语句> | <当型循环语句> | <过程调用语句> | <读语句> | <写语句> | <复合语句> | ε
	int parseStatement();
	
	//	<赋值语句>→(标识符)(: = ) < 表达式 >
	int parseAssignmentStatement();
	
	//	<表达式>→+<项><项反复> |-<项><项反复> | <项><项反复>
	int parseExpression();
	
	// <项反复>→+<项><项反复> | -<项><项反复> | ε
	int parseItemLoop();

	//	<项反复1>→<项1><项反复2> 已被废除
	//	int parseItemLoop_1();

	//	<项反复2>→ + <项反复1> | -<项反复1> | ε 已被废除
	//	int parseItemLoop_2();

	// <项>→<因子><因子反复> 
	int parseItem();

	//	<项1>→<因子><项2> 已被废除
	//	int parseItem_1();

	//	<项2>→ * <项1>|/<项1> | ε 已被废除
	//	int parseItem_2();

	//	<因子>→(标识符) | (无符号整数) | '('表达式')'
	int parseFactor();

	// <因子反复> →*<因子> | /<因子> | ε
	int parseFactorLoop();

	//	< 复合语句 > →(BEGIN) <语句>< 语句反复> (END)
	int parseCompoundStatement();

	//	<语句反复>→;<语句><语句反复>|ε
	int parseStatementLoop();

	//	<条件语句>→(IF) <条件> (THEN) <语句>
	int parseConditionStatement();
	
	//	<条件>→<表达式><关系运算符><表达式> | (ODD) < 表达式 >
	int parseCondition();
	
	//	<当型循环语句>→(WHILE) < 条件 > do<语句>
	int parseWhileLoopStatement();
	
	//	<过程调用语句>→(CALL)(标识符)
	int parseCallStatement();
	
	//	< 读语句 > →(READ)'(' < 标识符反复1 > ')' 已被废除
	//  <读语句>→(READ)'(' < 标识符 > < 标识符反复> ')'
	int parseReadStatement();
	
	//	< 写语句 > →(WRITE)'(' < 表达式反复1 > ')' 已被废除
	// <写语句>→(WRITE)'(' <表达式><表达式反复>')'
	int parseWriteStatement();

	//	<表达式反复>→, <表达式><表达式反复> | ε
	int parseExpressionLoop();

	//	< 表达式反复1 > →<表达式><表达式反复2> 已被废除
	//	int parseExpresstionLoop_1();
	
	//	<表达式反复2>→, <表达式反复1> | ε
	//	int parseExpresstionLoop_2();

	// 不推进，获取当前的SymbolType
	const SymbolType& peekNextSymbolType();
	// 推进，然后获取SymbolType
	const SymbolType& getNextSymbolType();

	bool isROP(const SymbolType& SType);
};