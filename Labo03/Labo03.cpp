// Labo03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Brug {
public:
	Brug(int van, int naar, int prijs):van(van),naar(naar),prijs(prijs) { }
	const int van, naar, prijs;
};


class Deelop {
public:
	vector<Brug*> bruggen;
	int totkost;
	Deelop(vector<Deelop*>* dops, Brug *brug);

	bool zalKruisen(Brug *nieuw) {
		for ( vector<Brug*>::reverse_iterator bi = bruggen.rbegin(); bi != bruggen.rend(); bi++) {
			Brug *b = *bi;
			if ((nieuw->naar < b->naar && nieuw->van > b->van) || (nieuw->naar > b->naar && nieuw->van < b->van))
				return true;
			}
		return false;
	}
};
Deelop::Deelop(vector<Deelop*>* dops, Brug *brug) : totkost(brug->prijs) {
	if (dops->empty()) {
		bruggen.push_back(brug);
	} else {
		//alle vorige deeloplossingen overlopen om de optimaalste te kiezen waarbij brug toegevoegd kan worden
		Deelop* kandidaat=0; //de beste kandidaat om later nieuwe brug toe te voegen
		for ( vector<Deelop*>::reverse_iterator dpi = dops->rbegin(); dpi != dops->rend(); dpi++) {
			Deelop *dop = *dpi;			
				if (kandidaat==0 || dop->totkost > kandidaat->totkost)
					if (!dop->zalKruisen(brug))
						kandidaat = dop;
		}
		if (kandidaat != 0) {
			if (!(kandidaat->bruggen).empty())
				for (Brug* br : kandidaat->bruggen) {
					this->bruggen.push_back(br); //de optimaalste kandidaatoplossing kopieeren naar huide oplossing
					totkost += br->prijs;
				}
			this->bruggen.push_back(brug); //nieuwe brug toevoegen
		} else {
			//deze brug kruist alle voorgaande, maak er een aparte deeloplossing van
			bruggen.push_back(brug);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[]) {
	std::ifstream infile("versnick.dat"); //versnick   simpelBrug
	
	int n;
	infile >> n;
	cout << n << " bruggen" << endl;
	
	vector<Brug*> bruggen;
	int a, b, i = 0;
	while (infile >> a >> b) {
		Brug* br = new Brug(i++,b,a);
		bruggen.push_back(br);
	}

	/////algo
	vector<Deelop*>* dops = new vector<Deelop*>();
	Deelop* dop;
	for (int i = 0; i < n; i++) {
		Brug* next = bruggen[i];
		dop = new Deelop(dops, next);		
		dops->push_back(dop);
	}
	//duurste deeloplossing bepalen
	Deelop* duurste = dops->front();
	for (Deelop* dp : *dops)
		if (dp->totkost > duurste->totkost)
			duurste = dp;

	cout << "duurste deeloplossing kost " << duurste->totkost << " en heeft " << duurste->bruggen.size() << " bruggen" << endl << endl;

	return 0;
}
