// BoyerMoore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class BoyerMoore {
private:
	string P;
	vector<int> occur, suff, skip;
public:
	BoyerMoore(string P) {
		this->P = P;
		
		BadChar();
		GoodSuffix();
	}

	void BadChar() {
		occur.resize(256,-1);
		for (int i = 0; i < P.size(); i++)
			occur[P.at(i)]=i;
	}
	void GoodSuffix()
	{
		//suffix functie:
		//	voor elke positie j --> in de string: P[j,p] ; s(j) bepaalt de lengte v/d langste prefix die ook suffix is in die string.
		
		int p = P.size();
		suff.resize(p+1,0);
				
		for (int i = p-1; i > 0; i--) {
			int j = p - suff[i];
			while (j < p && P.at(i-1) != P.at(j-1) ) {
				j = p - suff[j];					
			}
			suff[i-1] = p - j + ( P.at(i-1) == P.at(j-1)? 1 : 0);			
		}

		//shift vector (v[i] uit cursus)
		p = P.size();
		skip.resize(p,p - suff[0]);
		
		for (int j = p-1; j > 0; j--) {
			int i = p - suff[j] - 1;
			if(skip[i] > i+1-j)
				skip[i] = i+1-j;
		}
		
	}

	vector<int> find(string T) {
		vector<int> ret;
		int n = T.length();
		int m = P.length();
		int i=0, j;
		while (i<=n-m) {
			j=m-1;
			while (j >= 0 && P.at(j)==T.at(i+j))
				j--;
			if (j < 0) {
				ret.push_back(i);
				i += skip[0];
			} else {
				int H1 = j-occur[T.at(i+j)];
				int H2 = skip[j];
				if (H2 > H1)
					i+= H2;
				else
					i+= H1;
			}
		}
		return ret;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	string T = "abbazab abbababbabab";
	cout << "i: ";
	for (int i = 0; i < T.size(); i++) {
		cout << i << " ";
	}
	cout << endl << "T: ";
	for (int i = 0; i < T.size(); i++) {
		cout << T.at(i) << " ";
	}
	cout << endl;

	string P = "abbabab";
	BoyerMoore bm(P);
	vector<int> found = bm.find(T);
	cout << endl <<endl<< "found at pos(s): ";
	for (int i = 0; i < found.size(); i++)
		cout << found[i] << ", ";
	cout << endl;
	return 0;
}

