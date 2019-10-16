#pragma once
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "LexicalAnalyzer.h"
using namespace std;

#define CURRENTSTRING  lexicalAnalyzer.currentString
#define CURRENTKEY     lexicalAnalyzer.currentKey


class GrammarAnalyzer
{
public:
	GrammarAnalyzer():lexicalAnalyzer() {};
	void grammarAnalyze();   //处理的入口
	
//private:
	LexicalAnalyzer lexicalAnalyzer;
	int indexRecord;
	string currentStringRecord;
	string currentKeyRecord;       
	vector <string> returnFunctionList;      //有返回值函数表
	vector <string> noReturnFunctionList;    //无返回值函数表
	vector <string> variableList;
	vector <string> constantList;

	void getNext();                          //调用词法分析器对下一个单词进行分析，已经准备好2个current
	void save();                             //保存现场
	void recover();                          //恢复现场
	void functionDispatcher();			     //函数分配器

	void compoundStatementAnalyze();         //复合语句::=[＜常量说明＞］［＜变量说明＞］＜语句列＞
	void constantDeclareAnalyze();           //常量说明
	void constantDefineAnalyze();            //常量定义
	bool variableDeclareAnalyze();           //变量说明
	void variableDefineAnalyze();	         //变量定义
	void returnFunctionDefineAnalyze();	     //有返回值函数定义
	void noReturnFunctionDefineAnalyze();    //无返回值函数定义
	void mainFunctionDefineAnalyze();        //主函数
	void parameterListAnalyze();             //参数表
	void statementColumnAnalyze();           //语句列
	void assignmentStatementAnalyze();       //赋值语句
	void conditionalStatementAnalyze();      //条件语句
	void loopStatementAnalyze();             //循环语句
	void callNoReturnFunctionAnalyze();      //无返回值函数调用语句
	void callReturnFunctionAnalyze();        //有返回值函数调用语句
	void scanfStatementAnalyze();            //读语句
	void printfStatementAnalyze();           //写语句
	void returnStatementAnalyze();           //返回语句
	void expressionStatementAnalyze();       //表达式
	void factorStatementAnalyze();           //因子
	void itemAnalyze();                      //项
	void stepAnalyze();                      //步长
	void valueParameterListAnalyze();        //值参数表
	void conditionAnalyze();                 //条件
	void statementAnalyze();                 //语句
	void doWhileLoopAnalyze();               //do while
	void forLoopAnalyze();					 //for 
	void whileLoopAnalyze();			     //while
};


