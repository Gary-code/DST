#pragma warning(disable : 4996)  
#include <iostream>
#include <cstring>
#include <map>
#include <set>
#include <stack>
#include <cstdlib>

#define DATATYPE_INT 1
#define DATATYPE_STRING 2
#define DATATYPE_DOUBLE 3
#define DATATYPE_BOOL 4
#define RESULT_MAX_LEN 256
using namespace std;



set<char> g_setOperators;


//������
struct tagOperand
{
	int nType;         //���������� 
	string strName;    //���� 
	string strValue;   //ֵ 
};


//��ʼ������
void initset()//set������ʼ�� 
{
	g_setOperators.clear();
	g_setOperators.insert('+');
	g_setOperators.insert('-');
	g_setOperators.insert('*');
	g_setOperators.insert('/');
	g_setOperators.insert('(');
	g_setOperators.insert(')');
}





class CParserExpr;


//������
struct tagOperator
{
	string strOperator; 				//����������
	int nType;          				//0,operater;1sential;2function
	int nPrecedence;    				//���ȼ�
	int nOperandsCount;					//������Ԫ��
	int nAssociativity;					//0,���ϣ�1�ҽ��
	bool bUnary;						//��Ŀ���
	int (CParserExpr::* fpEvaluate)(); 	//������������ 
};


class CParserExpr
{
public:
	CParserExpr()
	{
		InitOperator();
	}
	~CParserExpr() {};
	int ParseExpr(string& pExpr, char* strResult);
	void GetVarIntoMap(const char* strName, const char* strValue, int nType);
	int GetVarValue(string& strValName, tagOperand* pstOperand);
	int DoParseExpr();
	int PushOperator(tagOperator& refOperator);
	int Evaluates(tagOperator& refOperator);
private:
	tagOperator		PLeft, PRight, Divide, Multiply, Add, Subtract, Sentinel;
	string	m_strExpression;
	char* m_pszExp;
	stack<tagOperator>	m_Operators;
	stack<tagOperand>	m_pOperands;
	map<string, tagOperand> m_Map;

	void InitOperator();
	int EvaluateAddExpr();
	int EvaluateSubtractExpr();
	int EvaluateMultiplyExpr();
	int EvaluateDivideExpr();
};






