#include <iostream>
#include "RPN.h"


int main()
{
    const char * s="1+6/2";
    char * k;
    strcpy(k,s);
    char* rpn = ( char* ) malloc ( sizeof ( char ) * 1 );   rpn[0] = '\0'; //逆波兰表达式
    evaluate(k,rpn);
    return 0;
}