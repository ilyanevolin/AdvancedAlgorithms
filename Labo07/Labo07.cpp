// Labo07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "knuthmorrispratt.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void demo1() {
	string P = "abbabab";// anaanbs
	KnuthMorrisPratt kmp((unsigned char*)P.c_str(), P.length());
	string T = "ABC ABCXABCDAB"; // anaanaanbs

	std::queue<const unsigned int> q;
	kmp.zoek(q, (unsigned char*)T.c_str(), T.length()); 
	cout <<endl<< "found " << q.size() << endl;
	//perf should be equal to length of string T (24 == 24) YES! :)
	while (!q.empty()) {
		unsigned int pos = q.front();
		q.pop();
		cout << pos << ": ";
		for (int i = 0; i < P.length(); i++)
			cout << T[pos+i];
		cout << endl;
	}
	
	cout << endl << endl;
}
void opl() {

	std::ifstream infile("Shakespeare.txt");	
	std::string s, ss;
	int lines = 0;
	while (std::getline(infile, s)) {		
		ss += s;
		++lines;
	}
	cout << "lines: " << lines << endl;

	string p= "";
	//cout << "geef een woord:" << endl; std::getline(cin, p);
	p = "the"; //case sensitive!
	KnuthMorrisPratt kmp((unsigned char*)p.c_str(),p.length());
	std::queue<const unsigned int> q;

	kmp.zoek(q, (unsigned char*)ss.c_str(), ss.length());
	cout <<endl<< "found " << q.size() << endl;

	/*
	while (!q.empty()) {
		unsigned int pos = q.front();
		q.pop();
		cout << pos << ": '";
		for (int i = 0; i < p.length(); i++)
			cout << ss[pos+i];
		cout << "'"<< endl;
	}
	*/
	cout << endl;
}
int _tmain(int argc, _TCHAR* argv[]) {
	demo1();
	//opl();
	return 0;
}

