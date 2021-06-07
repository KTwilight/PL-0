#pragma once
#include "LexCommon.h"
#include <vector>
class LexicalAnalyzer
{
public:
	LexicalAnalyzer();
	~LexicalAnalyzer();

	bool analyze(const char* content);
	const vector<Symbol> &getSymbols()
	{
		return symbols;
	}

private:
	const char* keywords[keywordCount]
	{
		"begin",
		"call",
		"const",
		"do",
		"end",
		"if",
		"odd",
		"procedure",
		"read",
		"then",
		"var",
		"while",
		"write"
	};
	// ��ȡ��һ�����ţ����ƽ������е�����
	Symbol getNextSymbol(ErrorInfo& errorInfo);
	// ��ȡ��һ���ַ������ƽ������е�����
	char getNextChar();
	// ������һ���ַ������ƽ������е�����
	void ignoreNextChar();
	// ��ȡ��һ���ַ��������ƽ������е����ݣ���Ƶ�ԭ���ǣ�����Ҫ�ع��ĺ�����ơ�
	// ������ǻع���
	char peekNextChar();

	// �ж��Ƿ��ǹؼ��֣�����-1��
	int isKeyword(const string& word )
	{
		for (int i = 0; i < keywordCount; i++)
		{
			if (word == keywords[i])
				return i;
		}
		return -1;
	}
	const char* curContent;			// ��ǰ�����Ĵ�����������λ�á������Ѿ�����ĵ��ʶ��ԣ�����Զ�ǡ���һ������
	vector<Symbol> symbols;			// ���ż�
	CodeCoord curCoord;				// ��ǰ���꣬�������Ϊ�ڼ��еĵڼ������ʡ�
	vector<ErrorInfo> errorInfos;	// ������Ϣ

};

