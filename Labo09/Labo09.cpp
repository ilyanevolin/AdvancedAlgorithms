// Labo09.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "shiftand.h"
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include <map>
#include <queue>
using namespace std;

void test_replacement() {
	//controle p5: www.cs.uku.fi/~kilpelai/BSA05/lectures/slides05.pdf
	string P = "abcd";
	ShiftAnd sa((unsigned char*)P.c_str(), P.length());

	string T = "abczabcde";
	std::queue<const uchar*> q = sa.match_replacement_k_1((unsigned char*)T.c_str(), T.length());

	while (!q.empty()) {
		const uchar* c = q.front();
		q.pop();
		for (int i = P.length()-1; i >= 0; i--) {
			cout << *(c-i);
		}
		cout << endl;
	}
}
void test_insertion_append() {
	string P = "abcd";
	ShiftAnd sa((unsigned char*)P.c_str(), P.length());

	string T = "xyzabcdxyz";
	std::queue<const uchar*> q = sa.match_insertion_k_1((unsigned char*)T.c_str(), T.length());

	while (!q.empty()) {
		const uchar* c = q.front();
		q.pop();
		for (int i = P.length()-1; i >= -1; i--) {
			cout << *(c-i);
		}
		cout << endl;
	}	
}

void test_insertion_middle() {
	string P = "abcd";
	ShiftAnd sa((unsigned char*)P.c_str(), P.length());

	string T = "abced";
	std::queue<const uchar*> q = sa.match_insertion_k_1((unsigned char*)T.c_str(), T.length());

	while (!q.empty()) {
		const uchar* c = q.front();
		q.pop();
		for (int i = P.length()-1; i >= -1; i--) {
			cout << *(c-i);
		}
		cout << endl;
	}	
}

void test() {
	cout << "Test replacement:" << endl;
	test_replacement();
	
	cout << endl << "Test insertion append:" << endl;
	test_insertion_append();

	cout << endl << "Test insertion middle:" << endl;
	test_insertion_middle();
	
	
	cout << endl << endl;
}
void opl() {
	/*
		for each word in Kapital
			if word.len <= 2 | isNot [a-zA-Z] then continue
			else if no queue found for word then calculate queue and map <word, queue> :: calculation based on DonQuijote
			finally take next from queue new_word and replace word by new_word
		write string back to file
	*/
	string line;
	ifstream dqi; 
	string dq;
	dqi.open("donquijote.txt"); 
	while (std::getline(dqi, line)) {
		dq += line + "\n";
	}
	dqi.close();
	int Tl = dq.length();
	unsigned char* T = (unsigned char*)dq.c_str();

	ifstream infile; 
	infile.open("capital.txt"); 
	std::ofstream out("capital_new.txt");

	map<string, queue<const uchar*>> mp;
	while (std::getline(infile, line)) {
		istringstream iss(line);
		string word;
		while(iss >> word) {
			if (word.length() > 2) {
				if (word == "style=\"border:none;padding-top:0cm;padding-bottom:0cm;padding-left:0.191cm;padding-right:0.191cm;\"|") {
					cout << "";
				}
				map<string,queue<const uchar*>>::iterator it = mp.find(word);
				queue<const uchar*> q;
				if (it == mp.end()) { //key not exists, add it
					try {
						string P = word;
						ShiftAnd sa((unsigned char*)P.c_str(), P.length());
						q = sa.match_replacement_k_1(T, Tl);
						mp.insert(make_pair(word, q));
					 } catch (exception e) {
						continue;
					}
				} else {
					q = it->second;
				}
				//key exists, do work
				if (!q.empty()) {
					string newWord;
					const uchar* c = q.front();
					q.pop();
					q.push(c); //cyclic queue
					for (int i = word.length()-1; i >= 0; i--)
						newWord += *(c-i); 
					out << newWord;					
				} else {
					out << word;
				}
			} else {
				out << word;
			}
			out << " ";
		}		
		out.flush();
		out << "\n";
	}
	infile.close();

	
   
    out.close();
}
int _tmain(int argc, _TCHAR* argv[]){
	test();
	//opl();
	

	return 0;
}

