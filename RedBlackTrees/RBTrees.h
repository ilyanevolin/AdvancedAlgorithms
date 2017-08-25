#ifndef __RBTree_H
#define __RBTree_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>

#include <queue>
#include <string>
#include <sstream>
using namespace std;

template <class Sleutel,class Data>
class Node;
template <class Sleutel,class Data>
class NullNode;

template <class Sleutel,class Data>
class RBTree : public unique_ptr<Node<Sleutel,Data>>{
	public:
		RBTree() : n(0) { }
		RBTree(Node<Sleutel,Data>* el) : std::unique_ptr<Node<Sleutel,Data>>(el)  {
			el->links.reset(  new NullNode<Sleutel,Data>() );
			el->rechts.reset(  new NullNode<Sleutel,Data>() ); 
		}
		void voegtoe_bottomUp(const Sleutel& sleutel,const Data& data);
		void verwijder_bottomUp(const Sleutel& sleutel);
		void voegtoe_topDown(const Sleutel& sleutel,const Data& data);
		void verwijder_topDown(const Sleutel& sleutel);
		void schrijf(bool simple = false);
		int diepte();
		const int knopen() {return n;}
		void roteer(bool right); //true = van links->rechts
		int telKnopen(RBTree<Sleutel,Data>* zb);	
	protected:
		void zoek(const Sleutel& sleutel, RBTree<Sleutel,Data>*& gparent, RBTree<Sleutel,Data>*& parent, RBTree<Sleutel,Data>*& plaats);			
		int n;
};

template <class Sleutel,class Data>
class NullNode : public Node<Sleutel, Data> {
	public:
		NullNode() {isBlack = true; data=NULL;}
};

template <class Sleutel,class Data>
class Node {
	friend class RBTree<Sleutel,Data>;		
	public:
		Node(Sleutel sl,Data dt) : data(dt),sleutel(sl),isBlack(true) { }
		bool isBlack;    		
		friend std::ostream& operator<<(std::ostream& os, const Node<Sleutel,Data> &node){
			return os<<"("<<node.sleutel<<")";
		};
    protected:
        Sleutel sleutel;
        Data data;
        Node<Sleutel,Data>* ouder;
        RBTree<Sleutel,Data> links, rechts;
        Node() : ouder(0){}

};

template <class Sleutel,class Data>
void RBTree<Sleutel,Data>::roteer(bool right) {	
	RBTree<Sleutel,Data> *p;			
	// 1)
	p = &(right ? this->get()->links : this->get()->rechts);					
	Node<Sleutel,Data> *ouder = (this->get()->ouder);
	this->swap(*p);
	if (this && *this && (this->get()->data)!=NULL)
		this->get()->ouder = std::move(ouder);
	else 
		return;
	p->get()->ouder = this->get();
	// 2)
	RBTree<Sleutel,Data>* beta_to = &(right ? p->get()->links : p->get()->rechts);	
	if (beta_to && *beta_to && (beta_to->get()->data)!=NULL) {
		RBTree<Sleutel,Data>* beta = &(right ? this->get()->rechts : this->get()->links);
		if (beta && *beta && (beta->get()->data)!=NULL)
			beta->get()->ouder = p->get();
		beta_to->swap(*beta);
	}
}

