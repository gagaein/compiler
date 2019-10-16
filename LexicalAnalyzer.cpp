#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "LexicalAnalyzer.h"	
using namespace std;

//#define CHINOUT(item) {                                     \
//    cout << "<" << item << ">" << endl;        \
//    lexicalAnalyzer.fileDealer.outputFile << "<" << item << ">" << endl;  \
//}


void LexicalAnalyzer::getInput()
{
	ifstream fpi("testfile.txt");  //建立一个输入流对象和文件关联  只有一个参数  默认情况下后面为ios::in
	char ch;
	while (fpi.get(ch))
	{
		input.push_back(ch);
	}
	fpi.close();
}

bool LexicalAnalyzer::isFirstOfMark(char ch)
{
	if ((ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') 
		|| (ch == '<') || (ch == '>') || (ch == '=') || (ch == '!')
		|| (ch == ';') || (ch == ',') || (ch == '(') || (ch == ')')
		|| (ch == '[') || (ch == ']') || (ch == '{') || (ch == '}'))
		return true;
	else
		return false;
}

bool LexicalAnalyzer::isBlank(char ch)
{
	//把各种换行符啥的都换成空格
	if (isspace(ch))
		return true;
	else
		return false;
}

string LexicalAnalyzer::readFileIntoString(const char* filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}

char LexicalAnalyzer::getNextChar()
{
	if (index < input.size())
		return input[index++];
	else
		return -1;
}

void LexicalAnalyzer::retreat()
{
	index--;
}

//void LexicalAnalyzer::printOutput()
//{
//	FILE* fp;
//	if ((fp = fopen("output.txt", "w")) == NULL)
//	{
//		printf("file cannot be opened\n");
//		exit(0);
//	}
//	else
//	{
//		//string st = output.substr(0, output.size() - 1);
//		fprintf(fp, output.c_str());
//	}
//	//cout << "job done\n" << endl;
//	fclose(fp);
//}

bool LexicalAnalyzer::isNumber(char ch)
{
	if (ch >= '0' && ch <= '9')
		return true;
	else
		return false;
}

bool LexicalAnalyzer::isPartOfIDENFR(char ch)  //include '_' 
{
	if (ch >= 'A' && ch <= 'Z')
		return true;
	else if (ch >= 'a' && ch <= 'z')
		return true;
	else if (ch == '_')
		return true;
	else if (ch >= '0' && ch <= '9')
		return true;
	else
		return false;
}

bool LexicalAnalyzer::isLetter(char ch)  //include '_' !!!!!!
{
	if (ch >= 'A' && ch <= 'Z')
		return true;
	else if (ch >= 'a' && ch <= 'z')
		return true;
	else if (ch == '_')
		return true;
	else
		return false;
}

int LexicalAnalyzer::judgeFirstChar(char ch)
{
	if (isLetter(ch))
		return 1;
	if (isNumber(ch))
		return 2;
	if (isFirstOfMark(ch))
	{
		//ch 是markMap中的一个元素
		//if (ch == '+' or ch == '-')
		//{
		//	char nextChar = getNextChar();
		//	if (isNumber(nextChar) == false)
		//	{
		//		retreat();
		//		return 3;  //not a int number
		//	}
		//	else
		//	{
		//		retreat();
		//		return 6;
		//	}
		//}
		//else
			return 3;
	}
	if (ch == '\"')
		return 4;
	if (ch == '\'')
		return 5;
	cout << "error in judgeFirstChar" << endl;
	exit(0);
}

bool LexicalAnalyzer::identitier(string s)
{
	map<string, string>::iterator iter;
	iter = ReversedWordsMap.find(s);
	if (iter != ReversedWordsMap.end())
	{
		//还要检测接下来一个字符如果加上，能不能再构成标识符
		char nextChar = getNextChar();
		if (!isPartOfIDENFR(nextChar))
		{
			currentString = string(s);
			currentKey = iter->second;
			retreat();
			return true;
		}
		else  //如果再加上一个字还是能构成标识符，就下次再算，这次返回false
		{
			retreat();
			return false;
		}
	}
	else //iter==end 说明是保留字
		return false;
}

void LexicalAnalyzer::judge1(char firstChar)
{
	string s = string(1, firstChar);
	while (true)
	{
		if (identitier(s))
		{
			//s是一个保留字
			//不需要退出并维护2c
			break;
		}
		else
		{
			char nextChar = getNextChar();
			if (isPartOfIDENFR(nextChar))
				s.append(1, nextChar);
			else
			{
				//读入的不再是标识符或者保留字的一部分，
				//就应该退出并维护2c
				retreat();
				currentString = s;
				currentKey = string("IDENFR");
				break;
			}
		}
	}
	//COUPLEOUT;
}

void LexicalAnalyzer::judge2(char firstChar)
{
	string s(1, firstChar);
	while (true)
	{
		char nextChar = getNextChar();
		if (isNumber(nextChar) == false)
		{
			retreat();
			currentString = s;
			currentKey = string("INTCON");
			break;
		}
		else
		{
			s.append(string(1, nextChar));
		}
	}
	//COUPLEOUT;
	//STATEMENTOUT("无符号整数");
	//STATEMENTOUT("整数");
}

void LexicalAnalyzer::judge3(char firstChar)
{
	string s(1, firstChar);
	//先检查是不是二元，再检查是不是一元
	if (index < input.size())
	{
		char nextChar = getNextChar();
		s.append(string(1, nextChar));
		map<string, string>::iterator iter = markMap.find(s);
		if (iter != markMap.end())
		{
			//是二元
			currentString = s;
			currentKey = iter->second;
		}
		else
		{
			retreat();
			string ss(1, firstChar);
			//是一元
			map<string, string>::iterator iter = markMap.find(ss);
			if (iter != markMap.end())
			{
				currentString = ss;
				currentKey = iter->second;
			}
			else
				exit(0);
		}
	}
	else
	{
		//到末尾了，就只能是单个了
		map<string, string>::iterator iter = markMap.find(s);
		if (iter != markMap.end())
		{
			currentString = s;
			currentKey = iter->second;
		}
		else
			exit(0);
	}
	//COUPLEOUT;
}

void LexicalAnalyzer::judge4(char firstChar)
{
	string s;
	char nextChar;
	while ((nextChar = getNextChar()) != '"')
	{
		s.append(1, nextChar);
	}
	currentString = s;
	currentKey = string("STRCON");
	//COUPLEOUT;
}

void LexicalAnalyzer::judge5(char firstChar)
{
	char nextChar = getNextChar();
	if ((nextChar >= '0' && nextChar <= '9') or
		isLetter(nextChar) or
		(nextChar == '+') or
		(nextChar == '-') or
		(nextChar == '*') or
		(nextChar == '/')  )
	{
		currentString = string(1, nextChar);
		currentKey = string("CHARCON");
		getNextChar();
	}
	else
		exit(0);
	//COUPLEOUT;
}

//void LexicalAnalyzer::judge6(char firstchar)
//{
//	string s(1, firstchar);  //s = "+" or "-"
//	while (true)
//	{
//		char nextchar = getNextChar();
//		if (isNumber(nextchar) == false)
//		{
//			retreat();
//			currentString = s;
//			currentKey = string("intcon");  //和judeg一样，所以只能用CS的首字母区分
//			break;
//		}
//		else
//		{
//			s.append(string(1, nextchar));
//		}
//	}
//	//COUPLEOUT;
//	//STATEMENTOUT("整数");
//}

void LexicalAnalyzer::initialize()
{
	currentKey = string();
	currentString = string();
}

//void LexicalAnalyzer::transferIntoOutput()
//{
//	if (currentString.size() > 0)
//	{
//		output.append(currentKey);
//		output.append(" ");
//		output.append(currentString);
//		output.append("\n");
//	}
//}

//void LexicalAnalyzer::LexicalDealer()
//{
//	for (; index < input.size();)
//	{
//		initialize();
//		char firstChar = getNextChar();
//		if (isBlank(firstChar))
//			continue;
//		int x = judgeFirstChar(firstChar);
//		if (x == 1)
//			judge1(firstChar);
//		if (x == 2) 
//			judge2(firstChar);
//		if (x == 3) 
//			judge3(firstChar);
//		if (x == 4) 
//			judge4(firstChar);
//		if (x == 5) 
//			judge5(firstChar);
//		//if (x == 6)
//		//	judge6(firstChar);
//		transferIntoOutput();
//	} //for
//}


//void LexicalAnalyzer::lexicalAanalyse()
//{
//	getInput();
//	LexicalDealer();
//	printOutput();  //should be changed later
//}


void LexicalAnalyzer::dealNextChar()
{
start:
	initialize();
	char firstChar = getNextChar();
	if (firstChar == -1)
	{
		currentKey = "END";
		return;
	}
	while (isBlank(firstChar))
		goto start;
	int x = judgeFirstChar(firstChar);
	if (x == 1)
		judge1(firstChar);
	else if (x == 2)
		judge2(firstChar);
	else if (x == 3)
		judge3(firstChar);
	else if (x == 4)
		judge4(firstChar);
	else if (x == 5)
		judge5(firstChar);
	//else
		//exit(0);
	//getNextChar();  //多读一个字符
}