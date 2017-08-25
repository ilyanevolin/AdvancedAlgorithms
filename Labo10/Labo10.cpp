// Labo10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sais.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void t1() {
	ifstream infile;
	infile.open("demo.txt");
	string line="", T="";
	while (std::getline(infile, line)) {
		T.append( line + "\n" );
	}
	T.erase(0,3); //T contains unreadable invalid characters!!?
	unsigned char * Tp = (unsigned char *)T.c_str();
	SAIS s(Tp, T.length());
	string pat = "aa";
	int range_l, range_r;
	 s.findPatternRange((char*)Tp, T.length(), pat.c_str(), pat.length(), range_l,range_r);
	cout << "T contains '"<< pat <<"'?  => "<< (range_l != -1 );
	cout << endl;
	infile.close();
}
int _tmain(int argc, _TCHAR* argv[])
{

	
	ifstream infile;
	infile.open("demo.txt");
	string line="", T="";
	while (std::getline(infile, line)) {
		T.append( line + "\n" );
	}
	T.erase(0,3); //T contains unreadable invalid characters!!?
	unsigned char * Tp = (unsigned char *)T.c_str();
	SAIS s(Tp, T.length());
	s.replaceMagic((char*)Tp, T.length());
	/*string pat = "of";
	int range_l, range_r;
	s.findPatternRange(T.c_str(), T.length(), pat.c_str(), pat.length(), range_l, range_r);*/
	return 0;
}