template <class Sleutel,class Data>
void RBTree<Sleutel,Data>::voegtoe_bottomUp(const Sleutel& sleutel,const Data& data){
	cout << "adding: " << sleutel << endl;
    RBTree<Sleutel,Data>* plaats;
    RBTree<Sleutel,Data>* parent = nullptr;
	RBTree<Sleutel,Data>* gparent = nullptr;
	
    zoek(sleutel, gparent, parent, plaats);
    if (plaats->get() == nullptr || (plaats->get()->data)==NULL ){
        RBTree<Sleutel,Data> nieuw( new Node<Sleutel,Data>(sleutel,data) );
        nieuw.get()->ouder = parent ? parent->get() : nullptr;
		plaats->swap(nieuw); //swap		
		this->n++;

		plaats->get()->isBlack = false;
		if (parent==0 || (parent->get()->data)==NULL) return;
		bool doubleRed = (parent->get()->isBlack == false && plaats->get()->isBlack == false);		
		while (gparent/*gparent && doubleRed*/) { // als parent ook rood is, moeten we situatie oplossen
			if (gparent && doubleRed && gparent->get()->isBlack &&
				gparent->get()->links.get()->isBlack == false &&
				gparent->get()->rechts.get()->isBlack == false)
			{ //figuur 1.3
				gparent->get()->isBlack = false; //de grootouder is zwart, parent & parent-broer zijn rood => promotion
				gparent->get()->links.get()->isBlack = true;
				gparent->get()->rechts.get()->isBlack = true;
			} else if (gparent && doubleRed) { //figuur 1.4 & 1.5 
				bool ouderLinks = (parent == &gparent->get()->links); // ligt de parent links van grootouder?
				bool kindLinks = (plaats == &parent->get()->links); // ligt de kind links van parent?
				if (ouderLinks != kindLinks) { //figuur 1.5 : kind & parent liggen niet op 1 lijn
					if (ouderLinks)
						gparent->get()->links.roteer(false); //roteer kind naar links
					else
						gparent->get()->rechts.roteer(true);
				}
				// figuur 1.4 : kind & parent liggen op 1 lijn
				gparent->get()->isBlack = false; //grandparent goes red
				parent->get()->isBlack = true; // X goes black
				if (gparent) {
					if (&gparent->get()->links == gparent)
						gparent->roteer(ouderLinks);
					else
						gparent->roteer(ouderLinks);
				} else if (gparent == this) {
					this->roteer(ouderLinks);
					parent = this;
					if (kindLinks) {						
						plaats = &parent->get()->links;
					} else {
						plaats = &parent->get()->rechts;
					}
				}
			}
			zoek(parent->get()->sleutel, gparent, parent, plaats);
			if (parent)
				doubleRed = parent->get()->isBlack == false && plaats->get()->isBlack == false;			
			else
				doubleRed = 0;
		}
		this->get()->isBlack = true; //eventueel wortel zwart maken
    };
    
};

