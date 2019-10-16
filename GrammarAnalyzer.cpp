#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <string>
#include <algorithm>    
#include "LexicalAnalyzer.h"
#include "GrammarAnalyzer.h"
using namespace std;

ofstream outputFile;
ofstream errorFile;

#define COUPLEOUT { cout << CURRENTKEY << " " << CURRENTSTRING << endl; outputFile << CURRENTKEY << " " << CURRENTSTRING << endl;}
#define PUTTHEWORDOUT(item) { cout << "<" << item << ">" << endl; outputFile << "<" << item << ">" << endl;}
#define GET  if(lexicalAnalyzer.index < lexicalAnalyzer.input.size())  getNext();


void GrammarAnalyzer::getNext()  //调用词法分析器对下一个单词进行分析，已经准备好2个current
{
	lexicalAnalyzer.dealNextChar();
}

void GrammarAnalyzer::save()    //保存现场
{
	indexRecord = lexicalAnalyzer.index;
	currentStringRecord = CURRENTSTRING;
	currentKeyRecord = CURRENTKEY;
}

void GrammarAnalyzer::recover()//恢复现场
{
	lexicalAnalyzer.index = indexRecord;
	CURRENTSTRING = currentStringRecord;
	CURRENTKEY = currentKeyRecord;
}

void GrammarAnalyzer::grammarAnalyze()     //整个类的入口
{
	//＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
	outputFile.open("output.txt", ios::out);
	outputFile.open("error.txt", ios::out);
	GET
	//常量说明
	if (CURRENTKEY == "CONSTTK")
	{
		constantDeclareAnalyze();
		PUTTHEWORDOUT("常量说明");
	}
	//变量说明
	if(variableDeclareAnalyze())
		PUTTHEWORDOUT("变量说明");
	//函数分析 
	while(lexicalAnalyzer.index < lexicalAnalyzer.input.size())
		functionDispatcher();
	PUTTHEWORDOUT("程序")
	outputFile.close();
	errorFile.close();
}

void GrammarAnalyzer::compoundStatementAnalyze()     //复合语句（仅用于三种函数中）
{
	//＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞  
	//复合语句也是不包含{}的
	//这部分和整个程序的入口很相似
	//常量说明
	if (CURRENTKEY == "CONSTTK")
	{
		constantDeclareAnalyze();
		PUTTHEWORDOUT("常量说明");
	}
	//变量说明
	if (variableDeclareAnalyze())
		PUTTHEWORDOUT("变量说明");
	//＜语句列＞
	//S();
	statementColumnAnalyze();
	PUTTHEWORDOUT("语句列")
}

void GrammarAnalyzer::constantDeclareAnalyze()
{
	//＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
	//const＜常量定义＞
	//const已经在上个getNext()中处理好，所以在读一个
	while (CURRENTKEY == "CONSTTK")
	{
		COUPLEOUT   //输出const
		GET
		constantDefineAnalyze();
		PUTTHEWORDOUT("常量定义");
		COUPLEOUT   //输出;
		GET
	}
}

void GrammarAnalyzer::constantDefineAnalyze()
{
	//＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}| char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ }
	//int＜标识符＞＝＜整数＞|＜字符＞ 
	//int已经读入了
	COUPLEOUT   //输出int/char
constantDefineAnalyzeStart:
	GET  //得到一个标识符
	COUPLEOUT   //输出标识符
	GET  //得到一个等号
	COUPLEOUT   //输出等号
	GET  //得到一个整数/字符/+/-
	// +99   -99   99 'A' '+'
	if (CURRENTKEY == "PLUS" or CURRENTKEY == "MINU")
	{
		COUPLEOUT
		GET
	}
	if (CURRENTKEY == "INTCON")
	{
		COUPLEOUT   //输出无符号整数
		PUTTHEWORDOUT("无符号整数")
		PUTTHEWORDOUT("整数")
	}
	else if (CURRENTKEY == "CHARCON")
	{
		COUPLEOUT
	}
	GET  //得到一个,/;（多读的）
	if (CURRENTKEY == "COMMA")
	{
		COUPLEOUT
		goto constantDefineAnalyzeStart;
	}
}

