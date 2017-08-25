// Labo08.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "regexp11.h"
#include "symbool.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <stack>
#include <map>
using namespace std;

// om duplicates te vermeiden gaan we map gebruiken (later) -->dynamisch progr.
// duplicates wel toegelaten, gewoon recursief

void process_with_dups(const Regexp& re, Symbool& sy, stack<string>& st) {
	string s ="";	
	if (re.geefOpcode() == Regexp::letter) {			
		s = "<"+sy+">" + " := " + (char)re.geefLetter();
		st.push(s);
	} else {
		s = "<"+sy+">" + " := ";
		sy++;	
		s += "<"+sy+">";
		process_with_dups(*re.geefEersteOperand(), sy, st);		
		switch (re.geefOpcode()) {
			case Regexp::ster:
				s += "*";
				break;
			case Regexp::of:
				s += "|";
				break;
			case Regexp::plus:
				s += "+";
				break;
		}
		sy++;	
		s += "<"+sy+">";
		process_with_dups(*re.geefTweedeOperand(), sy, st);		
		st.push(s);		
	}
}
void demo() {
	string s;
    cout<<"geef regexp:\n";
    cin>>s;
    cerr<<"string:"<<s<<":\n";
    try{
        Regexp re(s);
        cout<<s<<'\n'<<re<<'\n';
    }
    catch(Taalexceptie &te){
        cerr<<te.what()<<'\n';
    }
	Symbool sy;
	cout << (sy++);
	cout << (sy) << (sy++)  << endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	map<string, string> mp;
	stack<string> st;
	Regexp re("(x)((a)|(c))(d)a");
	cout << re << endl;
	Symbool sy;
	process_with_dups(re, sy, st);

	while (!st.empty()) {
		cout << st.top() << endl;
		st.pop();
	}

    return 0;
}

