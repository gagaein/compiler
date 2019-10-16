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
	void dealNextChar();  //����һ�������ַ���ά������current֮�󷵻�
	int index = 0;  //��ʶ����ȥ��ȡ���ַ���λ��
	vector<char> input;   //��testfile.txt�ж�ȡ�����ַ�����
	//string output;   //д��reslut.txt�е�����
	string currentString;  //��ǰ�������ַ���
	string currentKey;  //��ǰ�����ó����ַ���������
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
	string readFileIntoString(const char*);  //û����;
	char getNextChar();  //getnextchar
	void getInput();  //�����ļ�
	//void printOutput();  //����ļ�
	bool isNumber(char);  
	bool isPartOfIDENFR(char);  //���ܲ�����ɱ�ʶ����һ���֣����������ַ���
	bool isLetter(char);   //include '_' !
	int judgeFirstChar(char);  
	bool identitier(string);  //��string�ǲ��Ǳ�ʶ��
	void judge1(char);  //��ĸinclude '_' !
	void judge2(char);  //����
	void judge3(char);  //�Ǹ���markMap�еķ��ŵ�����ĸ
	void judge4(char);  //��˫����
	void judge5(char);  //�ǵ�����
	//void judge6(char);  //s = "+" or "-" �Ը����˷��ŵ�����
	void initialize();
	//void transferIntoOutput();  //������current������ַ�����������output
	//void LexicalDealer();  //����input��output֮������й���������������ܵ�
};