bool GrammarAnalyzer::variableDeclareAnalyze()
{
	//＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
	bool have;
	bool haveOne = false;
	while (true)
	{
		have = false;
		save();
		if (CURRENTKEY == "INTTK" or CURRENTKEY == "CHARTK")
		{
			GET
			if (CURRENTKEY == "IDENFR")
			{
				GET
				//if (CURRENTSTRING == ";" or CURRENTSTRING == "," or CURRENTSTRING == "[")  
				//和有返回值函数区分
				if (CURRENTKEY == "SEMICN" or CURRENTKEY == "COMMA" or CURRENTKEY == "LBRACK")
					have = true;
			}
		}
		recover();
		if (have)
		{
			haveOne = true;
			variableDefineAnalyze();
			PUTTHEWORDOUT("变量定义")
			COUPLEOUT  //输出;
			GET
		}
		else
			break;   //要不然就while(true)死循环了。。。
	}
	return haveOne;
}

void GrammarAnalyzer::variableDefineAnalyze()
{
	//＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )}
	COUPLEOUT  //输出＜类型标识符＞
variableDefineAnalyzeStart:
	GET
	COUPLEOUT  //输出标识符
	GET
	if (CURRENTKEY == "SEMICN")   //这行在 int data;会用到
		return;
	else if (CURRENTKEY == "LBRACK")
	{
		//是int A[7]这种数组的定义
		COUPLEOUT  //输出[
		GET  //读入无符号整数
		COUPLEOUT  //输出INTCON
		PUTTHEWORDOUT("无符号整数")   
		//根据定义，只输出＜无符号整数＞
		GET  //读入]
		COUPLEOUT  //输出]
		GET
		//if (CURRENTSTRING == ";")
		if (CURRENTKEY == "SEMICN")
			return;
		//else if (CURRENTSTRING == ",")
		else if (CURRENTKEY == "COMMA")
		{
			COUPLEOUT  //输出,
			goto variableDefineAnalyzeStart;
		}
	}
	else if (CURRENTKEY == "COMMA")
	{
		COUPLEOUT;  //输出逗号
		goto variableDefineAnalyzeStart;
	}
}

void GrammarAnalyzer::functionDispatcher()
{
	//if (CURRENTSTRING == "int" or CURRENTSTRING == "char")
	if (CURRENTKEY == "INTTK" or CURRENTKEY == "CHARTK")
	{
		//有返回值函数
		returnFunctionDefineAnalyze();
		PUTTHEWORDOUT("有返回值函数定义")
	}
	else
	{
		//以void打头的函数
		save();
		GET
		//if (CURRENTSTRING == "main")
		if (CURRENTKEY == "MAINTK")
		{
			recover();
			mainFunctionDefineAnalyze();
			PUTTHEWORDOUT("主函数")
		}
		else if (CURRENTKEY == "IDENFR")
		{
			recover();
			noReturnFunctionDefineAnalyze();
			PUTTHEWORDOUT("无返回值函数定义")
		}
		else
			return;  
			//这旨在应对main函数之后仍然有空格而进入了functionDispatcher()的情况
			//在这种情况下，GET了但没有recover，所以也多读了一个字符。不会陷入死循环。
	}
}

void GrammarAnalyzer::returnFunctionDefineAnalyze()   
{
	//＜有返回值函数定义＞  ::=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
	COUPLEOUT  //输出int/char
	GET
	COUPLEOUT  //输出标识符
	returnFunctionList.push_back(CURRENTSTRING);      //放入有返回值函数表
	PUTTHEWORDOUT("声明头部")  //声明头部由此一家，别无分店
	GET
	COUPLEOUT  //输出(
	GET        //为参数表处理多读入一个字符
	parameterListAnalyze();
	PUTTHEWORDOUT("参数表")
	COUPLEOUT  //输出)
	GET
	COUPLEOUT  //输出{
	GET
	compoundStatementAnalyze();  //分析函数体内的复合语句
	PUTTHEWORDOUT("复合语句");
	COUPLEOUT  //输出}
	GET        //为接下来处理多读一个字符
}

void GrammarAnalyzer::noReturnFunctionDefineAnalyze()
{
	//＜无返回值函数定义＞  ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
	COUPLEOUT  //输出void
	GET
	COUPLEOUT  //输出标识符
	noReturnFunctionList.push_back(CURRENTSTRING);   //放入无返回值函数表
	GET
	COUPLEOUT  //输出(
	GET        //为参数表处理多读入一个字符 
	parameterListAnalyze();
	PUTTHEWORDOUT("参数表")
	COUPLEOUT  //输出)
	GET
	COUPLEOUT  //输出{
	GET
	compoundStatementAnalyze();  //分析函数体内的block
	PUTTHEWORDOUT("复合语句");
	COUPLEOUT  //输出}
	GET        //为接下来处理多读一个字符
}

