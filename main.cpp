#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "LexicalAnalyzer.h"
#include "GrammarAnalyzer.h"
using namespace std;


int main(int argc, char* argv[])	
{
	GrammarAnalyzer grammarAnalyzer;
	grammarAnalyzer.grammarAnalyze();	
	return 0;
}