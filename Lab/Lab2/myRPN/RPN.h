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


//操作数
struct tagOperand
{
	int nType;         //操作数类型 
	string strName;    //名称 
	string strValue;   //值 
};


//初始化集合
void initset()//set容器初始化 
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


//操作符
struct tagOperator
{
	string strOperator; 				//操作符名称
	int nType;          				//0,operater;1sential;2function
	int nPrecedence;    				//优先级
	int nOperandsCount;					//操作数元数
	int nAssociativity;					//0,左结合；1右结合
	bool bUnary;						//单目算符
	int (CParserExpr::* fpEvaluate)(); 	//操作符处理函数 
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






