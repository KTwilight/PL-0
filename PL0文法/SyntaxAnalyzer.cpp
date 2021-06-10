#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer()
{
    errorMessage.insert(pair<int, string>(-1, "δ֪����"));
    errorMessage.insert(pair<int, string>(1, "����˵���е�=д��:="));
    errorMessage.insert(pair<int, string>(2, "����˵���е�=��Ӧ������"));
    errorMessage.insert(pair<int, string>(3, "����˵���еı�ʶ����Ӧ��="));
    errorMessage.insert(pair<int, string>(4, "const, var��procedure��ӦΪ��ʶ��"));
    errorMessage.insert(pair<int, string>(5, "©����,��;"));
    errorMessage.insert(pair<int, string>(6, "����˵����ķ��Ų���ȷ(Ӧ����俪ʼ��, ����̶����)"));
    errorMessage.insert(pair<int, string>(7, "Ӧ����俪ʼ��"));
    errorMessage.insert(pair<int, string>(8, "����������䲿�ֵĺ��������ȷ"));
    errorMessage.insert(pair<int, string>(9, "�����β���˾��"));
    errorMessage.insert(pair<int, string>(10, "���֮��©��;"));
    errorMessage.insert(pair<int, string>(11, "��ʶ��δ˵��"));
    errorMessage.insert(pair<int, string>(12, "��ֵ�����, ��ֵ���󲿱�ʶ������Ӧ�Ǳ���"));
    errorMessage.insert(pair<int, string>(13, "��ֵ����󲿱�ʶ����Ӧ�Ǹ�ֵ��:="));
    errorMessage.insert(pair<int, string>(14, "call��ӦΪ��ʶ��"));
    errorMessage.insert(pair<int, string>(15, "call���ʶ������ӦΪ����"));
    errorMessage.insert(pair<int, string>(16, "��������ж���then"));
    errorMessage.insert(pair<int, string>(17, "����end��;"));
    errorMessage.insert(pair<int, string>(18, "while��ѭ������ж���do"));
    errorMessage.insert(pair<int, string>(19, "����ķ��Ų���ȷ"));
    errorMessage.insert(pair<int, string>(20, "ӦΪ��ϵ�����"));
    errorMessage.insert(pair<int, string>(21, "���ʽ�ڱ�ʶ�����Բ����ǹ���"));
    errorMessage.insert(pair<int, string>(22, "���ʽ��©��������)"));
    errorMessage.insert(pair<int, string>(23, "���Ӻ�ķǷ�����"));
    errorMessage.insert(pair<int, string>(24, "���ʽ�Ŀ�ʼ�������Ǵ˷���"));
    errorMessage.insert(pair<int, string>(31, "��Խ��"));
    errorMessage.insert(pair<int, string>(32, "read��������еı�ʶ�����Ǳ���"));

}

bool SyntaxAnalyzer::analyze(const vector<Symbol>& symbols)
{
    for (auto symbol : symbols)
    {
        this->symbols.push(symbol);
    }
    int errorCode = parseProgram();

    //    1 : ����˵���е� = д��:: - ��
    //    2 : ����˵���е�=��Ӧ�����֡�
    //    3 : ����˵���еı�ʶ����Ӧ��=��
    //    4 : const, var��procedure��ӦΪ��ʶ����
    //    5 : ©����, ��;��
    //    6 : ����˵����ķ��Ų���ȷ(Ӧ����俪ʼ��, ����̶����)��
    //    7 : Ӧ����俪ʼ����
    //    8 : ����������䲿�ֵĺ��������ȷ��
    //    9 : �����β���˾��.��
    //    10 : ���֮��©��; ��
    //    11 : ��ʶ��δ˵����
    //    12 : ��ֵ�����, ��ֵ���󲿱�ʶ������Ӧ�Ǳ�����
    //    13 : ��ֵ����󲿱�ʶ����Ӧ�Ǹ�ֵ�� : = ��
    //    14 : call��ӦΪ��ʶ����
    //    15 : call���ʶ������ӦΪ���̡�
    //    16 : ��������ж���then��
    //    17 : ����end��; ��
    //    18 : while��ѭ������ж���do��
    //    19 : ����ķ��Ų���ȷ��
    //    20 : ӦΪ��ϵ�������
    //    21 : ���ʽ�ڱ�ʶ�����Բ����ǹ��̡�
    //    22 : ���ʽ��©��������)��
    //    23 : ���Ӻ�ķǷ����š�
    //    24 : ���ʽ�Ŀ�ʼ�������Ǵ˷��š�
    //    31 : ��Խ�硣
    //    32 : read��������еı�ʶ�����Ǳ�����
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
        cout << "��������λ��" << symbol.beg.row << "��" << symbol.beg.col << "�С�" << endl;
        cout << "������ϢΪ" << errorMessage.at(errorCode) << endl;
        return false;
    }
    else
    {
        cout << "�﷨����ͨ����" << endl;
        return true;
    }
}

