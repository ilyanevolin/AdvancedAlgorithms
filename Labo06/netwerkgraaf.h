#include <iostream>
#include <queue>
#include <map>
#include <assert.h>
#include "graaf.h"
#include "Vergrotendpadzoeker.h"
#include <vector>

using namespace std;

template<RichtType RT,class Takdata>
class Netwerk: public GraafMetTakdata<RT, Takdata> {
public:
	Netwerk():GraafMetTakdata<RT, Takdata>(0){};
	Netwerk(int n);
	void wordMaxStroomVan(const Netwerk< RT,Takdata >& netwerk, Netwerk< RT,Takdata> & restnetwerk, int producent, int verbruiker);
	void knopenInSnede(const Netwerk< RT,Takdata> & restnetwerk, std::vector<int>& knopen, int producent);
	void verdubbelNetwerk(int P, int C);
	Netwerk<RT,Takdata>& operator+=(const Pad<Takdata>& path);
	Netwerk<RT,Takdata>& operator-=(const Pad<Takdata>& path);
	void geefVergrotendPad(Pad<Takdata>& path,int producent, int verbruiker);	
};

template < RichtType RT,class Takdata >
Netwerk< RT,Takdata >::Netwerk(int n) {
	knopen.resize(n);
};

template < RichtType RT,class Takdata >
void Netwerk< RT,Takdata >::wordMaxStroomVan(const Netwerk<RT,Takdata >& netwerk, Netwerk<RT,Takdata>& restnetwerk, int producent, int verbruiker) {
	if (!isGericht())
	    throw GraafExceptie("Ongerichte grafen zijn geen stroomnetwerken.");
		
	this->knopen.resize( netwerk.aantalKnopen() ); //zetAantalKnopen(); 
	restnetwerk=netwerk;
	Pad<Takdata> vergrotendPad;	
	restnetwerk.geefVergrotendPad(vergrotendPad,producent,verbruiker); //vergrotendPad bevat een willekeurig pad
	int f = 0;
	while(vergrotendPad.size() > 0 ) {	
		//for (int i = 0; i < vergrotendPad.size(); i++) {cout << (i > 0 ? " --> " : "") << vergrotendPad[i];}cout << endl;
		f += vergrotendPad.geefCapaciteit();
	    restnetwerk-=vergrotendPad; 		
	    *this+=vergrotendPad;// this is initieel leeg, we use capacity of vergrotendPad to add paths
	    restnetwerk.geefVergrotendPad(vergrotendPad,producent,verbruiker);		
	}
	cout << endl << "max flow: " << f << endl<<endl;	
};

template < RichtType RT,class Takdata >
void Netwerk< RT,Takdata >::knopenInSnede(const Netwerk<RT,Takdata> & restnetwerk, std::vector<int>& knopen, int producent) {
	vector<bool> visited(restnetwerk.aantalKnopen(), false);
	queue<int> q;
	q.push(producent);
	while (!q.empty()) {
		int k = q.front();
		q.pop();
		visited[k] = true;

		std::map<int,int> buren = restnetwerk[k];
		for (std::map<int,int>::iterator it = buren.begin(); it!=buren.end();it++) {
			const Takdata* td = restnetwerk.geefTakdata(k, it->first);
			if (td != 0 && *td > 0) {
				if (!visited[it->first]) {
					q.push(it->first);
					//cout << it->first << "   ";
				}
			}
		}
	}

	for (int i = 0; i < visited.size(); i++) {
		if (visited[i] == true) {
			std::map<int,int> buren = this->knopen[i];			
			for (std::map<int,int>::iterator it = buren.begin(); it!=buren.end();it++) {
				if (visited[it->first] == false) 
					if(std::find(knopen.begin(), knopen.end(), it->first) == knopen.end()) {
						//it->first is out-vertex (Vout)
						knopen.push_back(it->first);		 //we don't know the nodeID in original network of Vin, but we do know the name/data	
					}
			}
		}
	}

	
	
}

