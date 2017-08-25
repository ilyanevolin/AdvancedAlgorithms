#ifndef __SPLAYBOOMB_H
#define __SPLAYBOOMB_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>

#include <stack>
#include <string>
#include <sstream>

#include "zoekboom11.h"
using namespace std;

/**********************************************************************

   Class: Zoekboom
   
   beschrijving: Binaire zoekboom waarin geen duplicaatsleutels zijn toegestaan.
   
***************************************************************************/

//Bottom-Up splayboom
template <class Sleutel,class Data>
class SplayboomB : public Zoekboom<Sleutel, Data>{
	public:
		SplayboomB() { }
		Zoekboom<Sleutel,Data>* zoek(const Sleutel& sleutel, Zoekboom<Sleutel,Data>* plaats);	
		void verwijder(const Sleutel& sleutel);	
		Zoekboom<Sleutel,Data>* zoekTopDown(const Sleutel& sleutel, Zoekboom<Sleutel,Data>* M);	
		void verwijderTopDown(const Sleutel& sleutel);	
	private:
		void splay(const Sleutel& sleutel, stack<Zoekboom<Sleutel,Data>*> &bomenOnderweg, stack<bool> &richtingOnderweg);
};
template <class Sleutel,class Data>
void SplayboomB<Sleutel,Data>::splay(const Sleutel& sleutel, stack<Zoekboom<Sleutel,Data>*> &bomenOnderweg, stack<bool> &richtingOnderweg) {
	bomenOnderweg.pop();	//remove c
	while (!bomenOnderweg.empty()) {
		Zoekboom<Sleutel,Data>* p = bomenOnderweg.top();
		bomenOnderweg.pop();
		bool c_RechtsVan_p = richtingOnderweg.top();
		richtingOnderweg.pop();

		if (bomenOnderweg.empty()) {
			// c is child of the root node			
			p->roteer(!c_RechtsVan_p);
			break;
		} else {
			// c is a child, has a parent and grandparent						
			Zoekboom<Sleutel,Data>* g = bomenOnderweg.top();
			bomenOnderweg.pop();
			bool p_RechtsVan_g = richtingOnderweg.top();

			if (p_RechtsVan_g == c_RechtsVan_p) { // zig-zig (LL && RR)
				g->roteer(!p_RechtsVan_g); //first grandparent-parent rotation
				richtingOnderweg.pop();
				//after rotation, g becomes p; so we perform rotate on g again (should be p in theory)
				g->roteer(!p_RechtsVan_g); //then parent-child rotation
			} else { // zig-zag (RL) and zag-zig (LR)
				p->roteer(!c_RechtsVan_p); //first grandparent-parent rotation
				richtingOnderweg.pop();
				//after rotation g remains itself
				g->roteer(!p_RechtsVan_g); //then parent-child rotation
			}
		}
	}	
};

template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>* SplayboomB<Sleutel,Data>::zoek(const Sleutel& sleutel, Zoekboom<Sleutel,Data>* plaats) {
    //zie theorie om L/R bij te houden, alsook de ouders om zigzag of zigzig te bepalen
	stack<Zoekboom<Sleutel,Data>*> bomenOnderweg; 
	stack<bool> richtingOnderweg; //true = rechts
	bomenOnderweg.push(plaats);	 //!!!!!!!!!!!
	int afdalingZoek = 0;
    while (plaats && *plaats && (*plaats)->sleutel !=sleutel){
		
		afdalingZoek++;
        if ((*plaats)->sleutel < sleutel) {
            plaats=&(*plaats)->rechts;
			if (*plaats) {
				bomenOnderweg.push(plaats);	//!!!!!!!!!!!
				richtingOnderweg.push(true);//!!!!!!!!!!!
			}
		}
        else {
            plaats=&(*plaats)->links;
			if (*plaats) {
				bomenOnderweg.push(plaats);	
				richtingOnderweg.push(false);
			}
		}			
    };
	
	plaats->afdalingZoek = afdalingZoek;

	// niet gevonden:	de laatst bezoechte knoop naar boven gesplayd.
	// wel gevonden:	de gezochte (lege plaats) wordt naar boven gebracht (de root)
	//splay it
	//pagina 18 cursus, bij zigzig eerst grootouder roteren, dan pas ouder!! ??
	//http://yaikhom.com/2014/05/12/understanding-splay-tree-rotations.html
	// http://i.stack.imgur.com/aK0Kn.png
	/*
		if zig-zig then rotate parent-grandparent first, then rotate newparent-X
		if zig-zag then rotate parent-X first, then rotate newparent-X
	*/
	
	splay(sleutel, bomenOnderweg, richtingOnderweg);
	return plaats;
};