int SyntaxAnalyzer::parseProgram()
{
    //	<����>��<�ֳ���>.
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
                return 9; //9:�����β���˾�š�.����
        }


    }
    else if (SType == ST_period)
        return 0;
    else
        return 9;   //9:�����β���˾�š�.����

}

int SyntaxAnalyzer::parseSubroutine()
{
    //	<�ֳ���>��<����˵������><����˵������><����˵������><���>

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
    //	<����˵������>��(CONST)<��������><�������巴��>(;)|��
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    //<����˵������>��(CONST)<��������><�������巴��>(;)
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
            return 5;//5:©����,��;��
    }
    //<����˵������>����
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
    //	<��������>��(��ʶ��) = (�޷�������)
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
                return 2;   //2 : ����˵���еġ�=����Ӧ�����֡�

        }
        else if (SType == ST_assign)
            return 1; //1 : ����˵���еġ� = ��д��:: - ����
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
    //	<�������巴��>��(, ) < �������� > <�������巴��> | ��

    SymbolType SType = peekNextSymbolType();
    int errorCode;
    //<�������巴��>��(, ) < �������� > <�������巴��>
    if (SType == ST_comma)
    {
        symbols.pop();
        errorCode = parseConstDefined();
        if (errorCode != 0)
            return errorCode;

        errorCode = parseConstDefinedLoop();

        return errorCode;
    }
    //�������巴��>����
    else if(SType == ST_semicolon)
    {
        return 0;
    }
    else
    {
        return 5; //5 : ©����, ��r��
    }
}

int SyntaxAnalyzer::parseVarDescSection()
{
    // <����˵������>��(VAR) (��ʶ��) < ��ʶ������ > (;) | ��
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    //   <����˵������>��(VAR) (��ʶ��) < ��ʶ������ > (;)
    if (SType == ST_var)
    {
        symbols.pop();
        SType = peekNextSymbolType();
        if (SType != ST_identifier)
            return 4; //4 : const, var��procedure��ӦΪ��ʶ����
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
            return 5;   //5 : ©����, �򣻡�

    }
    //<<����˵������>����

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
    //	<��ʶ������>��(,) (��ʶ��)<��ʶ������>|��
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
        return 5; // 5 : ©����, ��r��
    }
}
int SyntaxAnalyzer::parseProcedureDescSection()
{
    //	<����˵������>��<�����ײ�><�ֳ���>; <����˵������> | ��

    SymbolType SType = symbols.front().symbolType;
    int errorCode;
    //<����˵������>��<�����ײ�><�ֳ���>; <����˵������>
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
            return 5; //5 : ©����, ��;��

        symbols.pop();
        errorCode = parseProcedureDescSection();
        
        return errorCode;

    }
    //	<�������巴��2>��(, ) < �������巴��1 >
    //<����˵������>����
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
    //	<�����ײ�>��(PROCEDURE)(��ʶ��);

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
            return 4; //4 : const, var��procedure��ӦΪ��ʶ����
    }
    else
    {
        return -1; //���治֪���������⺯������ô�������
    }
}
int SyntaxAnalyzer::parseStatement()
{	
    //	<���>��<��ֵ���> | <�������> | <����ѭ�����> | <���̵������> | <�����> | <д���> | <�������> | ��

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
        //<���>��<��ֵ���>
        if (SType == ST_identifier)
        {
            errorCode = parseAssignmentStatement();
        }
        //<���>��<�������>
        else if (SType == ST_if)
        {
            errorCode = parseConditionStatement();
        }
        //<���>��<����ѭ�����>

        else if (SType == ST_while)
        {
            errorCode = parseWhileLoopStatement();
        }
        //<���>��<���̵������>

        else if (SType == ST_call)
        {
            errorCode = parseCallStatement();
        }
        //<���>��<�����>

        else if (SType == ST_read)
        {
            errorCode = parseReadStatement();
        }
        //<���>��<д���>

        else if (SType == ST_write)
        {
            errorCode = parseWriteStatement();
        }
        //<���>��<�������>

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
        return 7; //7 : Ӧ����俪ʼ����
    }

}
int SyntaxAnalyzer::parseAssignmentStatement()
{
    //<��ֵ���>��(��ʶ��)(:=)<���ʽ>
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_identifier)
    {
        SType = getNextSymbolType();
        if (SType != ST_assign)
            return 13;//13 : ��ֵ����󲿱�ʶ����Ӧ�Ǹ�ֵ�š� : = ����
        symbols.pop();
        errorCode = parseExpression();
        return errorCode;
    }
    else
        return -1; //��Ҳ�ܴ�
}
int SyntaxAnalyzer::parseExpression()
{
    //	<���ʽ>��+<��><���> |-<��><���> | <��><���>
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
        return 24; //24:���ʽ�Ŀ�ʼ�������Ǵ˷��š�
    }
}
int SyntaxAnalyzer::parseItemLoop()
{	
    // <���>��+<��><���> | -<��><���> | ��
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    //<���2>�� <���>��+<��><���> | -<��><���>
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
    //<���2>�� ��
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
        return 23;  //23 : ���Ӻ�ķǷ����š�
    }

    return 0;
}
int SyntaxAnalyzer::parseItem()
{
    // <��>��<����><���ӷ���> 
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
        return -1; //����֮ǰӦ�þͱ�ɱ����
    }
}
int SyntaxAnalyzer::parseFactor()
{
    //	<����>��(��ʶ��) | (�޷�������) | '('���ʽ')'

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
            return 22;//    22 : ���ʽ��©�������š�)����
        symbols.pop();
        return 0;
    }
    else
    {
        return -1; //�������Ž��������˻���ʲô����
    }
}
int SyntaxAnalyzer::parseFactorLoop()
{
    // <���ӷ���> ��*<����><���ӷ���> | /<����><���ӷ���> | ��

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
    //	< ������� > ��(BEGIN) <���>< ��䷴��> (END)

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
            return 17;//    17 : ����end��;��
        symbols.pop();
        return 0;
    }
    else
    {
        return -1; //�������Ž��������˻���ʲô����
    }
}
int SyntaxAnalyzer::parseStatementLoop()
{
    //	<��䷴��>��;<���><��䷴��>|��
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
        return 10; //10 : ���֮��©��; ��
    }

}
int SyntaxAnalyzer::parseConditionStatement()
{
    //	<�������>��(IF) <����> (THEN) <���>
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
            return 16;  //16 : ��������ж���then��
        
        symbols.pop();
        errorCode = parseStatement();
        return errorCode;
    }
    else
    {
        return -1; //���ʱ����������ɣ���
    }
}

