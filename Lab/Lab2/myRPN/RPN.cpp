#include "RPN.h"


//处理表达式
int CParserExpr::DoParseExpr()
{

	int nRet;
	string strToken;
	tagOperator stOperator;
	tagOperand	stOperand;

	while (1)
	{
		//operator
		switch (*m_pszExp)
		{
		case ' ': //ignore
		case '\t': //ignore
		case '\n': //ignore
		case ','://next argument of param
			break;
		case '(':
			m_Operators.push(PLeft);
			m_pszExp++;
			nRet = DoParseExpr();
			if (*m_pszExp != ')')
				return -1;
			if (nRet != 0)
				return -1;
			break;
		case ')':
		case '\0':
			return Evaluates(Sentinel);
		case '+':
			if (PushOperator(Add))
				return -1;
			break;
		case '-':
			if (PushOperator(Subtract))
				return -1;
			break;
		case '*':
			if (PushOperator(Multiply))
				return -1;
			break;
		case '/':
			if (PushOperator(Divide))
				return -1;
			break;
		default:
			strToken = *m_pszExp;
			int flag = GetVarValue(strToken, &stOperand);
			if (flag == 1)
			{
				stOperand.nType = DATATYPE_STRING;
				stOperand.strName = " ";
				stOperand.strValue = strToken;
			}

			m_pOperands.push(stOperand);
		}
		m_pszExp++;
	}
	return 0;
}




//初始化操作符栈
void CParserExpr::InitOperator()
{
	Add.strOperator = "+";
	Add.nType = 0;
	Add.nPrecedence = 8;
	Add.nOperandsCount = 2;
	Add.nAssociativity = 0;
	Add.bUnary = false;
	Add.fpEvaluate = &CParserExpr::EvaluateAddExpr;

	Subtract.strOperator = "-";
	Subtract.nType = 0;
	Subtract.nPrecedence = 8;
	Subtract.nOperandsCount = 2;
	Subtract.nAssociativity = 0;
	Subtract.bUnary = false;
	Subtract.fpEvaluate = &CParserExpr::EvaluateSubtractExpr;

	Multiply.strOperator = "*";
	Multiply.nType = 0;
	Multiply.nPrecedence = 16;
	Multiply.nOperandsCount = 2;
	Multiply.nAssociativity = 0;
	Multiply.bUnary = false;
	Multiply.fpEvaluate = &CParserExpr::EvaluateMultiplyExpr;

	Divide.strOperator = "/";
	Divide.nType = 0;
	Divide.nPrecedence = 16;
	Divide.nOperandsCount = 2;
	Divide.nAssociativity = 0;
	Divide.bUnary = false;
	Divide.fpEvaluate = &CParserExpr::EvaluateDivideExpr;

	PRight.strOperator = "#";
	PRight.nType = 0;
	PRight.nPrecedence = 64;
	PRight.nOperandsCount = 0;
	PRight.nAssociativity = 0;
	PRight.bUnary = false;
	PRight.fpEvaluate = NULL;

	PLeft.strOperator = "#";
	PLeft.nType = 0;
	PLeft.nPrecedence = 64;
	PLeft.nOperandsCount = 0;
	PLeft.nAssociativity = 0;
	PLeft.bUnary = false;
	PLeft.fpEvaluate = NULL;

	Sentinel.strOperator = "#";
	Sentinel.nType = 1;
	Sentinel.nPrecedence = 0;
	Sentinel.nOperandsCount = 0;
	Sentinel.nAssociativity = 0;
	Sentinel.bUnary = false;
	Sentinel.fpEvaluate = NULL;
}

//求值函数
int CParserExpr::ParseExpr(string& pExpr, char* strResult)
{
	int iRetVal;
	m_pszExp = new char[sizeof(pExpr)];
	strcpy(m_pszExp, pExpr.c_str());
	while (!m_pOperands.empty())
	{
		m_pOperands.pop();
	}
	while (!m_Operators.empty())
	{
		m_Operators.pop();
	}
	iRetVal = DoParseExpr();
	if (iRetVal != 0)
	{
		return -1;
	}
	if (m_pOperands.empty())
	{
		strcpy(strResult, "0");
	}
	else
	{
		strncpy(strResult, m_pOperands.top().strValue.c_str(), RESULT_MAX_LEN - 1);
	}
	return m_pOperands.size();

}


//插入变量，值进入Map
void CParserExpr::GetVarIntoMap(const char* strName, const char* strValue, int nType)
{
	tagOperand stOperand;
	stOperand.strName = strName;
	stOperand.strValue = strValue;
	stOperand.nType = nType;
	m_Map.insert(make_pair(stOperand.strName, stOperand));
}






//获取map容器的value
int CParserExpr::GetVarValue(string& strValName, tagOperand* pstOperand)
{
	map<string, tagOperand>::iterator iteMap;
	iteMap = m_Map.find(strValName);
	if (iteMap != m_Map.end())
	{
		pstOperand->strName = iteMap->second.strName;
		pstOperand->strValue = iteMap->second.strValue;
		pstOperand->nType = iteMap->second.nType;
		return 0;
	}
	return 1;
}