template <class Sleutel,class Data>
void SplayboomB<Sleutel,Data>::verwijder(const Sleutel& sleutel) {
	cout << "deleting: " << sleutel << endl;
	Zoekboom<Sleutel,Data>* found = zoek(sleutel, this);
	Zoekboom<Sleutel,Data>* leftSubtree = new Zoekboom<Sleutel,Data>(found->get()->links.release());
	leftSubtree->get()->ouder = 0;
	Zoekboom<Sleutel,Data>* rightSubtree = new Zoekboom<Sleutel,Data>(found->get()->rechts.release());
	rightSubtree->get()->ouder = 0;

	Zoekboom<Sleutel,Data>* mostRightNode = leftSubtree;	
	while (mostRightNode->get()->rechts) {
		mostRightNode = &mostRightNode->get()->rechts;
	}
	zoek(mostRightNode->get()->sleutel, leftSubtree); //splay leftSubtree

	leftSubtree->get()->rechts.swap(*rightSubtree);

	this->swap(*leftSubtree);
};

template <class Sleutel,class Data>
Zoekboom<Sleutel,Data>* SplayboomB<Sleutel,Data>::zoekTopDown(const Sleutel& sleutel, Zoekboom<Sleutel,Data>* M) {

	//pseudocode taken from https://www.csee.umbc.edu/courses/undergraduate/CMSC341/fall02/Lectures/Splay/TopDownSplay.ppt

	//Zoekboom<Sleutel,Data>* M = this;
	Zoekboom<Sleutel,Data>* L = new Zoekboom<Sleutel,Data>();
	Zoekboom<Sleutel,Data>* R = new Zoekboom<Sleutel,Data>();
	
	while (M && (*M)->sleutel != sleutel) {
		if ((*M)->sleutel > sleutel) { //go left
			Zoekboom<Sleutel,Data>* M_left = &((*M)->links);		
			if ((*M_left)->sleutel > sleutel && (*M_left)->links && (*M_left)->links.get()->sleutel > sleutel) { // go left (zig zig)
				M->roteer(true);
				Zoekboom<Sleutel,Data>* tmp = move(M);
				M = new Zoekboom<Sleutel,Data>( (*tmp)->links.release() );
				(*M)->ouder = 0;				
				Zoekboom<Sleutel,Data>* loper = R;
				Zoekboom<Sleutel,Data>* loper_parent = 0;
				while( loper && *loper ) {
					loper = &((*loper)->links);
					loper_parent = loper;
				}
				if (loper_parent && *loper_parent)
					(*tmp)->ouder = loper->get()->ouder;
				loper->swap(*tmp);

			} else if ((*M_left)->sleutel < sleutel && (*M_left)->rechts && (*M_left)->rechts.get()->sleutel < sleutel) { // go right (zig zag)
				Zoekboom<Sleutel,Data>* tmp = move(M);
				Zoekboom<Sleutel,Data>* leftPart = new Zoekboom<Sleutel,Data>( (*tmp)->links.release() );				
				M = new Zoekboom<Sleutel,Data>( (*leftPart)->rechts.release() );
				(*M)->ouder = 0;				
	
				Zoekboom<Sleutel,Data>* loper = R;
				Zoekboom<Sleutel,Data>* loper_parent = 0;
				while( loper && *loper ) {
					loper = &((*loper)->links);
					loper_parent = loper;
				}
				if (loper_parent && *loper_parent)
					(*tmp)->ouder = loper->get()->ouder;
				loper->swap(*tmp); // tmp is "p and gamma"

				loper = L;
				loper_parent = 0;
				while( loper && *loper ) {
					loper = &((*loper)->rechts);
					loper_parent = loper;
				}
				if (loper_parent && *loper_parent)
					(*leftPart)->ouder = loper->get()->ouder;
				loper->swap(*leftPart);
			} else {
				// zig OR if element not found yet
				Zoekboom<Sleutel,Data>* tmp = move(M);
				M = new Zoekboom<Sleutel,Data>( (*tmp)->links.release() );
				(*M)->ouder = 0;

				Zoekboom<Sleutel,Data>* loper = R;
				Zoekboom<Sleutel,Data>* loper_parent = 0;
				while( loper && *loper ) {
					loper = &((*loper)->links);
					loper_parent = loper;
				}
				if (loper_parent && *loper_parent)
					(*tmp)->ouder = loper->get()->ouder;
				loper->swap(*tmp);
			}	
		} else { //go right
			Zoekboom<Sleutel,Data>* M_right = &((*M)->rechts);
			if ((*M_right)->sleutel < sleutel && (*M_right)->rechts && (*M_right)->rechts.get()->sleutel < sleutel) { // go right (zig zig)
				M->roteer(false);
				Zoekboom<Sleutel,Data>* tmp = move(M);
				M = new Zoekboom<Sleutel,Data>( (*tmp)->rechts.release() );
				(*M)->ouder = 0;				
				Zoekboom<Sleutel,Data>* loper = L;
				Zoekboom<Sleutel,Data>* loper_parent = 0;
				while( loper && *loper ) {
					loper = &((*loper)->rechts);
					loper_parent = loper;
				}
				if (loper_parent && *loper_parent)
					(*tmp)->ouder = loper->get()->ouder;
				loper->swap(*tmp);

			} else if ((*M_right)->sleutel > sleutel && (*M_right)->links && (*M_right)->links.get()->sleutel > sleutel) { // go left (zig zag)
				Zoekboom<Sleutel,Data>* tmp = move(M);
				Zoekboom<Sleutel,Data>* rightPart = new Zoekboom<Sleutel,Data>( (*tmp)->rechts.release() );				
				M = new Zoekboom<Sleutel,Data>( (*rightPart)->links.release() );
				(*M)->ouder = 0;				
	
				Zoekboom<Sleutel,Data>* loper = L;
				Zoekboom<Sleutel,Data>* loper_parent = 0;
				while( loper && *loper ) {
					loper = &((*loper)->rechts);
					loper_parent = loper;
				}
				if (loper_parent && *loper_parent)
					(*tmp)->ouder = loper->get()->ouder;
				loper->swap(*tmp); // tmp is "p and gamma"

				loper = R;
				loper_parent = 0;
				while( loper && *loper ) {
					loper = &((*loper)->links);
					loper_parent = loper;
				}
				if (loper_parent && *loper_parent)
					(*rightPart)->ouder = loper->get()->ouder;
				loper->swap(*rightPart);
			} else {
				// zig OR if element not found yet
				Zoekboom<Sleutel,Data>* tmp = move(M);
				M = new Zoekboom<Sleutel,Data>( (*tmp)->rechts.release() );
				(*M)->ouder = 0;

				Zoekboom<Sleutel,Data>* loper = L;
				Zoekboom<Sleutel,Data>* loper_parent = 0;
				while( loper && *loper ) {
					loper = &((*loper)->rechts);
					loper_parent = loper;
				}
				if (loper_parent && *loper_parent)
					(*tmp)->ouder = loper->get()->ouder;
				loper->swap(*tmp);
			}
		}
	}

	// re-assemble
	Zoekboom<Sleutel,Data>* loper = R;
	while( loper && *loper && loper->get() )
		loper = &(loper->get()->links);
	Zoekboom<Sleutel,Data>* tmp = new Zoekboom<Sleutel,Data>( (*M)->rechts.release() );
	loper->swap(*tmp);	
	R->swap( (*M)->rechts );
	
	loper = L;
	while( loper && loper->get() )
		loper = &(loper->get()->rechts);
	tmp = new Zoekboom<Sleutel,Data>( (*M)->links.release() );
	loper->swap(*tmp);
	L->swap( (*M)->links );
	
	this->swap(*M);

	return this;
};


template <class Sleutel,class Data>
void SplayboomB<Sleutel,Data>::verwijderTopDown(const Sleutel& sleutel) {
	cout << "deleting: " << sleutel << endl;
	Zoekboom<Sleutel,Data>* found = zoekTopDown(sleutel, this);
	Zoekboom<Sleutel,Data>* leftSubtree = new Zoekboom<Sleutel,Data>(found->get()->links.release());
	if (leftSubtree && *leftSubtree)
		leftSubtree->get()->ouder = 0;
	Zoekboom<Sleutel,Data>* rightSubtree = new Zoekboom<Sleutel,Data>(found->get()->rechts.release());
	if (rightSubtree && *rightSubtree)
		rightSubtree->get()->ouder = 0;

	Zoekboom<Sleutel,Data>* mostRightNode = leftSubtree;	
	while (mostRightNode->get()->rechts) {
		mostRightNode = &mostRightNode->get()->rechts;
	}
	zoek(mostRightNode->get()->sleutel, leftSubtree); //splay leftSubtree

	leftSubtree->get()->rechts.swap(*rightSubtree);

	this->swap(*leftSubtree);
};


#endif
