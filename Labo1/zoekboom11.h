#ifndef __ZOEKBOOM_H
#define __ZOEKBOOM_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>

#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>
using namespace std;

template <class Sleutel,class Data>
class Zoekknoop;

template <class Sleutel,class Data>
class Zoekboom : public unique_ptr<Zoekknoop<Sleutel,Data>>{
	public:
		Zoekboom() { n = 0;}
		Zoekboom(Zoekknoop<Sleutel,Data>* el) : std::unique_ptr<Zoekknoop<Sleutel,Data>>(el) { }
		void voegtoe(const Sleutel& sleutel,const Data& data);
		void schrijf(bool simple = false);
		int diepte();
		double gem_knoopdiepte();
		const int knopen() {return n;}
		void roteer(bool right); //true = van links->rechts
		void onevenwichtig();
		void evenwichtig();
	protected:
		//zoekfunctie zoekt sl en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
		//ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
		//noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
		void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel,Data>*& ouder, Zoekboom<Sleutel,Data>*& plaats);	
		int telKnopen();
		int balance(); // balance(N) = Depth(Nleft) - Depth(Nright)
		int n;
};
template <class Sleutel,class Data>
class Zoekknoop {
	public:
		Zoekknoop(Sleutel sl,Data dt) : data(dt),sleutel(sl) { }
    friend class Zoekboom<Sleutel,Data>;
    protected:
        Sleutel sleutel;
        Data data;
        Zoekknoop<Sleutel,Data>* ouder;
        Zoekboom<Sleutel,Data> links, rechts;
        Zoekknoop() : ouder(0){}
};

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::roteer(bool right) {	
	Zoekboom<Sleutel,Data> *p;			
	// 1)
	p = &(right ? this->get()->links : this->get()->rechts);					
	Zoekknoop<Sleutel,Data> *ouder = (this->get()->ouder);
	this->swap(*p);
	if (this && *this)
		this->get()->ouder = std::move(ouder);
	else 
		return;
	p->get()->ouder = this->get();
	// 2)
	Zoekboom<Sleutel,Data>* beta_to = &(right ? p->get()->links : p->get()->rechts);	
	if (beta_to && *beta_to) {
		Zoekboom<Sleutel,Data>* beta = &(right ? this->get()->rechts : this->get()->links);
		if (beta && *beta)
			beta->get()->ouder = p->get();
		beta_to->swap(*beta);
	}
}

template <class Sleutel,class Data>
int Zoekboom<Sleutel,Data>::balance() {
	int B = 0, dL = 0, dR = 0;
	if (this->get()->links)
		dL = this->get()->links.telKnopen();
	if (this->get()->rechts)
		dR = this->get()->rechts.telKnopen();
	B = dL - dR;
	return B;
}
template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::onevenwichtig() {
	Zoekboom<Sleutel,Data> *p = this;	
	while (p->get()->links) {
		p->roteer(true);	
	}
	while (p->get()->rechts) {
		if(p->get()->rechts.get()->links) {
			p->get()->rechts.roteer(true);
		} else
			p = &(p->get()->rechts);
	}
}

/*
	DSW Algorithm :: make perfect tree
	PDF: http://www.cs.up.ac.za/files/COS212/Download/242/  (page 13-15)
	1) calculate the difference between #nodes in vine and m=2^lg(n+1) -1
		lg(n+1) is height of balanaced tree with n nodes
		m= 2^lg(n+1) -1 is #nodes in complete BST
		thus: n-m = # of nodes which "overflow" a complete BST ==> these will be our most left nodes
	2)		--> we have to rotate left (n-m) times, so that these overflow nodes go left.
	3) next up: in while loop we rotate left for every 2nd node in the backbone/vine; starting from the root.
		**skip a node.


*/
template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::evenwichtig() {	
	Zoekboom<Sleutel,Data>* p = this;
	int m = pow(2, floor(log(n+1)/log(2))) - 1 ;
	int rotations = (n-m);
	while (p && rotations-- > 0) {			
		p->roteer(false);	
	}	
	while (m > 1) {
		m/=2;
		p = this; //starting from the root
		for (int i=0;i<m;i++) {
			p->roteer(false);	
			p = &(p->get()->rechts); //**skip a node
		}
	}
}

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::voegtoe(const Sleutel& sleutel,const Data& data){
    Zoekboom<Sleutel,Data>* plaats;
    Zoekknoop<Sleutel,Data>* ouder = nullptr;
    zoek(sleutel, ouder, plaats);
    if (!*plaats){
        Zoekboom<Sleutel,Data> nieuw( new Zoekknoop<Sleutel,Data>(sleutel,data) );
        nieuw->ouder=ouder;
        //*plaats=std::move(nieuw);
		plaats->swap(nieuw);
		this->n++;
    };
    
};

