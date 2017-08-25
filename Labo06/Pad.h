#include <iostream>
using namespace std;

template< class T >
//Een Pad is een reeks van knoopnummers.
class Pad:public std::vector< int >{
	public:
		T geefCapaciteit() const{
			return capaciteit;
		}
		void zetCapaciteit(const T& _capaciteit){
			capaciteit=_capaciteit;
		}
	friend ostream& operator<<(ostream& os, const Pad<T>& p){
		os<<"Capaciteit= "<<p.capaciteit<<" :: ";
		if (p.size() > 0){
			os<<p[0];
		}
		for (int i=1; i<p.size(); i++ ){
			os<<"->"<<p[i];
		}
		os<<"\n";
	}
	protected:
		T capaciteit;
};