template <class Sleutel,class Data>
void RBTree<Sleutel,Data>::verwijder_bottomUp(const Sleutel& sleutel) {
	cout << "removing: " << sleutel << endl;
	RBTree<Sleutel,Data>* plaats;
    RBTree<Sleutel,Data>* parent = nullptr;
	RBTree<Sleutel,Data>* gparent = nullptr;
	
	RBTree<Sleutel,Data>* DBblack = nullptr;

    zoek(sleutel, gparent, parent, plaats);
	if (plaats->get()->links->data == NULL && plaats->get()->rechts->data != NULL) { // a node with only right child
		Node<Sleutel,Data>* ouder = plaats->get()->ouder;
		plaats->reset( plaats->get()->rechts.release() ); //replace by right child
		plaats->get()->ouder = ouder; // adjust parent
	} else {
		RBTree<Sleutel,Data>* loper = &plaats->get()->links;
		while (loper && loper->get()->rechts.get()->data != NULL) {
			loper = &loper->get()->rechts;
		}				
		Node<Sleutel,Data>* plaats_links = loper == &plaats->get()->links ? new NullNode<Sleutel,Data>() : plaats->get()->links.release();
		Node<Sleutel,Data>* plaats_rechts = plaats->get()->rechts.release();
		plaats->reset( loper->release() );
		plaats->get()->links.reset(plaats_links);
		plaats->get()->rechts.reset(plaats_rechts);
		plaats_rechts->ouder = plaats->get();
		plaats->get()->ouder = parent ? parent->get() : nullptr;
	}
	bool deleted_node_isBlack = plaats->get()->isBlack;
	if (!deleted_node_isBlack)
		plaats->get()->isBlack = true;//?????????
	else
		DBblack = plaats;
	
	while (DBblack) {
		// ... http://www.slideshare.net/piotrszymanski/red-black-trees (p21)
		// 4 symmetric cases ; plaats is a new node
		if (	parent && parent->get()->isBlack &&
				&parent->get()->links == plaats && 
				parent->get()->rechts->isBlack == false) { //case 1a (fig 1.9)
			parent->roteer(false);
			parent->get()->isBlack = true;
			parent->get()->links.get()->isBlack = false;
			parent = &parent->get()->links;
			plaats = &parent->get()->links;
			DBblack = plaats;
			//push up black
		} else if (	parent && parent->get()->isBlack &&
					&parent->get()->rechts == plaats &&
					parent->get()->links->isBlack == false) {  //case 1b (fig 1.9)
			parent->roteer(true);
			parent->get()->isBlack = true;
			parent->get()->links.get()->isBlack = false;
			plaats = &parent->get()->rechts;
			parent = plaats;
			DBblack = plaats;
			//push up black
		} else if (	parent && &parent->get()->links == plaats &&
					parent->get()->rechts->isBlack &&
					parent->get()->rechts.get()->links.get()->isBlack &&
					parent->get()->rechts.get()->rechts.get()->isBlack		) {	//case 2a (fig 1.6)
			parent->get()->rechts->isBlack = false;
			if (!parent->get()->isBlack) {
				parent->get()->isBlack = true; //rode ouder wordt zwart
				DBblack = nullptr; //all good
			} else // zwarte ouder wordt dubbel zwart
				DBblack = parent;
			zoek(parent->get()->sleutel, gparent, parent, plaats);
		} else if (	parent && &parent->get()->rechts == plaats &&
					parent->get()->links->isBlack &&
					parent->get()->links.get()->links.get()->isBlack &&
					parent->get()->links.get()->rechts.get()->isBlack		) { //case 2b (fig 1.6)
			parent->get()->links->isBlack = false;
			if (!parent->get()->isBlack) {
				parent->get()->isBlack = true; //rode ouder wordt zwart
				DBblack = nullptr; //all good
			} else // zwarte ouder wordt dubbel zwart
				DBblack = parent;
			zoek(parent->get()->sleutel, gparent, parent, plaats);
		} else if (	parent && &parent->get()->links == plaats &&
					parent->get()->rechts->isBlack &&
					!parent->get()->rechts.get()->rechts.get()->isBlack		) {	//case 3a (fig 1.7)
			parent->roteer(false);
			bool P_color = plaats->get()->isBlack;
			plaats->get()->isBlack = parent->get()->isBlack; //it should swap with sibling and parent; not V !!!!
			parent->get()->isBlack = P_color;
			parent->get()->rechts.get()->isBlack = true;
			DBblack = nullptr; //terminal case
		} else if ( parent && &parent->get()->rechts == plaats &&
					parent->get()->links->isBlack &&
					!parent->get()->links.get()->links.get()->isBlack		) {	//case 3b (fig 1.7)
			parent->roteer(true);
			bool P_color = plaats->get()->isBlack;
			plaats->get()->isBlack = parent->get()->isBlack;
			parent->get()->isBlack = P_color;
			parent->get()->links.get()->isBlack = true;
			DBblack = nullptr; //terminal case
		} else if (	parent && &parent->get()->links == plaats &&
					parent->get()->rechts->isBlack &&
					parent->get()->rechts.get()->rechts.get()->isBlack &&
					!parent->get()->rechts.get()->links.get()->isBlack		) {	//case 4a (fig 1.8)
			bool S_color = parent->get()->rechts.get()->isBlack;				
			parent->get()->rechts.get()->isBlack = parent->get()->rechts.get()->links.get()->isBlack;
			parent->get()->rechts.get()->links.get()->isBlack = S_color;
			parent->get()->rechts.roteer(true);				
		} else if (	parent && &parent->get()->rechts == plaats &&
					parent->get()->links->isBlack &&
					parent->get()->links.get()->links.get()->isBlack &&
					!parent->get()->links.get()->links.get()->isBlack		) {	//case 4b (fig 1.8)
			bool S_color = parent->get()->links.get()->isBlack;				
			parent->get()->links.get()->isBlack = parent->get()->links.get()->rechts.get()->isBlack;
			parent->get()->links.get()->rechts.get()->isBlack = S_color;
			parent->get()->links.roteer(true);				
		} else if ( parent && !parent->get()->isBlack) {
			DBblack = nullptr;//if parent is red, it kills double blackness
		}
	}

	this->get()->isBlack = true;//root goes black (or absorbs double black)
}