void GrammarAnalyzer::mainFunctionDefineAnalyze()
{
	//＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
	COUPLEOUT  //输出void
	GET
	COUPLEOUT  //输出MAINTK
	GET
	COUPLEOUT  //输出(
	GET        
	COUPLEOUT  //输出)
	GET
	COUPLEOUT  //输出{
	GET
	compoundStatementAnalyze();  //分析函数体内的block
	PUTTHEWORDOUT("复合语句");
	COUPLEOUT  //输出}
	GET        //都结束了，没必要多读一个字符
}

void GrammarAnalyzer::parameterListAnalyze()   //参数表
{
	//＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
	//如果参数表为空，那么已经读入的字符就是)，就直接返回好了
	//if (CURRENTSTRING == ")")
	if (CURRENTKEY == "RPARENT")
		return;
	else
	{
parameterListAnalyzeStart:
		COUPLEOUT  //输出CHARTK/INTTK
		GET
		COUPLEOUT  //输出IDENFR
		GET
		//if (CURRENTSTRING == ")")
		if (CURRENTKEY == "RPARENT")
			return;
		else
		{
			//是,
			COUPLEOUT  //输出,
			GET    //读入int/char
			goto parameterListAnalyzeStart;
		}
	}

}

void GrammarAnalyzer::statementColumnAnalyze()       //语句列
{
	//＜语句列＞   :: = ＜语句＞{0, n}    注意语句列是不包含{}的
	//语句列实际上只会在<语句>中以{打头的时候才会被用到
	//拆成一句句语句分析
	//while (CURRENTSTRING != "}")
	while (CURRENTKEY != "RBRACE")
	{
		//S();
		statementAnalyze();
		PUTTHEWORDOUT("语句")
	}
	//退出时，CURR=="}"
}

void GrammarAnalyzer::assignmentStatementAnalyze()      //赋值语句
{
	//＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
	COUPLEOUT
	GET
	//if (CURRENTSTRING == "[")
	if (CURRENTKEY == "LBRACK")
	{
		COUPLEOUT  //[
		GET
		expressionStatementAnalyze();
		PUTTHEWORDOUT("表达式")
		COUPLEOUT  //]
		GET        //=
		COUPLEOUT  //=
		GET
		expressionStatementAnalyze();
		PUTTHEWORDOUT("表达式")
	}
	//else if (CURRENTSTRING == "=")
	else if (CURRENTKEY == "ASSIGN")
	{
		COUPLEOUT  //=
		GET
		expressionStatementAnalyze();
		PUTTHEWORDOUT("表达式")
	}
	else
		exit(0);
}

void GrammarAnalyzer::conditionalStatementAnalyze()     //条件语句
{
	//＜条件语句＞  ::= if '('＜条件＞')'＜语句＞［else＜语句＞］
	COUPLEOUT    //IFTK
	GET          //读入(
	COUPLEOUT    //输出(
	GET			 //提前读入
	conditionAnalyze();
	PUTTHEWORDOUT("条件")
	COUPLEOUT    //输出)
	GET
	statementAnalyze();
	PUTTHEWORDOUT("语句")
	if (CURRENTKEY == "ELSETK")
	{
		COUPLEOUT //输出ELSETK
		GET
		statementAnalyze();
		PUTTHEWORDOUT("语句")
	}
}

void GrammarAnalyzer::loopStatementAnalyze()           //循环语句
{
	//以 do, while, for 打头来区分
	//if (CURRENTSTRING == "do")
	if (CURRENTKEY == "DOTK")
	{
		doWhileLoopAnalyze();
	}
	//else if (CURRENTSTRING == "while")
	else if (CURRENTKEY == "WHILETK")
	{
		whileLoopAnalyze();
	}
	//else if (CURRENTSTRING == "for")
	else if (CURRENTKEY == "FORTK")
	{
		forLoopAnalyze();
	}
}

void GrammarAnalyzer::callNoReturnFunctionAnalyze()     //无返回值函数调用语句
{
	//＜无返回值函数调用语句＞ :: = ＜标识符＞'('＜值参数表＞')'
	COUPLEOUT     //标识符
	GET
	COUPLEOUT     //(
	GET
	valueParameterListAnalyze();
	PUTTHEWORDOUT("值参数表")
	COUPLEOUT     //)
	GET
}