template < RichtType RT,class Takdata >
void Netwerk< RT,Takdata >::geefVergrotendPad(Pad<Takdata>& path,int producent, int verbruiker) {
	// path needs to be filled in based on this graph.
	Vergrotendpadzoeker<int> zoeker(*this,producent, verbruiker, path);
}
template < RichtType RT,class Takdata >
Netwerk< RT,Takdata >& Netwerk< RT,Takdata >::operator+=( const Pad<Takdata>& pad ) {
	for (int i = 1; i < pad.size(); i++) {
        Takdata van = pad[i-1];
        Takdata naar = pad[i];
        
		Takdata* verbinding = this->geefTakdata(van, naar);
		if ( verbinding != 0 ) {
			*verbinding += pad.geefCapaciteit();
		} else {
			this->voegVerbindingToe(van, naar, pad.geefCapaciteit());
		}
        
		Takdata* terugverbinding = this->geefTakdata(naar, van);
        if ( terugverbinding != 0 ) {
			*terugverbinding -= pad.geefCapaciteit();
		}
	}
	return *this;
}

template < RichtType RT,class Takdata >
Netwerk< RT,Takdata >& Netwerk< RT,Takdata >::operator-=( const Pad<Takdata>& pad ) {
    
	for (int i = 1; i < pad.size(); i++) {
        Takdata van = pad[i-1];
        Takdata naar = pad[i];
		Takdata* verbinding = this->geefTakdata(van, naar);
		*verbinding -= pad.geefCapaciteit();
        
		Takdata* terugverbinding = this->geefTakdata(naar, van);
		if ( terugverbinding != 0 ) {
			*terugverbinding += pad.geefCapaciteit();
		} else {
			this->voegVerbindingToe(naar, van, pad.geefCapaciteit());
		}
	}
	return *this;
}

template<RichtType RT, class Takdata>
void Netwerk<RT,Takdata>::verdubbelNetwerk(int P, int C) {
	int n = this->knopen.size();
	for (int Vin = 0; Vin < n; Vin++) {
		if (Vin == P || Vin == C)
			continue;
		int Vout = this->voegKnoopToe();		
		std::map<int, int> outgoing_Vin = this->knopen[Vin]; //get outgoing edges of Vin
		for (std::map<int, int>::const_iterator it = outgoing_Vin.begin(); it != outgoing_Vin.end(); it++) {			
			this->voegVerbindingToe(Vout, it->first, *this->geefTakdata(Vin, it->first) );
			this->verwijderVerbinding(Vin, it->first);
		}
		this->voegVerbindingToe(Vin, Vout, 1); // create edge from Vin to Vout
	}
}

template<RichtType RT,class Takdata,class Knoopdata>
class NetwerkMetKnoopdata:	public Netwerk<RT, Takdata>,
							public GraafMetKnoopdata<RT, Knoopdata> {

	public:
		virtual void wis(){
			GraafMetKnoopdata<RT,Knoopdata>::wis();
			this->takdatavector.clear();
		}
		int geefKnoopIndex(Knoopdata &dt) {
			for (int i = 0; i < this->knoopdatavector.size(); i++) {
				if (strcmp(this->knoopdatavector[i],dt)==0)
					return i;
			}
			return -1;
		}
		void wordMaxStroomVan(const NetwerkMetKnoopdata<RT,Takdata,Knoopdata>& netwerk, NetwerkMetKnoopdata<RT,Takdata,Knoopdata> & restnetwerk, int producent, int verbruiker);
};
template<RichtType RT,class Takdata,class Knoopdata>
void NetwerkMetKnoopdata<RT,Takdata,Knoopdata>::wordMaxStroomVan(const NetwerkMetKnoopdata<RT,Takdata,Knoopdata >& netwerk, NetwerkMetKnoopdata<RT,Takdata,Knoopdata> & restnetwerk, int producent, int verbruiker) {
	if (!isGericht())
	    throw GraafExceptie("Ongerichte grafen zijn geen stroomnetwerken.");
		
	this->knopen.resize( netwerk.aantalKnopen() ); //zetAantalKnopen(); 
	this->knoopdatavector=netwerk.knoopdatavector;
	restnetwerk=netwerk;
	Pad<Takdata> vergrotendPad;	
	restnetwerk.geefVergrotendPad(vergrotendPad,producent,verbruiker); //vergrotendPad bevat een willekeurig pad
	int f = 0;
	while(vergrotendPad.size() > 0 ) {	
		//for (int i = 0; i < vergrotendPad.size(); i++) {cout << (i > 0 ? " --> " : "") << vergrotendPad[i];}cout << endl;
		f += vergrotendPad.geefCapaciteit();
	    restnetwerk-=vergrotendPad; 		
	    *this+=vergrotendPad;// this is initieel leeg, we use capacity of vergrotendPad to add paths
	    restnetwerk.geefVergrotendPad(vergrotendPad,producent,verbruiker);		
	}
	cout << endl << "max flow: " << f << endl<<endl;	
	
}




