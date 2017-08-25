// LGD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
using namespace std;

int lcs(string A, string B, vector<vector<int>>& L) {	
	int n = A.length();
	int m = B.length();

	for (int i = n; i >= 0; i--)
		for (int j = m; j >= 0; j--) {
			if (A[i] == '\0' || B[j] == '\0')
				L[i][j] = 0;
			else if (A[i] == B[j])
				L[i][j] = 1 + L[i+1][j+1];
			else
				L[i][j] = (L[i+1][j] > L[i][j+1] ? L[i+1][j] : L[i][j+1]);
		}

	return L[0][0];
}
string subseq(string A, string B, vector<vector<int>>& L) {
	string ret;
	int i = 0, j = 0;
	int n = A.length();
	int m = B.length();

	while (i < n && j < m) {
		if (A[i] == B[j]) {
			ret+= A.c_str()[i];
			++i;++j;
		} else if (L[i+1][j] >= L[i][j+1]) {
			i++;
		} else {
			j++;
		}
	}

	return ret;
}
int _tmain(int argc, _TCHAR* argv[]) {
	string a = "aloysius";
	string b = "louis";
	vector<vector<int>> L;
	L.resize(a.length()+1);
	for (int i = 0; i <= a.length(); i++)
		L[i].resize(b.length()+1);

	cout << "LCS length: " << lcs(a.c_str(), b.c_str(), L) << endl;
	cout << "LCS substr: " << subseq(a,b,L).c_str() << endl << endl;
	return 0;
}