void GrammarAnalyzer::callReturnFunctionAnalyze()        //有返回值函数调用语句
{
	//＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
	COUPLEOUT     //标识符
	GET
	COUPLEOUT     //(
	GET
	valueParameterListAnalyze();
	PUTTHEWORDOUT("值参数表")
	COUPLEOUT     //)
	GET
}

void GrammarAnalyzer::scanfStatementAnalyze()           //读语句
{
	//＜读语句＞    ::=  scanf '('＜标识符＞{,＜标识符＞}')'
	COUPLEOUT  //SCANFTK
	GET
	COUPLEOUT  //(
	GET
	COUPLEOUT  //标识符
	GET
	//while (CURRENTSTRING == ",")
	while (CURRENTKEY == "COMMA")
	{
		COUPLEOUT  //,
		GET
		COUPLEOUT  //标识符
		GET
	}
	COUPLEOUT  //)
	GET
}

void GrammarAnalyzer::printfStatementAnalyze()          //写语句
{
	//＜写语句＞    ::= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')'
	COUPLEOUT     //PRINTFTK
	GET
	COUPLEOUT     //(
	GET
	if (CURRENTKEY == "STRCON")
	{
		COUPLEOUT //STRCON
		PUTTHEWORDOUT("字符串")
		GET
		//if (CURRENTSTRING == ",")
		if (CURRENTKEY == "COMMA")
		{
			//说明逗号后头有表达式
			COUPLEOUT  //先输出逗号
			GET
			expressionStatementAnalyze();
			PUTTHEWORDOUT("表达式")
		}
	}
	else
	{
		expressionStatementAnalyze();
		PUTTHEWORDOUT("表达式")
	}
	COUPLEOUT     //)
	GET
}

void GrammarAnalyzer::returnStatementAnalyze()          //返回语句
{
	//＜返回语句＞   ::=  return['('＜表达式＞')']
	COUPLEOUT    //输出RETURNTK
	GET
	//if (CURRENTSTRING == "(")
	if (CURRENTKEY == "LPARENT")
	{
		COUPLEOUT   //输出(
		GET
		expressionStatementAnalyze();
		PUTTHEWORDOUT("表达式")
		COUPLEOUT   //输出)
		GET
	}
}

void GrammarAnalyzer::expressionStatementAnalyze()     //表达式
{
	//＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}  
	if (CURRENTKEY == "PLUS" or CURRENTKEY == "MINU")   //dead point
	{
		COUPLEOUT  //输出+/-
		GET	       //读入项的首字符
	}
	itemAnalyze();
	PUTTHEWORDOUT("项")
	while (CURRENTKEY == "PLUS" or CURRENTKEY == "MINU")
	{
		COUPLEOUT  //输出+/-号而不是字符
		GET
		itemAnalyze();
		PUTTHEWORDOUT("项")
	}
}

void GrammarAnalyzer::factorStatementAnalyze()          //因子
{
	//＜因子＞    ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
	if (CURRENTKEY == "IDENFR")
	{
		//可能只有一个标识符/是数组元素/是有返回值函数的调用
		save();
		GET
		if (CURRENTKEY == "LBRACK")
		{
			//数组元素   a[7]
			recover();
			COUPLEOUT  //输出标识符（新增)
			GET        //读入[
			COUPLEOUT  //输出[
			GET        //多读入一个字符
			expressionStatementAnalyze();
			PUTTHEWORDOUT("表达式")
			COUPLEOUT  //输出]
			GET        //多读入一个字符
		}
		else if (CURRENTKEY == "LPARENT")
		{ 
			//函数调用   fun1()
			recover();
			callReturnFunctionAnalyze();  //已经读入了函数名，所以不用多读入一位了
			PUTTHEWORDOUT("有返回值函数调用语句")
			//不用多读入一个字符，因为callReturnFunctionAnalyze()已经多读入了
		}
		else
		{
			//只有一个标识符   ele
			recover();
			COUPLEOUT  //输出标识符
			GET
		}
	}
	else if (CURRENTKEY == "INTCON")
	{
		//输出无符号整数
		COUPLEOUT  //输出INTCON
		PUTTHEWORDOUT("无符号整数")
		PUTTHEWORDOUT("整数")
		GET
	}
	else if (CURRENTKEY == "PLUS" or CURRENTKEY == "MINU")
	{
		//输出整数（有符号那种）
		COUPLEOUT  //输出+-
		GET        //读入无符号整数
		COUPLEOUT  //输出INTCON
		PUTTHEWORDOUT("无符号整数")
		PUTTHEWORDOUT("整数")
		GET
	}
	else if (CURRENTKEY == "CHARCON")
	{
		COUPLEOUT  //输出CHARCON
		GET
	}
	else if (CURRENTKEY == "LPARENT")
	{
		COUPLEOUT  //输出(
		GET        //为表达式分析多读一个
		expressionStatementAnalyze();
		PUTTHEWORDOUT("表达式")
		//GET        //不需要再读入)，前面已经读入了 
		COUPLEOUT  //输出)
		GET        //多读一个
	}
	else
	{
		cout << "ERROR IN FACTOR" << endl;
	}
}

