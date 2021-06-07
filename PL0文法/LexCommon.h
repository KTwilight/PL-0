#pragma once
#include <string>
using namespace std;
constexpr int keywordOffset = 100;		// 关键字相对于符号类型枚举值的偏移
constexpr int keywordCount = 13;		// 关键字数目
constexpr int identifierMaxLength = 47;// 标识符最大长度
constexpr int integerMaxLength = 10;	// 整数最大长度


//枚举类型，字符的类型
enum SymbolType
{
	ST_null = 0,
	ST_begin,
	ST_call,
	ST_const,
	ST_do,
	ST_end,
	ST_if,
	ST_odd,
	ST_procedure,
	ST_read,
	ST_then,
	ST_var,
	ST_while,
	ST_write,

	//加法运算符
	ST_plus,		// +
	ST_minus,		// -
	//乘法运算符
	ST_times,		// *
	ST_slash,		// /
	ST_mod,			// %

	ST_lparen,		// (
	ST_rparen,		// )
	//关系运算符
	ST_eql,			// =
	ST_neq,			// #
	ST_less,		// <
	ST_lessOrEql,	// <=
	ST_more,		// >
	ST_moreOrEql,	// >=
	//分隔符
	ST_comma,		// ,
	ST_period,		// .
	ST_semicolon,	// ;
	ST_assign,		// :=

	ST_error,
	ST_identifier,
	ST_integer
};

struct CodeCoord
{
	int row;
	int col;
};
struct Symbol
{
	string word;
	CodeCoord beg{ 0,0 }, end{0,0};
	SymbolType symbolType = ST_null;
};

struct ErrorInfo
{
	CodeCoord beg, end;
	int errorCode;
};