template <class Sleutel,class Data>
void RBTree<Sleutel,Data>::voegtoe_topDown(const Sleutel& sleutel,const Data& data){
    cout << "adding: " << sleutel << endl;
	RBTree<Sleutel,Data>* plaats = this;
	RBTree<Sleutel,Data>* parent = nullptr;
	RBTree<Sleutel,Data>* gparent = nullptr;
	RBTree<Sleutel,Data>* ogparent = nullptr;
    
	while (*plaats && plaats->get()->data != NULL /*&& (*plaats)->sleutel != sleutel*/) {		
		if (parent && parent->get()->links.get()->isBlack && parent->get()->rechts.get()->isBlack ) {			
			cout << ""; //no problem
		} else if (	parent && parent->get()->isBlack &&
					(	(!parent->get()->links.get()->isBlack && parent->get()->rechts.get()->isBlack) ||
						(!parent->get()->rechts.get()->isBlack && parent->get()->links.get()->isBlack)		)
					)
		{
			if (	(parent->get()->rechts.get()->isBlack && &parent->get()->rechts == plaats) ||
					(parent->get()->links.get()->isBlack && &parent->get()->links == plaats)    )
			{				
				cout << ""; //no problem
			} else if (	(sleutel < plaats->get()->sleutel && plaats->get()->links.get()->data == NULL) ||
						 (sleutel >= plaats->get()->sleutel && plaats->get()->rechts.get()->data == NULL))
			{
				//do rotation & recoloring (only if we have reached the leaf node
				bool kindLinks = (plaats->get() == parent->get()->links.get()); 
				parent->roteer(kindLinks);
				parent->get()->isBlack = true;
				parent->get()->links.get()->isBlack = false;
				parent->get()->rechts.get()->isBlack = false;
				plaats = parent;
	
			}
		} else if (	parent &&
					parent->get()->isBlack &&
					!parent->get()->links.get()->isBlack &&
					!parent->get()->rechts.get()->isBlack	)
		{
			//this situation must be avoided
			if (!gparent || gparent->get()->isBlack) { //case 1
				parent->get()->isBlack = false;
				parent->get()->links.get()->isBlack = true;
				parent->get()->rechts.get()->isBlack = true;
			} else if (		gparent && !gparent->get()->isBlack &&
						(	&ogparent->get()->links == gparent && &gparent->get()->links == parent ||
							&ogparent->get()->rechts == gparent && &gparent->get()->rechts == parent)	)
			{ // case 2
				bool gparentLeft = &ogparent->get()->links == gparent; //bools are both equal (true or false)
				bool parentLeft = &gparent->get()->links == parent;
				ogparent->roteer(gparentLeft);

				parent->get()->isBlack = false;
				parent->get()->links.get()->isBlack = true;
				parent->get()->rechts.get()->isBlack = true;
				ogparent->get()->isBlack = true;
				ogparent->get()->links.get()->isBlack = false;
				ogparent->get()->rechts.get()->isBlack = false;
			} else if (		gparent && !gparent->get()->isBlack &&
						(	&ogparent->get()->links == gparent && &gparent->get()->rechts == parent ||
							&ogparent->get()->rechts == gparent && &gparent->get()->links == parent)		)
			{ // case 3
				bool gparentLeft = &ogparent->get()->links == gparent; //bools are both equal (true or false)
				bool parentLeft = &gparent->get()->links == parent;				

				parent->get()->isBlack = false;
				parent->get()->links.get()->isBlack = true;
				parent->get()->rechts.get()->isBlack = true;
				gparent->roteer(parentLeft);

				gparent->roteer(gparentLeft);
				ogparent->get()->isBlack = true;
				ogparent->get()->links.get()->isBlack = false;
				ogparent->get()->rechts.get()->isBlack = false;
			}
		}		

		ogparent = gparent;
		gparent = parent;
		parent = plaats;
		if ((*plaats)->sleutel <= sleutel)
			plaats=&(*plaats)->rechts;
		else
			plaats=&(*plaats)->links;
		
	}	  

	// plaats points to position to add new key
	if (parent && *parent && !parent->get()->isBlack) {
		bool ouderLinks = (parent->get() == gparent->get()->links.get()); 
		gparent->roteer(ouderLinks);
		gparent->get()->isBlack = true;
		gparent->get()->links.get()->isBlack = false;
		gparent->get()->rechts.get()->isBlack = false;
		parent = gparent;
	}
	RBTree<Sleutel,Data> nieuw( new Node<Sleutel,Data>(sleutel,data) );
    nieuw->ouder = parent ? parent->get() : nullptr; //gparent is the previous parent
	plaats->reset(nieuw.release()); //swap
	plaats->get()->isBlack = false; //red initially
	
	
	this->get()->isBlack = true; //eventueel wortel zwart maken
	this->n++;
};

