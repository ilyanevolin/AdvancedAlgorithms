#include "stdafx.h"
#include "netwerkgraaf.h"
#include "pugixml.h"
#include <iostream>
#include <functional>

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
			if (found == -1) {
				std::string s(dt);
				char* ss = (char*)calloc(s.size()+1, sizeof(char));
				strncpy_s(ss, s.size()+1, dt, s.size()+1);
				van = g.voegKnoopToe( ss );
			}
			else
				van = found;
		} else if (strcmp(nn.name(), "route") == 0) {
			pugi::xml_attribute attr = nn.attribute("doel");
			char* dt = (char*)attr.value() ;
			int naar = g.geefKnoopIndex(dt);
			if (naar == -1) {
				std::string s(dt);
				char* ss = (char*)calloc(s.size()+1, sizeof(char));
				strncpy_s(ss, s.size()+1, dt, s.size()+1);
				naar = g.voegKnoopToe( ss );
			}
			else
				cout << "";

			pugi::xml_attribute attrDist = nn.attribute("afstand");
			double dist = (double)attrDist.as_double() ;
			g.voegVerbindingToe(van, naar, dist);
		}
	});
    
};



template<RichtType RT, class Takdata, class Knoopdata>
class TSA_met_SimAnnealing : public SimulatedAnnealing<	Pad<Takdata> > {
public:
	TSA_met_SimAnnealing() {	
		srand(time(0));

		_T0 = -1;		
		T0();
		coolingRate = 0.01; //set to 0.001 ** better solution (more time required)

		char* begin = "Ffarquhar";
		g.voegKnoopToe(begin);
		pugi::xml_document doc;
		if (!doc.load_file("treingraaf.xml")) return;
		verwerkXML(doc, g);
		
		cout << "";
		/*g.voegKnoopToe("start");
		g.voegKnoopToe("A");
		g.voegKnoopToe("B");
		g.voegKnoopToe("C");
		g.voegVerbindingToe(0,1,5);
		g.voegVerbindingToe(1,2,2);
		g.voegVerbindingToe(2,3,4);
		g.voegVerbindingToe(3,0,6);

		g.voegVerbindingToe(1,0,7);
		g.voegVerbindingToe(2,1,5);
		g.voegVerbindingToe(3,2,3);
		g.voegVerbindingToe(0,3,1);

		g.schrijf(cout);*/
	}

protected:
	NetwerkMetKnoopdata<GERICHT, int, char*> g;
	double coolingRate;
	int INFIN ;

	virtual double T0() { 
		if (_T0 != -1)
			return _T0;
		else
			return (_T0 = 1000); //trial & error
	}	
	virtual bool terminateCond() {
		return T < 1 ;
	}
	virtual double f(const Pad<Takdata>& oplossing) {
		int totaal = 0;
		
		for (int i = 1; i < oplossing.size(); i++) {
			Takdata* td = g.geefTakdata(oplossing[i-1], oplossing[i]);
			if (td == 0)
				return INFIN;
			totaal += *td;
		}


        return totaal;
	}
	virtual Pad<Takdata> initialSolution() {	
		
		Pad<int> opl;
		opl.resize(g.aantalKnopen()-1, 0);
		for (int i = 1; i < opl.size()+1; i++) 
			opl[i-1] = i;		
		random_shuffle(opl.begin(), opl.end());

		opl.push_back(0);
		int tmp = opl[0];
		opl[0] = 0;
		opl[opl.size()-1] = tmp;

		return opl;
	}
	virtual Pad<Takdata> pickAtRandom(const Pad<Takdata>& from) {
		
		Pad<Takdata> newOpl = from;
		int x = 1 + rand()%(newOpl.size()-1);
		int y = 1 + rand()%(newOpl.size()-1);
		while (x == y)
			y = 1 + rand()%(newOpl.size()-1);
		Takdata dt = newOpl[x];
		newOpl[x] = newOpl[y];
		newOpl[y] = dt;
		return newOpl;
	}
	virtual double p(double T, double deltaf) {
		double p = exp(-deltaf/T);
		return p;
	}
	virtual void updateT() {
		T = T-coolingRate;
	}
public:
	vector<int> seen;
	virtual void print(const Pad<Takdata>& v) {
		INFIN = 999999999;
		int F = f(v);
		if (std::find(seen.begin(), seen.end(), F) == seen.end()) {
			/*for (int i = 1; i < v.size(); i++) {
				cout << "van: "<< *g.geefKnoopdata(v[i-1]) << " -> " << *g.geefKnoopdata(v[i]) << "\t";
			}*/
		
			cout << endl << "  f=" << F << endl << endl;
			seen.push_back(F);
		}
	}
};
