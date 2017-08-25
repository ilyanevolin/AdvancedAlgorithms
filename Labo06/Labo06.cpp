// Labo06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "netwerkgraaf.h"
#include <fstream>
#include <vector>
#include "pugixml.h"
#include <iostream>
#include <functional>

using namespace std;


void m1(Netwerk<GERICHT,int>& g) {
	//MaxFlow problem.pdf
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();

	g.voegVerbindingToe(0,1, 4);
	g.voegVerbindingToe(0,4, 2);

	g.voegVerbindingToe(1,2, 4);
	g.voegVerbindingToe(1,3, 2);

	g.voegVerbindingToe(2,5, 3);

	g.voegVerbindingToe(3,2, 1);
	g.voegVerbindingToe(3,5, 1);

	g.voegVerbindingToe(4,3, 1);
	g.voegVerbindingToe(4,5, 3);
}
void m2(Netwerk<GERICHT,int>& g) {
	// https://www.youtube.com/watch?v=Tl90tNtKvxs
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();

	g.voegVerbindingToe(0,1, 10);
	g.voegVerbindingToe(0,3, 10);

	g.voegVerbindingToe(1,2, 4);
	g.voegVerbindingToe(1,3, 2);
	g.voegVerbindingToe(1,4, 8);

	g.voegVerbindingToe(2,5, 10);

	g.voegVerbindingToe(3,4, 9);

	g.voegVerbindingToe(4,2, 6);
	g.voegVerbindingToe(4,5, 10);

}
void m3(Netwerk<GERICHT,int>& g) {
	// voobreeld intranet
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();

	g.voegVerbindingToe(0,1, 7);
	g.voegVerbindingToe(0,2, 10);

	g.voegVerbindingToe(1,4, 4);

	g.voegVerbindingToe(2,1, 3);
	g.voegVerbindingToe(2,3, 5);
	g.voegVerbindingToe(2,5, 6);
	
	g.voegVerbindingToe(3,0, 2);
	g.voegVerbindingToe(3,6, 6);

	g.voegVerbindingToe(4,5, 6);

	g.voegVerbindingToe(5,7, 8);

	g.voegVerbindingToe(6,5, 2);
	g.voegVerbindingToe(6,7, 7);

	g.voegVerbindingToe(7,4, 1);

}
void oef1() {
	Netwerk<GERICHT, int> g(0);
	m3(g);

	Netwerk<GERICHT, int> opl, rest;
	opl.wordMaxStroomVan(g,rest,0,g.aantalKnopen()-1); //this will print max capacity of network
	//opl.schrijf(cout);
}
void oef2() {
	ifstream input("bipartitegraaf"); //last line must be whitespace
	int aantal, L, R;
	input >> aantal;
	input >> L;
	input >> R;

	Netwerk<GERICHT, int> g(aantal), rest;
	int P = g.voegKnoopToe(); //producren
	int C = g.voegKnoopToe(); //consumer

	while(! input.eof()) {

		if (g.geefTakdata(P, L) == 0 ) {
            g.voegVerbindingToe(P, L, 1);
        }
		
        g.voegVerbindingToe(L, R, 1);
        
        if (g.geefTakdata(R, C) == 0 ) {
            g.voegVerbindingToe(R, C, 1);
        }

		input >> L;
		input >> R;
	}
	Netwerk<GERICHT, int> opl;
	opl.wordMaxStroomVan(g,rest,P,C);
}

void m4(Netwerk<GERICHT,int>& g) {
	// http://jeffe.cs.illinois.edu/teaching/algorithms/notes/24-maxflowapps.pdf
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();
	g.voegKnoopToe();

	g.voegVerbindingToe(0,1, 1);
	g.voegVerbindingToe(1,0, 1);

	g.voegVerbindingToe(0,2, 1);
	g.voegVerbindingToe(2,0, 1);

	g.voegVerbindingToe(0,3, 1);
	g.voegVerbindingToe(3,0, 1);


	g.voegVerbindingToe(1,4, 1);
	g.voegVerbindingToe(4,1, 1);

	g.voegVerbindingToe(1,5, 1);
	g.voegVerbindingToe(5,1, 1);


	g.voegVerbindingToe(2,4, 1);
	g.voegVerbindingToe(4,2, 1);

	g.voegVerbindingToe(2,5, 1);
	g.voegVerbindingToe(5,2, 1);


	g.voegVerbindingToe(3,4, 1);
	g.voegVerbindingToe(4,3, 1);

	g.voegVerbindingToe(3,5, 1);
	g.voegVerbindingToe(5,3, 1);


	g.voegVerbindingToe(4,6, 1);
	g.voegVerbindingToe(6,4, 1);

	g.voegVerbindingToe(4,7, 1);
	g.voegVerbindingToe(7,4, 1);

	g.voegVerbindingToe(4,8, 1);
	g.voegVerbindingToe(8,4, 1);


	g.voegVerbindingToe(5,6, 1);
	g.voegVerbindingToe(6,5, 1);

	g.voegVerbindingToe(5,7, 1);
	g.voegVerbindingToe(7,5, 1);

	g.voegVerbindingToe(5,9, 1);
	g.voegVerbindingToe(9,5, 1);


	g.voegVerbindingToe(6,10, 1);
	g.voegVerbindingToe(10,6, 1);

	g.voegVerbindingToe(7,10, 1);
	g.voegVerbindingToe(10,7, 1);


	g.voegVerbindingToe(8,10, 1);
	g.voegVerbindingToe(10,8, 1);

	g.voegVerbindingToe(8,9, 1);
	g.voegVerbindingToe(9,8, 1);

	g.voegVerbindingToe(9,10, 1);
	g.voegVerbindingToe(10,9, 1);

}
void oef3_test() {
	Netwerk<GERICHT, int> g;
	m4(g);
	int P = 0;
	int C = g.aantalKnopen()-1;	
	g.verdubbelNetwerk(P,C); //verdubbel alle knopen
	Netwerk<GERICHT, int> opl;
	Netwerk<GERICHT, int> restnetwerk;
	opl.wordMaxStroomVan(g, restnetwerk, P,C); // should be 2: ok
	cout << "MaxFlow waarde geeft hier aan hoeveel knopen er verwijderd moeten worden." << endl;
	cout << "We weten wel niet welke knopen, hiervoor moeten we snede S bepalen:" << endl;

	std::vector<int> knopen;
	g.knopenInSnede(restnetwerk, knopen, P);
	for (int i = 0; i < knopen.size(); i++) {cout << (i > 0 ? ", " : "") << knopen[i];}cout << endl;

}

