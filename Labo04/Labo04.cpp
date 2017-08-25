// Labo04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "btree.h"
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

const int m = 6; //m=3;4;5;...
void test(), test_r(), test_rnd(), opl(), test_debug();
int _tmain(int argc, _TCHAR* argv[])
{
	
	//test();
	cout << endl << "--------------------------" << endl;
	//test_r();
	cout << endl << "--------------------------" << endl;
	//test_rnd();
	cout << endl << "--------------------------" << endl;
	//test_debug();
	cout << endl << "--------------------------" << endl;
	opl();
	int f;
	cin >> f;
	return 0;
}


void test() {	
	Schijf<Bknoop<string, unsigned int, m>> schijf;
	Btree<string, unsigned int, m> btree(schijf);
	// https://www.cs.usfca.edu/~galles/visualization/BTree.html

	

	btree.voegToe("a",1);
	btree.voegToe("b",1);
	btree.voegToe("c",1);
	btree.voegToe("d",1);
	btree.voegToe("e",1);
	btree.voegToe("f",1);
	btree.voegToe("f",1); //data ++
	btree.voegToe("g",1);	
	
	btree.voegToe("h",1);
	btree.voegToe("i",1);
	btree.voegToe("j",1);
	btree.voegToe("k",1);
	
	btree.voegToe("l",1);
	btree.voegToe("m",1);
	btree.voegToe("n",1);
	btree.voegToe("o",1);	
	
	btree.voegToe("p",1);	
	btree.voegToe("q",1);	
}

void test_r() {	
	Schijf<Bknoop<string, unsigned int, m>> schijf;
	Btree<string, unsigned int, m> btree(schijf);
	// https://www.cs.usfca.edu/~galles/visualization/BTree.html
	btree.voegToe("g",1);
	btree.voegToe("f",1);
	btree.voegToe("e",1);
	btree.voegToe("d",1);
	btree.voegToe("c",1);
	btree.voegToe("b",1);
	btree.voegToe("a",1);	
	
}

void test_rnd() {	
	Schijf<Bknoop<string, unsigned int, m>> schijf;
	Btree<string, unsigned int, m> btree(schijf);
	// https://www.cs.usfca.edu/~galles/visualization/BTree.html
	btree.voegToe("gr",1);	
	btree.voegToe("a",1);
	btree.voegToe("hr",1);
	btree.voegToe("erd",1);
	btree.voegToe("cer",1);
	btree.voegToe("bz",1);
	btree.voegToe("ape",1);	
	btree.voegToe("yape",1);	
	
}

void test_debug() {
	Schijf<Bknoop<string, unsigned int, m>> schijf;
	Btree<string, unsigned int, m> btree(schijf);
	btree.voegToe("Por",1,true);
btree.voegToe("cuanto",1,true);
btree.voegToe("por",1,true);
btree.voegToe("parte",1,true);
btree.voegToe("de",1,true);
btree.voegToe("vos",1,true);
btree.voegToe("Miguel",1,true);
btree.voegToe("de",1,true);

btree.voegToe("que",1,true);
btree.voegToe("habíades",1,true);
btree.voegToe("compuesto",1,true);
btree.voegToe("un",1,true);
btree.voegToe("y",1,true);
btree.voegToe("era",1,true);
btree.voegToe("muy",1,true);
btree.voegToe("útil",1,true);
btree.voegToe("y",1,true);
btree.voegToe("provechoso",1,true);

	std::priority_queue<Bknoop<string, unsigned int, m>, std::vector<Bknoop<string, unsigned int, m>>, CompareKnoop<string, unsigned int, m>>* q;
	q = btree.geefDataSorted();

	std::stack<Bknoop<string, unsigned int, m>> rq;
	while (!q->empty()) {
		Bknoop<string, unsigned int, m> k = q->top();
		q->pop();
		rq.push(k);		
	}
	while (!rq.empty()) {
		Bknoop<string, unsigned int, m> k = rq.top();
		rq.pop();
		cout << "\"" <<  k.sleutel[1] << "\"" << "\t\t komt " << k.data[1] << " keer voor." << endl;
	}
}

void opl() {
	std::ifstream infile("donquijote.txt");	
	std::string s;
	Schijf<Bknoop<string, unsigned int, m>> schijf;
	Btree<string, unsigned int, m> btree(schijf);
	int i = 1;
	while (infile >> s) {
		if (s.length() == 0 || strcmp(s.c_str(), "") == 0)
			continue;
		/*if (++i == 102)
			cout << i << endl;
		else
			cout << i << endl;*/
		btree.voegToe(s, 1, 0);
	}
	
		std::priority_queue<Bknoop<string, unsigned int, m>, std::vector<Bknoop<string, unsigned int, m>>, CompareKnoop<string, unsigned int, m>>* q;
		q = btree.geefDataSorted();

		std::stack<Bknoop<string, unsigned int, m>> rq; //reverse queue
		while (!q->empty()) {
			Bknoop<string, unsigned int, m> k = q->top();
			q->pop();
			rq.push(k);		
		}
		while (!rq.empty()) {
			Bknoop<string, unsigned int, m> k = rq.top();
			rq.pop();
			cout << "\"" <<  k.sleutel[1] << "\"" << "\t\t\t\t komt " << k.data[1] << " keer voor." << endl;
		}

}