template <class Sleutel,class Data>
void RBTree<Sleutel,Data>::verwijder_topDown(const Sleutel& sleutel) {
	//if we delete a red leaf, then no problem
	//so the idea is to change every node we encounter to red, and fix any violations		

	RBTree<Sleutel,Data>* plaats = nullptr;
	RBTree<Sleutel,Data>* plaats_broer = nullptr;
	bool plaats_broer_links = false;
	RBTree<Sleutel,Data>* parent = nullptr;

	//step 1: examine the root
	if (this->get()->links->isBlack && this->get()->rechts->isBlack) {
		this->get()->isBlack = false;
		parent = this;
		if ((*this)->sleutel < sleutel) {
			plaats_broer_links = true;
			plaats_broer=&(*this)->links;
			plaats=&(*this)->rechts;			
		} else {
			plaats_broer_links = false;
			plaats_broer=&(*this)->rechts;
			plaats=&(*this)->links;			
		} //proceed to step 2
	} else {
		plaats = this;
		//proceed to step 2B
	}
	
	while (plaats->get()->data != NULL) {
		plaats->get()->isBlack = false; //color X red

		// case 2A: plaats has 2 black children
		if (plaats->get()->links.get()->isBlack && plaats->get()->rechts.get()->isBlack) {
			// case 2A.1 : broer has 2 black children
			if (plaats_broer->get()->links.get()->isBlack && plaats_broer->get()->rechts.get()->isBlack) {
				plaats->get()->isBlack = false; //recolor 3 nodes
				plaats_broer->get()->isBlack = false;
				parent->get()->isBlack = true;				
			}
			// case 2A.2 : broer's inner child is red
			else if ((plaats_broer_links  && !plaats_broer->get()->rechts.get()->isBlack && plaats_broer->get()->links.get()->isBlack) ||
					 (!plaats_broer_links && !plaats_broer->get()->links.get()->isBlack  && plaats_broer->get()->rechts.get()->isBlack))
			{ // dia 62
				plaats_broer->roteer(!plaats_broer_links);
				parent->roteer(plaats_broer_links);
				//we have to re-color X and P, then move down the tree
				if (plaats_broer_links)
					parent = &parent->get()->rechts;
				else
					parent = &parent->get()->links;
				parent->get()->isBlack = true;
				plaats->get()->isBlack = false;
			}
			// case 2A.3 : broer's outer child is red (or if both children are red)
			else { // dia 63
				parent->roteer(plaats_broer_links);
				parent->get()->isBlack = false;
				parent->get()->links.get()->isBlack = true;
				parent->get()->rechts.get()->isBlack = true;				
				if (plaats_broer_links) {
					parent = &parent->get()->rechts;
				} else {
					parent = &parent->get()->links;
				}

				plaats->get()->isBlack = false;
			}

			parent = plaats;// move down the tree
			if ((*plaats)->sleutel <= sleutel) {
				plaats_broer_links = true;
				plaats_broer=&(*plaats)->links;
				plaats=&(*plaats)->rechts;				
			} else {
				plaats_broer_links = false;
				plaats_broer=&(*plaats)->rechts;
				plaats=&(*plaats)->links;
			}
		}
		// case 2B : plaats has at least 1 red child
		else {

			parent = plaats;// move down the tree
			if ((*plaats)->sleutel <= sleutel) {
				plaats_broer_links = true;
				plaats_broer=&(*plaats)->links;
				plaats=&(*plaats)->rechts;				
			} else {
				plaats_broer_links = false;
				plaats_broer=&(*plaats)->rechts;
				plaats=&(*plaats)->links;					
			}

			if (!plaats->get()->isBlack) {
				//go down once more
				parent = plaats;
				if ((*plaats)->sleutel <= sleutel) {
					plaats_broer_links = true;
					plaats_broer=&(*plaats)->links;
					plaats=&(*plaats)->rechts;				
				} else {
					plaats_broer_links = false;
					plaats_broer=&(*plaats)->rechts;
					plaats=&(*plaats)->links;					
				}
			} else if (plaats && plaats->get()->isBlack && plaats->get()->data != NULL) {
				bool broerLinksVanParent = &parent->get()->links == plaats_broer ? 1 : 0;
				parent->roteer(broerLinksVanParent);
				if (broerLinksVanParent) {			
					cout << "";
					// plaats_broer = ... ; parent = ...
				} else {
					cout << "";
					// plaats_broer = ... ; parent = ...
				}
			} else {
				break;
			}


		}
	} 

	RBTree<Sleutel,Data>* toDelete = nullptr;
	if (parent->get()->sleutel == sleutel)
		toDelete = parent;
	else
		toDelete = plaats;
	cout << "we should delete: " << *(toDelete->get()) << endl; //correct? (or plaats?)
	
	RBTree<Sleutel,Data>* replace = new RBTree<Sleutel,Data>(new NullNode<Sleutel,Data>());
	toDelete->swap(*replace);
	
	this->get()->isBlack = true;//color root black
	
}