void GrammarAnalyzer::itemAnalyze()   //项
{
	//＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
	factorStatementAnalyze();
	PUTTHEWORDOUT("因子");
	//while (CURRENTSTRING == "*" or CURRENTSTRING == "/")  //这里已经在factorStatementAnalyze()里头读好了
	while (CURRENTKEY == "MULT" or CURRENTKEY == "DIV")
	{
		COUPLEOUT     //输出*或/
		GET
		factorStatementAnalyze();
		PUTTHEWORDOUT("因子");
	}
	//else return;
}

void GrammarAnalyzer::stepAnalyze()     //步长
{
	//＜步长＞:: = ＜无符号整数＞    //不输出<整数>
	COUPLEOUT   //无符号整数
	PUTTHEWORDOUT("无符号整数")
	GET
}

void GrammarAnalyzer::valueParameterListAnalyze()        //值参数表
{
	//＜值参数表＞   :: = ＜表达式＞{ ,＜表达式＞ }｜＜空＞
	//因为值参数表只出现在括号中，所以可以用)来判断
	//if (CURRENTSTRING == ")")
	if (CURRENTKEY == "RPARENT")
	{
		//空
		;
	}
	else
	{
		//＜表达式＞打头
		expressionStatementAnalyze();
		PUTTHEWORDOUT("表达式")
		//while (CURRENTSTRING == ",")
		while (CURRENTKEY == "COMMA")
		{
			COUPLEOUT   //,
			GET
			expressionStatementAnalyze();
			PUTTHEWORDOUT("表达式")
		}
		//退出时候CURR == ")"
	}
}

void GrammarAnalyzer::conditionAnalyze()     //条件
{
	//＜条件＞    :: = ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞
	expressionStatementAnalyze();
	PUTTHEWORDOUT("表达式")
	//if (CURRENTSTRING == "<" or CURRENTSTRING == "<=" or CURRENTSTRING == ">"
	//	or CURRENTSTRING == ">=" or CURRENTSTRING == "!=" or CURRENTSTRING == "==")
	if (CURRENTKEY == "LSS" or CURRENTKEY == "LEQ" or CURRENTKEY == "GRE" or 
		CURRENTKEY == "GEQ" or CURRENTKEY == "NEQ" or CURRENTKEY == "EQL" )
	{
		//说明是＜表达式＞＜关系运算符＞＜表达式＞
		COUPLEOUT
		GET
		expressionStatementAnalyze();
		PUTTHEWORDOUT("表达式")
	}
}