using namespace pugi;
const char* node_types[] =
{
    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
};
//bezoekt een (deel)boom met een visitor in preorder

void DEZBezoekpre(pugi::xml_node n, int &van, NetwerkMetKnoopdata<GERICHT, int, char*>&g, std::function<void(pugi::xml_node, int &van, NetwerkMetKnoopdata<GERICHT, int, char*> &g)> bezoekKnoop){
    bezoekKnoop(n, van, g);
    for (auto&& k:n.children())
        DEZBezoekpre(k,van,g,bezoekKnoop);
};
void verwerkXML(pugi::xml_node n, NetwerkMetKnoopdata<GERICHT, int, char*>& g) {
	int van = -1;
    DEZBezoekpre(n,van,g,[](pugi::xml_node nn, int &van, NetwerkMetKnoopdata<GERICHT, int, char*>&g){

		if (strcmp(nn.name(), "knooppunt") == 0) {
			pugi::xml_attribute attr = nn.attribute("nummer");
			char* dt = (char*)attr.value() ;
			int found = g.geefKnoopIndex(dt);
			if (found == -1)
				van = g.voegKnoopToe( dt );
			else
				van = found;
		} else if (strcmp(nn.name(), "route") == 0) {
			pugi::xml_attribute attr = nn.attribute("doel");
			char* dt = (char*)attr.value() ;
			int naar = g.geefKnoopIndex(dt);
			if (naar == -1)
				naar = g.voegKnoopToe( dt );
			else
				cout << "";
			g.voegVerbindingToe(van, naar, 1);
		}
    });
    
};
void oef3() {
	// aid: http://web.williams.edu/Mathematics/sjmiller/public_html/hudson/Eusden_maxflowmincut.pdf
	NetwerkMetKnoopdata<GERICHT, int, char*> g, restnetwerk;
	pugi::xml_document doc;
    if (!doc.load_file("treingraaf.xml")) return;
    verwerkXML(doc, g);

	char* begin = "Ffarquhar";
	char* eind = "Tidmouth";
	int P = g.geefKnoopIndex(begin);
	int C = g.geefKnoopIndex(eind);

	//verdubbel alle knopen (omdat we een MinCut moeten bepalen van Knopen, en niet takken; dus alles knopen omzetten in takken)
	g.verdubbelNetwerk(P,C);
	NetwerkMetKnoopdata<GERICHT, int, char*> opl;
	opl.wordMaxStroomVan(g,restnetwerk, P,C); //berekent MaxFlow (= som van alle capaciteiten van de takken in de MinCut)
	cout << "MaxFlow waarde geeft hier aan hoeveel knopen er verwijderd moeten worden." << endl;
	cout << "We weten wel niet welke knopen, hiervoor moeten we snede S bepalen:" << endl;

	//We weten nu dat we een MaxFlow van 15 hebben, maar welke 15 knopen zijn het nu die gesloten mogen worden?
	//http://www.geeksforgeeks.org/minimum-cut-in-a-directed-graph/
	//We gaan eerst, vanuit onze producten P, BFS zoeken en alle knopen bijhouden die wij konden bezoeken.
	//De knopen in de MinCut zijn dan diegene,
	// uit het origineel netwerk (=g), die niet in BFS voorkwamen maar wel rechtstreeks bereikbaar zijn (buren).
	std::vector<int> knopen;
	g.knopenInSnede(restnetwerk, knopen, P);
	for (int i = 0; i < knopen.size(); i++) {cout << knopen[i] << " : " << *g.geefKnoopdata(i) << endl;}cout << endl;

	
}


int _tmain(int argc, _TCHAR* argv[]) {
	//oef1();
	oef2();
	/*oef3_test();
	cout << "\n---------\n";
	oef3();*/
	return 0;
}

