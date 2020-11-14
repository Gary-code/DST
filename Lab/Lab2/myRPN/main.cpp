#include "RPN.h"

int main()
{
	CParserExpr Test;
	Test.GetVarIntoMap("a", "64", 1);
	Test.GetVarIntoMap("b", "4", 1);
	Test.GetVarIntoMap("c", "16", 1);
	string test1 = "a+b*2-c/(a-c)";
	char* result = new char[999];
	Test.ParseExpr(test1, result);
	cout << result;
}

