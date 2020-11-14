#include <cstring>
#include "priority.h"
#include <iostream>
#include <cmath>
#include "Stack.h"
#include "UniPrint.h"
int facI(int b){
    if(b==0||b==1) return 1;
    else return b*facI(b-1); 
}

double calcu ( double a, char op, double b ) { //执行二元运算
   switch ( op ) {
      case '+' : return a + b;
      case '-' : return a - b;
      case '*' : return a * b;
      case '/' : if ( 0==b ) exit ( -1 ); return a/b; //注意：如此判浮点数为零可能不安全
      case '^' : return pow ( a, b );
      default  : exit ( -1 );
   }
}

double calcu ( char op, double b ) { //执行一元运算
    switch (op)
    {
    case '!':
    return ( double ) facI ( ( int ) b );
        break;
    
    default:exit(-1);
    }
}


//起始于p的子串解析为数值，并存入操作数栈当中
void readNumber(char * &p,Stack<float> & stk)
{
    stk.push((float)(*p-'0'));//当前数位的数值入栈
    while(isdigit(*(++p)))//多位整数时候
    {
        stk.push(stk.pop()*10+(*p-'0'));
    }
    if('.'!=*p)return ;//没有小数点
    float fraction=1;//否则，意味着还有小数
    while(isdigit(*(++p)))
    {
        stk.push(stk.pop()+(*p-'0')*(fraction/=10));//小数部分加进去
    }
}

//比较优先级
 Operator optr2rank(char op)//运算符转译出编号
{
    switch(op)
    {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;
        default: exit(-1);//未知运算符
    }
}
char orderBetween(char op1,char op2)
{
    return pri[optr2rank(op1)][optr2rank(op2)];
}
//将操作数接连到RPN的尾部
void append(char * & rpn,float opnd)
{
    int n=strlen(rpn);//当前RPN的长度，以\0结尾长度为n+1
    char buf[64];
    if(opnd!=(float)(int)opnd)sprintf(buf,"%.2f \0",opnd);//浮点格式
    else sprintf(buf,"%d \0",opnd);//整数格式
    rpn=(char *)realloc(rpn,sizeof(char) *(n+strlen(buf)+1));//扩展空间
    strcat(rpn,buf);//rpn加长
}

//运算符接入rpn末尾
void append(char * & rpn,char optr)
{
    int n=strlen(rpn);
    rpn=(char *)realloc(rpn,sizeof(char) * (n+3));//扩展空间
    sprintf(rpn+n,"%c",optr);rpn[n+2]='\0';//接入指定的运算符
}
template <typename T> static void print ( T* x ) {  x ? print ( *x ) : printf ( " <NULL>" );  }
template <typename T> static void print ( T& x ) {  UniPrint::p ( x );  }
template <typename T> static void print ( const T& x ) {  UniPrint::p ( x );  } //for Stack
static void print ( char* x ) {  printf ( " %s", x ? x : "<NULL>" );  } //字符串特别处理
static void print ( const char* x ) {  printf ( " %s", x ? x : "<NULL>" );  } //字符串特别处理


void displayProgress ( char* expr, char* pCh, Stack<float>& opndStk, Stack<char>& optrStk, char* rpn ) {
   system ( "cls" );
   printf ( "\n" );
   for ( char* p = expr; '\0' != *p; p++ ) printf ( " %c", *p ); printf ( " $\n" );
   for ( char* p = expr; p < pCh; p++ ) printf ( "  " );
   if ( '\0' != * ( pCh - 1 ) )
      { for ( char* p = pCh; '\0' != *p; p++ ) printf ( " %c", *p ); printf ( " $" ); }
   printf ( "\n" );
   for ( char* p = expr; p < pCh; p++ ) printf ( "--" ); printf ( " ^\n\n" );
   print ( optrStk ); printf ( "\n" );
   print ( opndStk ); printf ( "\n" );
   printf ( "RPN:\n %s\n\n", rpn ); //输出RPN
   getchar();
}