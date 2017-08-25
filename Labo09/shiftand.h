#include "bitpatroon.h"
#include <queue>
#include <iostream>
typedef unsigned int uint;
typedef unsigned char uchar;
using namespace std;

class ShiftAnd{
	public:
		ShiftAnd(const uchar* naald,uint _naaldlengte);
		std::queue<const uchar*> zoek(const uchar* hooiberg, uint hooiberglengte);
		std::queue<const uchar*> match_replacement_k_1(const uchar* hooiberg, uint hooiberglengte);
		std::queue<const uchar*> match_insertion_k_1(const uchar* hooiberg, uint hooiberglengte);
	protected:
		Bitpatroon S[256];
		uint naaldlengte;
};

ShiftAnd::ShiftAnd(const uchar* naald, uint length) {
	if (length > patroonlengte)
		throw exception("//naaldlengte moet kleiner dan patroonlengte zijn!");
	this->naaldlengte = length;
	for (int i = 0; i < naaldlengte; i++) {
		S[naald[i]] |= Bitpatroon::eenbit(i);
	}
	// 2147483648 = 2^31 == (1000...0)²  --> elke OR operatie verhoogt dit getal
	//opm Bitpatroon heeft << operatie om bitpatroon uit te schrijven
}

std::queue<const uchar*> ShiftAnd::zoek(const uchar* T, uint len) {
	uchar c = T[0];
	Bitpatroon R(0);
	std::queue<const uchar*> q;
	
	for (int j = 0; j < len; j++) {
		Bitpatroon Rnew = (R.shiftrechts(1) | Bitpatroon::eenbit(0)) & S[T[j]];

		cout << Rnew << endl;
		R = Rnew;		
		if (R.en(Bitpatroon::eenbit(naaldlengte-1)))
			q.push(&T[j]);
	}

	return q;
}

std::queue<const uchar*> ShiftAnd::match_insertion_k_1(const uchar* T, uint len) {
	uchar c = T[0];
	std::queue<const uchar*> q;
	Bitpatroon R(0);
	Bitpatroon R1(0);
	for (int j = 0; j < len; j++) {		
		R1 = R | ((R1.shiftrechts(1) | Bitpatroon::eenbit(0)) & S[T[j]] );
		//cout << R1 << endl;
		R = (R.shiftrechts(1) | Bitpatroon::eenbit(0)) & S[T[j]];		
		//cout << R << endl << endl;		

		if (R.en(Bitpatroon::eenbit(naaldlengte-1)) ||
			R1.en(Bitpatroon::eenbit(naaldlengte-1)))
		{
			q.push(&T[j-1]);
			continue;
		}
	}
	return q;
}

std::queue<const uchar*> ShiftAnd::match_replacement_k_1(const uchar* T, uint len) {
	//enkel een vervanging toelaten
	uchar c = T[0];
	std::queue<const uchar*> q;
	Bitpatroon R(0);
	Bitpatroon R1(0);
	for (int j = 0; j < len; j++) {		
		R1 = (R.shiftrechts(1) | Bitpatroon::eenbit(0)) | (R1.shiftrechts(1) & S[T[j]]);
		//cout << R1 << endl;
		R = (R.shiftrechts(1) | Bitpatroon::eenbit(0)) & S[T[j]];		
		//cout << R << endl << endl;		

		if (R.en(Bitpatroon::eenbit(naaldlengte-1)) ||
			R1.en(Bitpatroon::eenbit(naaldlengte-1)))
		{
			q.push(&T[j]);
			continue;
		}
		
		
	}

	return q;
}
    
