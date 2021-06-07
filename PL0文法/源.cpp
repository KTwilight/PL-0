#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
using namespace std;

int main() 
{
	string filename;//文件名 
	//cin >> filename;//输入要处理的文件名 
	filename = "test.txt";
	fstream file(filename);//创建一个fstream文件流对象
	string line;
	string content;
	int i = 0;
	while (getline(file, line)) {
		content += line;
		content += '\n';
	}
	cout << content << endl;
	LexicalAnalyzer lexicalAnalyzer;
	lexicalAnalyzer.analyze(content.c_str());
	cout << "word" << '\t' << "row" << '\t' << "col" << '\t' << "type" << endl;
	vector<Symbol> symbols = lexicalAnalyzer.getSymbols();
	for (auto symbol : symbols)
	{
		cout << symbol.word << "\t" << symbol.beg.row << "\t" << symbol.beg.col << "\t" << symbol.symbolType << endl;
	}
	
	SyntaxAnalyzer syntaxAnalyzer;
	syntaxAnalyzer.analyze(symbols);
	
}