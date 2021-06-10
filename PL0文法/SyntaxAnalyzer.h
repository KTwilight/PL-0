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

	//	<����>��<�ֳ���>.
	int parseProgram();

	//	<�ֳ���>��<����˵������><����˵������><����˵������><���>
	int parseSubroutine();

	//	<����˵������>��(CONST) <��������><�������巴��>(;)|��
	int parseConstDescSection();

	//	<��������>��(��ʶ��) = (�޷�������)
	int parseConstDefined();

	//	<�������巴��>��(, ) < �������� > <�������巴��> | ��
	int parseConstDefinedLoop();

	//	< �������巴��1 > ��<��������><�������巴��2> �ѱ��ϳ�
	//	int parseConstDefinedLoop_1();

	//	<�������巴��2>��(, ) < �������巴��1 > | �� �ѱ��ϳ�
	//	int parseConstDefinedLoop_2();

	//	<����˵������>��(VAR) < ��ʶ������1 > (;) | �� �ѱ��ϳ�
	// <����˵������>��(VAR) (��ʶ��)<��ʶ������>(;)|��
	int parseVarDescSection();

	//	<��ʶ������>��(,) (��ʶ��)<��ʶ������>|��
	int parseIdentifierLoop();

	//	<��ʶ������1>��(��ʶ��) < ��ʶ������2 > �ѱ��ϳ�
	//	int parseIdentifierLoop_1();

	//	<��ʶ������2>��(, ) < ��ʶ������1 > | �� �ѱ��ϳ�
	//	int parseIdentifierLoop_2();
	
	//	<����˵������>��<�����ײ�><�ֳ���>; <����˵������> | ��
	int parseProcedureDescSection();

	//	<�����ײ�>��(PROCEDURE)(��ʶ��);
	int parseProcedureHeader();
	
	//	<���>��<��ֵ���> | <�������> | <����ѭ�����> | <���̵������> | <�����> | <д���> | <�������> | ��
	int parseStatement();
	
	//	<��ֵ���>��(��ʶ��)(: = ) < ���ʽ >
	int parseAssignmentStatement();
	
	//	<���ʽ>��+<��><���> |-<��><���> | <��><���>
	int parseExpression();
	
	// <���>��+<��><���> | -<��><���> | ��
	int parseItemLoop();

	//	<���1>��<��1><���2> �ѱ��ϳ�
	//	int parseItemLoop_1();

	//	<���2>�� + <���1> | -<���1> | �� �ѱ��ϳ�
	//	int parseItemLoop_2();

	// <��>��<����><���ӷ���> 
	int parseItem();

	//	<��1>��<����><��2> �ѱ��ϳ�
	//	int parseItem_1();

	//	<��2>�� * <��1>|/<��1> | �� �ѱ��ϳ�
	//	int parseItem_2();

	//	<����>��(��ʶ��) | (�޷�������) | '('���ʽ')'
	int parseFactor();

	// <���ӷ���> ��*<����> | /<����> | ��
	int parseFactorLoop();

	//	< ������� > ��(BEGIN) <���>< ��䷴��> (END)
	int parseCompoundStatement();

	//	<��䷴��>��;<���><��䷴��>|��
	int parseStatementLoop();

	//	<�������>��(IF) <����> (THEN) <���>
	int parseConditionStatement();
	
	//	<����>��<���ʽ><��ϵ�����><���ʽ> | (ODD) < ���ʽ >
	int parseCondition();
	
	//	<����ѭ�����>��(WHILE) < ���� > do<���>
	int parseWhileLoopStatement();
	
	//	<���̵������>��(CALL)(��ʶ��)
	int parseCallStatement();
	
	//	< ����� > ��(READ)'(' < ��ʶ������1 > ')' �ѱ��ϳ�
	//  <�����>��(READ)'(' < ��ʶ�� > < ��ʶ������> ')'
	int parseReadStatement();
	
	//	< д��� > ��(WRITE)'(' < ���ʽ����1 > ')' �ѱ��ϳ�
	// <д���>��(WRITE)'(' <���ʽ><���ʽ����>')'
	int parseWriteStatement();

	//	<���ʽ����>��, <���ʽ><���ʽ����> | ��
	int parseExpressionLoop();

	//	< ���ʽ����1 > ��<���ʽ><���ʽ����2> �ѱ��ϳ�
	//	int parseExpresstionLoop_1();
	
	//	<���ʽ����2>��, <���ʽ����1> | ��
	//	int parseExpresstionLoop_2();

	// ���ƽ�����ȡ��ǰ��SymbolType
	const SymbolType& peekNextSymbolType();
	// �ƽ���Ȼ���ȡSymbolType
	const SymbolType& getNextSymbolType();

	bool isROP(const SymbolType& SType);
};