int SyntaxAnalyzer::parseCondition()
{
    //	<����>��<���ʽ><��ϵ�����><���ʽ> | (ODD) < ���ʽ >
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
        return -1; //������ɶҲ����
    }
}

int SyntaxAnalyzer::parseWhileLoopStatement()
{
    //	<����ѭ�����>��(WHILE) < ���� > do<���>
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_while)
    {
        symbols.pop();
        errorCode = parseCondition();
        SType = peekNextSymbolType();
        if (SType != ST_do)
            return 18;// 18 : while��ѭ������ж���do��
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
    //	<���̵������>��(CALL)(��ʶ��)    
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_call)
    {
        SType = getNextSymbolType();
        if (SType != ST_identifier)
            return 14; //14 : call��ӦΪ��ʶ����
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
    //  <�����>��(READ)'(' < ��ʶ�� > < ��ʶ������> ')'
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_read)
    {
        SType = getNextSymbolType();
        if (SType != ST_lparen)
            return 19;//19 : ����ķ��Ų���ȷ��
        SType = getNextSymbolType();
        if (SType != ST_identifier)
            return 19; //19 : ����ķ��Ų���ȷ��
        symbols.pop();
        errorCode = parseIdentifierLoop();
        if (errorCode != 0)
            return errorCode;
        
        SType = peekNextSymbolType();
        if (SType != ST_rparen)
            return 22; //22 : ���ʽ��©��������)��
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
	// <д���>��(WRITE)'(' <���ʽ><���ʽ����>')'
    SymbolType SType = peekNextSymbolType();
    int errorCode;
    if (SType == ST_write)
    {
        SType = getNextSymbolType();
        if (SType != ST_lparen)
            return 19;//19 : ����ķ��Ų���ȷ��
        
        symbols.pop();
        errorCode = parseExpression();
        if (errorCode != 0)
            return errorCode;
        errorCode = parseExpressionLoop();
        if (errorCode != 0)
            return errorCode;
        SType = peekNextSymbolType();

        if (SType != ST_rparen)
            return 22; //22 : ���ʽ��©��������)��
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
    //	<���ʽ����>��, <���ʽ><���ʽ����> | ��

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
        return 22;//22 : ���ʽ��©��������)��
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