template <class Sleutel,class Data>
void RBTree<Sleutel,Data>::zoek(const Sleutel& sleutel, RBTree<Sleutel,Data>*& gparent, RBTree<Sleutel,Data>*& parent, RBTree<Sleutel,Data>*& plaats){
    plaats=this;
	gparent = nullptr;
	parent = nullptr;
    while (plaats && *plaats && (plaats->get()->data)!=NULL && (*plaats)->sleutel !=sleutel){
        gparent = parent;
		parent = plaats;
		if ((*plaats)->sleutel <= sleutel) {			
            plaats=&(*plaats)->rechts;
		} else {
            plaats=&(*plaats)->links;
		}
		if (!*plaats)
			return;
    };
};

template <class Sleutel,class Data>
void RBTree<Sleutel, Data>::schrijf(bool simple = false) {
	if (!this->get()) return;
	std::stringstream s1, s2;
	queue<RBTree<Sleutel,Data>*> q;
	q.push(this);

	while (!q.empty()) {
		RBTree<Sleutel,Data>* z = q.front();
		q.pop();
		if (!z->get() || z->get()->data == NULL)
			continue;
		
		s1 << z->get()->sleutel << " (" << (z->get()->isBlack ? "Z":"R") << ")\t" ;

		s2 << z->get()->sleutel << " (" << (z->get()->isBlack ? "Z":"R")<<")";
		s2 << "\t  parent: ";
		if (z->get()->ouder != nullptr) s2 <<  z->get()->ouder->sleutel; else s2<<" ";
		s2 << "\t  links: ";
		if (z->get()->links != nullptr && (NULL!=(z->get()->links.get()->data)))
			s2 <<  z->get()->links->sleutel; else s2<<" ";
		s2 << "\t  rechts: ";
		if (z->get()->rechts != nullptr && (NULL!=(z->get()->rechts.get()->data)))
			s2 <<  z->get()->rechts->sleutel; else s2<<" ";
		s2 << endl;

		if (NULL!=(z->get()->links.get()->data))
			q.push(&(z->get()->links));
		if (NULL!=(z->get()->rechts.get()->data))
			q.push(&(z->get()->rechts));
	}
	if (!simple)
		cout << s2.str();
	cout << "levelorder:  " << s1.str() << "\n";
	cout << "\n\n";
}

template <class Sleutel,class Data>
int RBTree<Sleutel, Data>::diepte() {
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
int RBTree<Sleutel, Data>::telKnopen(RBTree<Sleutel,Data>* zb) {	
	
	int ret = 0;
	if (!zb || !*zb)
		return 0;
	
	stack<RBTree<Sleutel,Data>*> s;
	s.push(zb);
	while (!s.empty()) {
		zb = s.top();
		s.pop();
		if (zb && *zb) {
			++ret;
			if (zb->get()->links)
				s.push(&(zb->get()->links));
			if (zb->get()->rechts)
				s.push(&(zb->get()->rechts));
		}
	}
	return ret;
}

#endif
