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

	// <����˵������>��(VAR) (��ʶ��)<��ʶ������>(;)|��
	int parseVarDescSection();

	//	<��ʶ������>��(,) (��ʶ��)<��ʶ������>|��
	int parseIdentifierLoop();

	
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

	// <��>��<����><���ӷ���> 
	int parseItem();

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

	//  <�����>��(READ)'(' < ��ʶ�� > < ��ʶ������> ')'
	int parseReadStatement();
	
	// <д���>��(WRITE)'(' <���ʽ><���ʽ����>')'
	int parseWriteStatement();

	//	<���ʽ����>��, <���ʽ><���ʽ����> | ��
	int parseExpressionLoop();

	// ���ƽ�����ȡ��ǰ��SymbolType
	const SymbolType& peekNextSymbolType();
	// �ƽ���Ȼ���ȡSymbolType
	const SymbolType& getNextSymbolType();

	bool isROP(const SymbolType& SType);
};