//Push进入栈当中
int CParserExpr::PushOperator(tagOperator& refOperator)
{
	int ret = Evaluates(refOperator);
	if (ret == 0)//操作符入栈前，与栈顶元素比较优先级，并做相应处理
	{
		m_Operators.push(refOperator);
		return 0;
	}
	else if (ret == 1)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}




int CParserExpr::Evaluates(tagOperator& refOperator)
{
	int iRet = 0;
	tagOperator stOperator;
	tagOperand stOperand;

	for (;;)
	{
		if (m_Operators.empty())
		{
			return 0;
		}

		stOperator = m_Operators.top();
		if (!strcmp(stOperator.strOperator.c_str(), "#"))
		{
			if ((!strcmp(refOperator.strOperator.c_str(), "#")) && refOperator.nPrecedence == 0)
			{
				m_Operators.pop();
			}
			return 0;
		}

		if (refOperator.nPrecedence > stOperator.nPrecedence)
		{
			return 0;
		}

		m_Operators.pop();
		switch (stOperator.nType)
		{
		case 0:
			iRet = (this->*stOperator.fpEvaluate)();
			if (iRet)
			{
				return -1;
			}
			break;
		case 1:
			return 0;
		case 2:
			return -1;
		}

	}
}


//加减乘除
int CParserExpr::EvaluateAddExpr()
{
	if (m_pOperands.size() < 2)
	{
		return -1;
	}
	tagOperand stOperand, stOperand1, stOperand2;
	char szTmp[32];
	stOperand1 = m_pOperands.top();
	m_pOperands.pop();
	stOperand2 = m_pOperands.top();
	m_pOperands.pop();
	if (stOperand2.nType == DATATYPE_DOUBLE || stOperand1.nType == DATATYPE_DOUBLE)
	{
		stOperand2.nType = DATATYPE_DOUBLE;
		sprintf(szTmp, "%lf", atof(stOperand2.strValue.c_str()) + atof(stOperand1.strValue.c_str()));
	}
	else
	{
		stOperand2.nType = DATATYPE_INT;
		sprintf(szTmp, "%d", atoi(stOperand2.strValue.c_str()) + atoi(stOperand1.strValue.c_str()));
	}
	stOperand.strValue = szTmp;
	m_pOperands.push(stOperand);

	return 0;
}

int CParserExpr::EvaluateSubtractExpr()
{
	if (m_pOperands.size() < 2)
	{
		return -1;
	}
	tagOperand stOperand, stOperand1, stOperand2;
	char szTmp[32];
	stOperand1 = m_pOperands.top();
	m_pOperands.pop();
	stOperand2 = m_pOperands.top();
	m_pOperands.pop();
	if (stOperand2.nType == DATATYPE_DOUBLE || stOperand1.nType == DATATYPE_DOUBLE)
	{
		stOperand2.nType = DATATYPE_DOUBLE;
		sprintf(szTmp, "%lf", atof(stOperand2.strValue.c_str()) - atof(stOperand1.strValue.c_str()));
	}
	else
	{
		stOperand2.nType = DATATYPE_INT;
		sprintf(szTmp, "%d", atoi(stOperand2.strValue.c_str()) - atoi(stOperand1.strValue.c_str()));
	}
	stOperand.strValue = szTmp;
	m_pOperands.push(stOperand);

	return 0;
}

int CParserExpr::EvaluateMultiplyExpr()
{
	if (m_pOperands.size() < 2)
	{
		return -1;
	}
	tagOperand stOperand, stOperand1, stOperand2;
	char szTmp[32];
	stOperand1 = m_pOperands.top();
	m_pOperands.pop();
	stOperand2 = m_pOperands.top();
	m_pOperands.pop();
	if (stOperand2.nType == DATATYPE_DOUBLE || stOperand1.nType == DATATYPE_DOUBLE)
	{
		stOperand2.nType = DATATYPE_DOUBLE;
		sprintf(szTmp, "%lf", atof(stOperand2.strValue.c_str()) * atof(stOperand1.strValue.c_str()));
	}
	else
	{
		stOperand2.nType = DATATYPE_INT;
		sprintf(szTmp, "%d", atoi(stOperand2.strValue.c_str()) * atoi(stOperand1.strValue.c_str()));
	}
	stOperand.strValue = szTmp;
	m_pOperands.push(stOperand);

	return 0;
}

int CParserExpr::EvaluateDivideExpr()
{
	if (m_pOperands.size() < 2)
	{
		return -1;
	}
	tagOperand stOperand, stOperand1, stOperand2;
	char szTmp[32];
	stOperand1 = m_pOperands.top();
	m_pOperands.pop();
	stOperand2 = m_pOperands.top();
	m_pOperands.pop();
	if (stOperand2.nType == DATATYPE_DOUBLE || stOperand1.nType == DATATYPE_DOUBLE)
	{
		stOperand2.nType = DATATYPE_DOUBLE;
		sprintf(szTmp, "%lf", atof(stOperand2.strValue.c_str()) / atof(stOperand1.strValue.c_str()));
	}
	else
	{
		stOperand2.nType = DATATYPE_INT;
		sprintf(szTmp, "%d", atoi(stOperand2.strValue.c_str()) / atoi(stOperand1.strValue.c_str()));
	}
	stOperand.strValue = szTmp;
	m_pOperands.push(stOperand);

	return 0;
}