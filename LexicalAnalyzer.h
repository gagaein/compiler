#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iostream>
using namespace std;



class LexicalAnalyzer
{
public:
	LexicalAnalyzer()
	{ 
		getInput();
	};
	void dealNextChar();  //读入一个或多个字符，维护两个current之后返回
	int index = 0;  //标识即将去读取的字符的位置
	vector<char> input;   //从testfile.txt中读取到的字符集合
	//string output;   //写入reslut.txt中的内容
	string currentString;  //当前分析的字符串
	string currentKey;  //当前分析得出的字符串的类型
	void retreat();  //index--

//private:
	map<string, string> ReversedWordsMap
	{
		make_pair(string("const"),string("CONSTTK")),
		make_pair(string("int"),string("INTTK")),
		make_pair(string("char"),string("CHARTK")),
		make_pair(string("void"),string("VOIDTK")),
		make_pair(string("main"),string("MAINTK")),
		make_pair(string("if"),string("IFTK")),
		make_pair(string("else"),string("ELSETK")),
		make_pair(string("do"),string("DOTK")),
		make_pair(string("while"),string("WHILETK")),
		make_pair(string("for"),string("FORTK")),
		make_pair(string("scanf"),string("SCANFTK")),
		make_pair(string("printf"),string("PRINTFTK")),
		make_pair(string("return"),string("RETURNTK"))
	};
	map<string, string> markMap
	{
		make_pair(string("+"),string("PLUS")),
		make_pair(string("-"),string("MINU")),
		make_pair(string("*"),string("MULT")),
		make_pair(string("/"),string("DIV")),
		make_pair(string("<"),string("LSS")),
		make_pair(string("<="),string("LEQ")),
		make_pair(string(">"),string("GRE")),
		make_pair(string(">="),string("GEQ")),
		make_pair(string("=="),string("EQL")),
		make_pair(string("!="),string("NEQ")),
		make_pair(string("="),string("ASSIGN")),
		make_pair(string(";"),string("SEMICN")),
		make_pair(string(","),string("COMMA")),
		make_pair(string("("),string("LPARENT")),
		make_pair(string(")"),string("RPARENT")),
		make_pair(string("["),string("LBRACK")),
		make_pair(string("]"),string("RBRACK")),
		make_pair(string("{"),string("LBRACE")),
		make_pair(string("}"),string("RBRACE")),
	};

	bool isFirstOfMark(char);
	bool isBlank(char);
	string readFileIntoString(const char*);  //没有用途
	char getNextChar();  //getnextchar
	void getInput();  //读入文件
	//void printOutput();  //输出文件
	bool isNumber(char);  
	bool isPartOfIDENFR(char);  //看能不能组成标识符的一部分（不考虑首字符）
	bool isLetter(char);   //include '_' !
	int judgeFirstChar(char);  
	bool identitier(string);  //看string是不是标识符
	void judge1(char);  //字母include '_' !
	void judge2(char);  //数字
	void judge3(char);  //是各种markMap中的符号的首字母
	void judge4(char);  //是双引号
	void judge5(char);  //是单引号
	//void judge6(char);  //s = "+" or "-" 对付带了符号的数字
	void initialize();
	//void transferIntoOutput();  //对两个current改造成字符串，并放入output
	//void LexicalDealer();  //除掉input和output之外的所有工作都是这个函数管的
};



