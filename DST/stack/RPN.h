#include  "Stack.h"
#include "priority.h"
#include <cstring>
#include "cal.h"

//递归实现进制转化
// void convert(Stack<char>&S ,  __int64 n,int base){//10进制转化为base进制
// static char digit[] //base小于等于16，大于0
// ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
// if(0<n){
//     S.push(digit[n%base]);
//     convert(S,n/base,base);//递归等到高位
// }
// }


//迭代版本
 void convert(Stack<char>&S ,  __int64 n, int base)//10进制转化为base进制
 {
     static char digit[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
     while(n>0)
     {
         int remainder=(int)(n%base);
         S.push(digit[remainder]);
         n/=base;
     }
 }


//递归嵌套表达式
 //括号匹配问题
bool paren(const char exp[],int lo,int hi)
{
    Stack<char> S;
    for(int i=lo;i<=hi;i++)
        switch(exp[i])
        {
        case '(':case '[':case '{':S.push(exp[i]);break;
        case ')':if((S.empty())||(('('!=S.pop())))return false;break;
        case ']':if((S.empty())||(('['!=S.pop())))return false;break;
        case '}':if((S.empty())||(('{'!=S.pop())))return false;break;
        default:break;
        }
    return S.empty();//最后看一下栈是不是空的，空的才说明成功
}



float  evaluate(char * S,char * & RPN)//对表达式s求值，并且转换为RPN
{
    char* expr=S;
    Stack<float> opnd;//运算数栈
    Stack<char> optr;//运算符栈
    optr.push('\0');//末尾的哨兵率先入队
    while(!optr.empty())//运算栈非空的时候，逐个处理表达式中的字符
    {
        if(isdigit(*S)){
            readNumber(S,opnd);
            append(RPN,opnd.top());//读入操作数街道RPN的尾部
        }
        else//当前为运算符
        switch(orderBetween(optr.top(),*S))
        {
            case '<':
                optr.push(*S);S++;
                break;
            case '=':
                optr.pop();S++;
                break;
            case '>':{
                char op=optr.pop();
                append(RPN,op);//栈顶运算符出栈接入RPN末尾
                if('!'==op)
                {
                    float pOpnd=opnd.pop();//只需取出一个操作室
                    opnd.push(calcu(op,pOpnd));//一元运算
                }
                else
                {
                    float pOpnd2=opnd.pop();
                    float pOpnd1=opnd.pop();
                    opnd.push(calcu(pOpnd1,op,pOpnd2));
                }
                break;
            }
            default:exit(-1);
        }//switch
        displayProgress ( expr, S, opnd, optr, RPN );
    }//while
    return opnd.pop();
}

