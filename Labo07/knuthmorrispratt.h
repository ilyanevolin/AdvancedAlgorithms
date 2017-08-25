#include <queue>
#include <vector>
#include <iostream>
typedef unsigned int uint;
typedef unsigned char uchar;
using namespace std;
// http://jakeboxer.com/blog/2009/12/13/the-knuth-morris-pratt-algorithm-in-my-own-words/
// http://www.inf.fh-flensburg.de/lang/algorithmen/pattern/kmpen.htm
class KnuthMorrisPratt{
public:
    KnuthMorrisPratt(const uchar* naald,uint naaldlengte);
    void zoek(std::queue<const uint>& plaats, const uchar* hooiberg, uint hooiberglengte);
	const vector<int> Values() {return values; }
	private:
		const uchar* naald;
		uint naaldlengte;
		vector<int> values;
		vector<int> vs;
		int perf;
};
/*
	KMP werkt niet met een prefixfunctie q zoals volgt:
	P = "aaaaaaa"
	V -> -1 0 0 0 0 0 0

	als q stijgend is (-1 0 1 2 3 4 5 ....) dan moet KMP prefixfunctie nul opleveren !!! (0 0 0 0 0 0)
*/

KnuthMorrisPratt::KnuthMorrisPratt(const uchar*naald, uint naaldlengte) {
	cout << "length P: " << naaldlengte << endl;
	cout << "P:\t" << naald << endl;

	this->naald = naald;
	this->naaldlengte = naaldlengte;
	values.resize(naaldlengte+1,0);
	vs.resize(naaldlengte+1,0);

	// bereken q:
	values[0]=-1;
    for (int i = 1; i < naaldlengte; i++) {
        int j = values[i];
        while (j > 0 && naald[i] != naald[j])
            j = values[j];     
        values[i+1] = j + (naald[i]==naald[j] ? 1 : 0);
    }

	// bereken s en sla ze op in values

	for (int pos = 1; pos < naaldlengte; pos++) {
		int j = pos;
		while (j > 0 && naald[values[j]] == naald[pos])
			j = values[j];
		vs[pos] = pos - (j==0 ? 0:values[j]);
	}

	//values.erase(values.begin());

	cout << "\tq: ";
	for (int i = 0; i < Values().size(); i++)
		cout << Values()[i] ;
	cout << endl;
	cout << "\ts: ";
	for (int i = 0; i < vs.size(); i++)
		cout << vs[i] ;
	cout << endl;
}

void KnuthMorrisPratt::zoek(std::queue<const uint>& plaats, const uchar* hooiberg, uint hooiberglengte) {
	cout << "length T: " <<  hooiberglengte << endl;

	perf= 0;
	uint i = 0;		
	uint j = 0;
	while (i < hooiberglengte) {				
		++perf;
		while (j < naaldlengte && i < hooiberglengte && naald[j] == hooiberg[i])  {			
			++j;
			++i;
			++perf;
		}
		if (j == naaldlengte) { //has match, add position to result
			plaats.push(i-naaldlengte);
			//j = values[j-1];
			j -= vs[j-1];
			--perf;
		} else { //no match
			if (j != 0) {
				//j = values[j-1];
				j -= vs[j];
				--perf;
			} else {
				i++;
			}
		}
	}
	std::cout << "perf: " << perf << std::endl;
}
