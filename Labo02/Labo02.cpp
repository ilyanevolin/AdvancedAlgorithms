// Labo02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "splayboomb.h"
#include "puntgenerator.h"
#include "csv.h"

#include <cstdlib>
#include <functional>
using namespace std;

void oef1(), oef2(), oef3_splaying(), bottomUp(), topDown();;
int _tmain(int argc, _TCHAR* argv[])
{
	//oef1();
	//oef2();

	//oef3_splaying();

	cout << "BottomUp:" << endl;
	bottomUp();
	cout <<endl;

	cout << "TopDown:" << endl;
	topDown();

	cout <<endl;
	return 0;
}

void oef1() {
	SplayboomB<int,int>* zb = new SplayboomB<int,int>();
	/*	zb->voegtoe(5,1);
	zb->voegtoe(3,1);
	zb->voegtoe(8,1);
	zb->voegtoe(1,1);
	zb->voegtoe(4,1);
	zb->voegtoe(6,1);
	zb->voegtoe(9,1);
	zb->schrijf();*/

	zb->voegtoe(10,1);
	zb->voegtoe(12,1);
	zb->voegtoe(5,1);
	zb->voegtoe(4,1);
	zb->voegtoe(6,1);

	zb->schrijf();

	zb->zoek(6, zb);

	zb->schrijf();
}
void oef2() {
	//Bereken ook eens de potentiaal van een boom met veel (pakweg 1 miljoen) punten als
    //1. Alles in volgorde is toegevoegd.
    //2. We punten in random volgorde toevoegen.

	SplayboomB<int,int>* zb;
	int num = pow(10,3);
	zb = new SplayboomB<int,int>();
	for (int i = 0; i < num; i++)
		zb->voegtoe(i,1);
	cout << "potentiaal van " << num << " elementen in volgorde: " << zb->berekenPotentiaal() << endl;

	zb = new SplayboomB<int,int>();
	int rnd = 0;
	for (int i = 0; i < num; i++)
		zb->voegtoe(rand()%100,1);
	cout << "potentiaal van " << num << " elementen randomly: " << zb->berekenPotentiaal() << endl;
}

void splay( vector<double> &punten, std::function<int(int)> index,int aantal, int geval){
	//opbouwen boom
	SplayboomB<int,int>* zb = new SplayboomB<int,int>();
	for (int i=0; i<aantal; i++ ){
        zb->voegtoe(punten[i],1);
    }

	//splayen boom
	std::string file = "dieptedata_geval_" + std::to_string (geval) + ".csv";
	CsvData csv(file,',');
	vector<double> dieptedata, potentialendata;
	int tel=0;
	int totaleZoekdiepte = 0;
    for (int i=0; i<aantal; i++ ){
        int nupunt=index(aantal);
		Zoekboom<int,int> * tmp = zb->zoek(nupunt, zb);
        tel++;
		totaleZoekdiepte += tmp->getAfdalingZoek(); //cumulatief voor elke 50 ops.
        if (tel == 50){
            dieptedata.push_back(totaleZoekdiepte);
			potentialendata.push_back(tmp->berekenPotentiaal());
            tel=0;
			totaleZoekdiepte = 0; //reset
        }
    }
	csv.voegDataToe(dieptedata); //zoekdiepte
	csv.voegDataToe(potentialendata); //potentiaal kolom

};
int scheveIndex(int aantal){
	//geeft een index terug die voor 99,9% zeker in de laatste 0,01% van de range ligt
    int keuze=rand()%1000;
    if (keuze==999)
        return rand()%(99*aantal/100);
    else
        return (99*aantal/100) + rand()%(aantal/100);
};
void oef3_splaying() {
	/*
		Nu gaan we splayen bekijken en zien of het iets oplevert.
		Daarvoor onderzoeken we verschillende gevallen.
		Hierbij maken we een grafiek van de zoekfase:
			we voeren bijvoorbeeld 10000 zoekoperaties uit en maken na elke groep van 50 operaties een datapunt voor
				1.De totale zoekdiepte van de 50 operaties samen.
				2.De potentiaal na de 50 operaties
		We doen dit voor 6 gevallen (6 bomen die op verschillend opgebouwd zijn)
	*/
	const int aantal=10000;
	vector<double> punten(aantal);

	//random opgebouwde boom
	for (int i =0;i<aantal;i++) punten[i] = rand()%aantal;
	splay(punten, [](int a){return rand()%a;}, aantal, 1);
    splay(punten, scheveIndex, aantal, 1);

	//opvolgeorde opgebouwde boom
	for (int i =0;i<aantal;i++) punten[i] = i;
	splay(punten, [](int a){return rand()%a;}, aantal, 2);
    splay(punten, scheveIndex, aantal, 2);

	//punt.gen. opgebouwde boom
	Puntgenerator dePuntgenerator;
	for (int i =0;i<aantal;i++) punten[i] = dePuntgenerator.geefpunt();
	splay(punten, [](int a){return rand()%a;}, aantal, 3);
    splay(punten, scheveIndex, aantal, 3);
}

void bottomUp() {
	SplayboomB<int,int>* zb = new SplayboomB<int,int>();

	zb->voegtoe(10,1);
	zb->zoek(10, zb);

	zb->voegtoe(12,1);
	zb->zoek(12, zb);

	zb->voegtoe(5,1);
	zb->zoek(5, zb);

	zb->voegtoe(4,1);
	zb->zoek(4, zb);

	zb->voegtoe(6,1);
	zb->zoek(6, zb);
	zb->schrijf();

	zb->verwijder(6);

	zb->schrijf();
	// ok https://www.cs.usfca.edu/~galles/visualization/SplayTree.html
}


void topDown() {
	SplayboomB<int,int>* zb = new SplayboomB<int,int>();

	zb->voegtoe(10,1);
	zb->zoekTopDown(10, zb);

	zb->voegtoe(12,1);
	zb->zoekTopDown(12, zb);

	zb->voegtoe(5,1);
	zb->zoekTopDown(5, zb);

	zb->voegtoe(4,1);
	zb->zoekTopDown(4, zb);

	zb->voegtoe(6,1);
	zb->zoekTopDown(6, zb);
	zb->schrijf();

	zb->verwijderTopDown(12);

	zb->schrijf();
	// https://www.cs.usfca.edu/~galles/visualization/SplayTree.html
}
