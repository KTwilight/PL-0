#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer()
{
	curContent = 0;
	curCoord = { 1, 1 };

}

LexicalAnalyzer::~LexicalAnalyzer()
{
}

bool LexicalAnalyzer::analyze(const char* content)
{
	curContent = content;
	symbols.clear();
	errorInfos.clear();
	curCoord = { 1, 1 };


	while (*curContent)
	{
		Symbol symbol;
		ErrorInfo errorInfo;
		symbol = getNextSymbol(errorInfo);
		// 注释部分需要忽略，否则将词语放入符号集
		if (symbol.symbolType != ST_null)
		{
			symbols.push_back(symbol);
		}
		// 若产生错误码则记录错误信息
		if (errorInfo.errorCode != 0)
		{
			errorInfos.push_back(errorInfo);
		}
	}
	// 没有错误信息意味着分析成功
	return errorInfos.empty();
}

Symbol LexicalAnalyzer::getNextSymbol(ErrorInfo& errorInfo)
{
	Symbol symbol;
	symbol.symbolType = ST_null;
	symbol.beg = curCoord;
	errorInfo.beg = curCoord;
	errorInfo.end = curCoord;
	errorInfo.errorCode = 0;

	char ch;
	while (isspace(peekNextChar()))
	{
		ignoreNextChar();
	}

	const char* startContent = curContent;
	symbol.beg = curCoord;

	ch = peekNextChar();


	// 有限自动机，关键字或标识符
	string word = "";
	if (isalpha(ch) || ch == '_')
	{
		word += ch;
		ignoreNextChar();
		ch = peekNextChar();
		while (isdigit(ch) || isalpha(ch) || ch == '_')
		{
			word += ch;
			ignoreNextChar();
			ch = peekNextChar();
		}
		curContent--;
		//按道理应该接受
		int keywordFlag = isKeyword(word);
		if (keywordFlag != -1)
		{
			symbol.symbolType = (SymbolType)(ST_begin + keywordFlag);
		}
		else
		{
			if (word.size() > identifierMaxLength)
			{
				errorInfo.errorCode = 12;	//因循守成而来的，我也不清楚应该是什么出错类型
			}
			else
			{
				symbol.symbolType = ST_identifier;
			}
		}
	}
	//是整数
	else if (isdigit(ch))
	{
		word += ch;
		ignoreNextChar();
		ch = peekNextChar();
		while (isdigit(ch))
		{
			word += ch;
			ignoreNextChar();
			ch = peekNextChar();
		}
		curContent--;
		if (word.size() < integerMaxLength)
			symbol.symbolType = ST_integer;
		else
		{
			errorInfo.errorCode = 1;	// 整数太长
		}
	}
	//赋值语句
	else if (ch == ':')
	{
		ignoreNextChar();
		ch = peekNextChar();
		if (ch == '=')
		{
			symbol.symbolType = ST_assign;
			word += ":=";
		}
		else
			errorInfo.errorCode = 3;
			
	}
	//一般运算符，PL/0文法不需要别的
	else if (ch == '+')
	{
		symbol.symbolType = ST_plus;
		word += ch;
	}
	else if (ch == '-')
	{
		symbol.symbolType = ST_minus;
		word += ch;
	}
	else if (ch == '*')
	{
		symbol.symbolType = ST_times;
		word += ch;
	}
	else if (ch == '/')
	{
		symbol.symbolType = ST_slash;
		word += ch;
	}
	else if (ch == '%')
	{
		symbol.symbolType = ST_mod;
		word += ch;
	}
	//关系运算符
	else if (ch == '=')
	{
		symbol.symbolType = ST_eql;
		word += ch;
	}
	else if (ch == '#')
	{
		symbol.symbolType = ST_neq;
		word += ch;
	}
	else if (ch == '<')
	{
		word += ch;
		symbol.symbolType = ST_less;
		ignoreNextChar();
		ch = peekNextChar();
		if (ch == '=')
		{
			symbol.symbolType = ST_lessOrEql;
			word += ch;
		}
		else
			curContent--;

	}
	else if (ch == '>')
	{
		word += ch;
		symbol.symbolType = ST_more;
		ignoreNextChar();
		ch = peekNextChar();
		if (ch == '=')
		{
			symbol.symbolType = ST_moreOrEql;
			word += ch;
		}
		else
			curContent--;
	}
	//括号
	else if (ch == '(')
	{
		symbol.symbolType = ST_lparen;
		word += ch;
	}
	else if (ch == ')')
	{
		symbol.symbolType = ST_rparen;
		word += ch;
	}
	//分隔符
	else if (ch == ',')
	{
		symbol.symbolType = ST_comma;
		word += ch;
	}
	else if (ch == '.')
	{
		symbol.symbolType = ST_period;
		word += ch;
	}
	else if (ch == ';')
	{
		symbol.symbolType = ST_semicolon;
		word += ch;
	}
	else
	{
		errorInfo.errorCode = 5; // [Error05] Unknown Character.
	}

	symbol.word = word;
	while (ch && isspace(peekNextChar()))
	{
		ch = getNextChar();
	}
	symbol.end = curCoord;
	errorInfo.end = curCoord;
	ignoreNextChar();
	return symbol;

}

char LexicalAnalyzer::getNextChar()
{
	char ch = *curContent;
	// 已经达到代码尾部
	if (ch == '\0')
	{
		return '\0';
	}
	// 已经达到当前行末
	if (ch == '\n')
	{
		++curCoord.row;
		curCoord.col = 1;
	}
	else
	{
		++curCoord.col;
	}

	++curContent;
	return ch;
}

void LexicalAnalyzer::ignoreNextChar()
{
	char ch = getNextChar();
	//实际干的事和GetNextChar没有区别，只是直接把他扔掉了。
}

char LexicalAnalyzer::peekNextChar()
{
	return *curContent;
}