void GrammarAnalyzer::statementAnalyze()      //语句
{
	//＜语句＞    ::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}'| ＜有返回值函数调用语句＞; 
	//| ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜＜读语句＞; ｜＜写语句＞; ｜＜空＞; | ＜返回语句＞;
	if (CURRENTKEY == "IDENFR")
	{
		//可能是赋值语句/有/无返回值函数调用语句
		save();
		GET
		//if (CURRENTSTRING == "[")
		if (CURRENTKEY == "LBRACK")
		{
			//＜赋值语句＞;
			recover();
			//此时已经多读了一个字符了
			assignmentStatementAnalyze();
			//多读了字符;
			PUTTHEWORDOUT("赋值语句")
			COUPLEOUT  //输出;
			GET
		}
		//else if (CURRENTSTRING == "=")
		else if (CURRENTKEY == "ASSIGN")
		{
			//＜赋值语句＞;
			recover();
			//此时已经多读了一个字符了
			assignmentStatementAnalyze();
			//多读了字符;
			PUTTHEWORDOUT("赋值语句")
			COUPLEOUT  //输出;
			GET
		}
		else
		{
			//有/无返回值函数调用语句
			recover();
			vector<string>::iterator iterOfReturn;  //find用法参见此处
			iterOfReturn = find(returnFunctionList.begin(), returnFunctionList.end(), CURRENTSTRING);
			if (iterOfReturn != returnFunctionList.end())
			{
				//＜有返回值函数调用语句＞; 
				//此时已经多读了一个字符了
				callReturnFunctionAnalyze();
				PUTTHEWORDOUT("有返回值函数调用语句")
				COUPLEOUT  //输出;
				GET
			}
			else
			{
				//只能是＜无返回值函数调用语句＞; 
				callNoReturnFunctionAnalyze();
				PUTTHEWORDOUT("无返回值函数调用语句")
				COUPLEOUT  //输出;
				GET
			}
		}
	}   //if (CURRENTKEY == "IDENFR")
	else if (CURRENTKEY == "FORTK")
	{
		//＜循环语句＞
		forLoopAnalyze();
		PUTTHEWORDOUT("循环语句")
	}
	else if (CURRENTKEY == "WHILETK")
	{
		//＜循环语句＞
		whileLoopAnalyze();
		PUTTHEWORDOUT("循环语句")
	}
	else if (CURRENTKEY == "DOTK")
	{
		//＜循环语句＞
		doWhileLoopAnalyze();
		PUTTHEWORDOUT("循环语句")		
	}
	else if (CURRENTKEY == "IFTK")
	{
		//＜条件语句＞
		conditionalStatementAnalyze();
		PUTTHEWORDOUT("条件语句")
	}
	else if (CURRENTKEY == "LBRACE")
	{
		//'{'＜语句列＞'}'
		COUPLEOUT   //输出{
		GET
			//S();
		statementColumnAnalyze();
		PUTTHEWORDOUT("语句列")
		COUPLEOUT   //输出}
		GET
	}
	else if (CURRENTKEY == "PRINTFTK")
	{
		//＜写语句＞;
		printfStatementAnalyze();
		PUTTHEWORDOUT("写语句")
		COUPLEOUT  //输出;
		GET
	}
	else if (CURRENTKEY == "SCANFTK")
	{
		//＜读语句＞;
		scanfStatementAnalyze();
		PUTTHEWORDOUT("读语句")
		COUPLEOUT  //输出;
		GET
	}
	else if (CURRENTKEY == "SEMICN")
	{
		//＜空＞;
		COUPLEOUT  //输出;
		GET
	}
	else if (CURRENTKEY == "RETURNTK")
	{
		//＜返回语句＞;
		returnStatementAnalyze();
		PUTTHEWORDOUT("返回语句")
		COUPLEOUT  //输出;x
		GET
	}
	else
	{
		PUTTHEWORDOUT("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
		exit(0);
	}
}

void GrammarAnalyzer::doWhileLoopAnalyze()              //do while
{
	//do＜语句＞while '('＜条件＞')'    
	//这里和C语法不同，不包括;
	COUPLEOUT    //do
	GET
	statementAnalyze();
	PUTTHEWORDOUT("语句")
	COUPLEOUT    //while
	GET
	COUPLEOUT    //(
	GET
	conditionAnalyze();
	PUTTHEWORDOUT("条件")
	COUPLEOUT    //)
	GET
}

void GrammarAnalyzer::forLoopAnalyze()					 //for 
{
	//for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
	COUPLEOUT    //FORTK
	GET
	COUPLEOUT    //(
	GET
	COUPLEOUT    //标识符
	GET
	COUPLEOUT    //=
	GET
	expressionStatementAnalyze();
	PUTTHEWORDOUT("表达式")
	COUPLEOUT    //;
	GET
	conditionAnalyze();
	PUTTHEWORDOUT("条件")
	COUPLEOUT    //;
	GET
	COUPLEOUT    //标识符
	GET
	COUPLEOUT    //=
	GET
	COUPLEOUT    //标识符
	GET
	COUPLEOUT    //+/-  必须有
	GET
	stepAnalyze();
	PUTTHEWORDOUT("步长")
	COUPLEOUT    //)
	GET
	statementAnalyze();
	PUTTHEWORDOUT("语句")
}

void GrammarAnalyzer::whileLoopAnalyze()			     //while
{
	//while '('＜条件＞')'＜语句＞
	COUPLEOUT    //WHILETK
	GET
	COUPLEOUT    //(
	GET
	conditionAnalyze();
	PUTTHEWORDOUT("条件")
	COUPLEOUT    //)
	GET
	statementAnalyze();
	PUTTHEWORDOUT("语句")
}
