#include <iostream>
#include "stdafx.h"

#include <fstream>
#include <time.h>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>
#include "probprob.h"
using namespace std;

/**********************************************************************

   Class: Dom
   
   beschrijving: klasse die een domme implementatie geeft van Probprob
   ze probeert een getal te vinden met een gegeven aantal cijfers dat zo klein mogelijk is.
   
***************************************************************************/
class Dom:public ProbProb<char>{
public:
    Dom(int aantal):aantal(aantal) {};
protected:
    virtual bool isOplossing(const vector<char>& deeloplossing){
        return deeloplossing.size()==aantal;
    };
    virtual double f(const vector<char>& oplossing){
        int totaal=oplossing[0]-'0';
        for (int i=1; i<oplossing.size(); i++ ){
            totaal*=2; //*=10
            totaal+=oplossing[i]-'0';
        }
        return totaal;
    };
//gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen
//samen met hun heuristische waarde
    virtual vector<pair<char,double> > geefVoortzetting(const vector<char>& deeloplossing){
        vector<pair<char,double> > voortzetting;
        for (int i=0; i<9; i++ ){
            voortzetting.emplace_back('0'+i,10-i);
        }
        return voortzetting;
    };
    int aantal;
};
void test1() {
	//mbv: probabilistic probing
	//we gaan opzoek naar een reeks getallen die de functie 'f' gebruiken en de kleinst mogelijke waarde opleveren
	srand(time(0));
    Dom dom(19);
    vector<char> optimum=dom.geefOptimum(2000);
    cout<<"Eindoplossing:\n";
    for (auto&& c : optimum ){
        cout<<" "<<c;
    }
	cout << "\t f = " << (int)dom.optimumFValue << endl << endl;
    cout<<"\n";
}

#include "simulanneal11.h"
#include "DomVoorbeeld.h";


void oef1() {
	Dom_met_SimAnnealing dom(19);	
	std::vector<char> opl = dom.oplossing();
	cout << "best solution:\n";
	dom.print(opl);
	cout << endl;
}

#include "TSA.h"

void oef2() {
	
	TSA_met_SimAnnealing<GERICHT, int, char*>* sa = new TSA_met_SimAnnealing<GERICHT, int, char*>();
	Pad<int> opl = sa->oplossing();
	sa->print(opl);
	cout << endl << endl;

}
int main(int argc, char *argv[]){
	//test1();
	//oef1();
	oef2();
    return 0;
}
