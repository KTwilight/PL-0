#pragma once
#include <string>
using namespace std;
constexpr int keywordOffset = 100;		// �ؼ�������ڷ�������ö��ֵ��ƫ��
constexpr int keywordCount = 13;		// �ؼ�����Ŀ
constexpr int identifierMaxLength = 47;// ��ʶ����󳤶�
constexpr int integerMaxLength = 10;	// ������󳤶�


//ö�����ͣ��ַ�������
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

	//�ӷ������
	ST_plus,		// +
	ST_minus,		// -
	//�˷������
	ST_times,		// *
	ST_slash,		// /
	ST_mod,			// %

	ST_lparen,		// (
	ST_rparen,		// )
	//��ϵ�����
	ST_eql,			// =
	ST_neq,			// #
	ST_less,		// <
	ST_lessOrEql,	// <=
	ST_more,		// >
	ST_moreOrEql,	// >=
	//�ָ���
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