template <class Sleutel,class Data>
void Zoekboom<Sleutel,Data>::zoek(const Sleutel& sleutel, Zoekknoop<Sleutel,Data>*& ouder, Zoekboom<Sleutel,Data>*& plaats){
    plaats=this;
    while (plaats && *plaats && (*plaats)->sleutel !=sleutel){
        if ((*plaats)->sleutel < sleutel) {
			ouder = plaats->get();
            plaats=&(*plaats)->rechts;
		}
        else {
			ouder = plaats->get();
            plaats=&(*plaats)->links;
		}
		if (!*plaats)
			return;
    };
};

template <class Sleutel,class Data>
void Zoekboom<Sleutel, Data>::schrijf(bool simple = false) {
	if (!this->get()) return;
	std::stringstream s1, s2;
	queue<Zoekboom<Sleutel,Data>*> q;
	q.push(this);

	while (!q.empty()) {
		Zoekboom<Sleutel,Data>* z = q.front();
		q.pop();
		if (!z->get())
			continue;
		
		s1 << z->get()->sleutel << "  ";

		s2 << z->get()->sleutel;
		s2 << "\t  parent: ";
		if (z->get()->ouder != nullptr) s2 <<  z->get()->ouder->sleutel; else s2<<" ";
		s2 << "\t  links: ";
		if (z->get()->links != nullptr) s2 <<  z->get()->links->sleutel; else s2<<" ";
		s2 << "\t  rechts: ";
		if (z->get()->rechts != nullptr) s2 <<  z->get()->rechts->sleutel; else s2<<" ";
		s2 << endl;

		q.push(&(z->get()->links));
		q.push(&(z->get()->rechts));
	}
	if (!simple)
		cout << s2.str() << "\n";
	cout << "levelorder:  " << s1.str() << "\n";
}

template <class Sleutel,class Data>
int Zoekboom<Sleutel, Data>::diepte() {
	if (!this || !*this) 
		return 0;
	int l = 0;
	if (this->get()->links)
		l = this->get()->links.diepte();
	int r = 0;
	if (this->get()->rechts)
		r = this->get()->rechts.diepte();
	return 1 + (l > r ? l : r);
	
}

template <class Sleutel,class Data>
double Zoekboom<Sleutel, Data>::gem_knoopdiepte() {
	if (!this || !*this || !this->get()) return 0;	
	queue<Zoekboom<Sleutel,Data>*> q;
	q.push(this);
	double d = 0;
	while (!q.empty()) {
		Zoekboom<Sleutel,Data>* z = q.front();
		q.pop();
		if (!z->get())
			continue;		
		if (z->get()->links)
			q.push(&(z->get()->links));
		if (z->get()->rechts)
			q.push(&(z->get()->rechts));				
		d += z->telKnopen();
	}
	return d/n;
}
template <class Sleutel,class Data>
int Zoekboom<Sleutel, Data>::telKnopen() {	
	if (!this && !*this)
		return 0;
	int d = 0;
	if (this->get()->links)
		d += this->get()->links.telKnopen();
	if (this->get()->rechts)
		d += this->get()->rechts.telKnopen();
	return 1+d;
}

#endif
