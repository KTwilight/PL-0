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
		// ע�Ͳ�����Ҫ���ԣ����򽫴��������ż�
		if (symbol.symbolType != ST_null)
		{
			symbols.push_back(symbol);
		}
		// ���������������¼������Ϣ
		if (errorInfo.errorCode != 0)
		{
			errorInfos.push_back(errorInfo);
		}
	}
	// û�д�����Ϣ��ζ�ŷ����ɹ�
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


	// �����Զ������ؼ��ֻ��ʶ��
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
		//������Ӧ�ý���
		int keywordFlag = isKeyword(word);
		if (keywordFlag != -1)
		{
			symbol.symbolType = (SymbolType)(ST_begin + keywordFlag);
		}
		else
		{
			if (word.size() > identifierMaxLength)
			{
				errorInfo.errorCode = 12;	//��ѭ�سɶ����ģ���Ҳ�����Ӧ����ʲô��������
			}
			else
			{
				symbol.symbolType = ST_identifier;
			}
		}
	}
	//������
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
			errorInfo.errorCode = 1;	// ����̫��
		}
	}
	//��ֵ���
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
	//һ���������PL/0�ķ�����Ҫ���
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
	//��ϵ�����
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
	//����
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
	//�ָ���
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
	// �Ѿ��ﵽ����β��
	if (ch == '\0')
	{
		return '\0';
	}
	// �Ѿ��ﵽ��ǰ��ĩ
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
	//ʵ�ʸɵ��º�GetNextCharû������ֻ��ֱ�Ӱ����ӵ��ˡ�
}

char LexicalAnalyzer::peekNextChar()
{
	return *curContent;
}
