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
	// 获取下一个符号，并推进分析中的内容
	Symbol getNextSymbol(ErrorInfo& errorInfo);
	// 获取下一个字符，并推进分析中的内容
	char getNextChar();
	// 抛弃下一个字符，并推进分析中的内容
	void ignoreNextChar();
	// 获取下一个字符，但不推进分析中的内容，设计的原因是，不想要回滚的函数设计。
	// 结果还是回滚了
	char peekNextChar();

	// 判断是否是关键字，返回-1。
	int isKeyword(const string& word )
	{
		for (int i = 0; i < keywordCount; i++)
		{
			if (word == keywords[i])
				return i;
		}
		return -1;
	}
	const char* curContent;			// 当前遍历的代码内容所处位置。对于已经存入的单词而言，他永远是“下一个”。
	vector<Symbol> symbols;			// 符号集
	CodeCoord curCoord;				// 当前坐标，具体表现为第几行的第几个单词。
	vector<ErrorInfo> errorInfos;	// 错误消息

};

