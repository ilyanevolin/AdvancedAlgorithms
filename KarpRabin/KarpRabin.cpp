// KarpRabin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class KarpRabin {
protected:
	long MR;
	int R;
	long patHash;
	string pat;
	long q;

	bool equals(string txt, int pos) {
		// monte carlo: return true;
		
		//las vegas:
		for (int i = 0; i < pat.length(); i++)
			if (txt[pos+i] != pat[i])
				return false;
		return true;
	}
public:
	KarpRabin(string pat) {
		this->pat = pat;
		MR = 1;
		R = 256;
		patHash = 0;
		q = 541; //some large prime

		for (int i = 1; i < pat.length(); i++)
			MR = (MR*R) % q;

		for (int i = 0; i < pat.length(); i++)
			patHash = (patHash*R + pat[i]) % q;
	}

	vector<int> find(string txt) {
		vector<int> ret;	

		long txtHash = 0;
		for (int i = 0; i < pat.length(); i++)
			txtHash = (txtHash * R + txt[i]) % q;

		if (txtHash == patHash && equals(txt, 0))
			ret.push_back(0);

		for (int i = pat.length(); i < txt.length(); i++) {
			txtHash = (txtHash + q - MR*txt[i-pat.length()]%q)%q; // remove leading digit
			txtHash = (txtHash*R + txt[i])%q;

			if (txtHash == patHash && equals(txt, i-pat.length()+1))
				ret.push_back(i-pat.length()+1);
		}

		return ret;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	string T = "cadcad";
	string P = "cad";
	KarpRabin kp(P);
	vector<int> found = kp.find(T);
	for (int i = 0; i < found.size(); i++)
		cout << found[i] << ", ";
	cout <<endl;
	return 0;
}

