#include "punt2.h";
#include <stack>
using namespace std;

struct closestToObj;
class Boom2D {
	public:
		Boom2D();
		Boom2D(punt2* punt);
		bool voegToe(punt2*);
		bool voegToe_deprecated(punt2*);
		void schrijf();		
		bool bestaat(punt2*);
		closestToObj closestTo(punt2* query);
		int hoogte();
		int punten;
	private:
		int hoogte(punt2* node);
		void init(punt2* punt);
		void closestToHulp(punt2* query, punt2* curr, punt2*& bestGuess, int& bestDist, int&);
		punt2* zoek(punt2*);
		punt2* root;
};

/*
	punt --> root = circle
	(punt.y + root.y == range to search in; all nodes in this range must be inspected; the closest node will shrink new circle)
	simpele methode
*/
Boom2D::Boom2D() {
	root = nullptr;
	punten = 0;
}
void Boom2D::init(punt2* punt) {
	punt->puntR = punt->puntL = 0;
	punt->Xsplit = 1;
	punt->puntParent = 0;
	root = punt;
}
Boom2D::Boom2D(punt2* punt) {
	punten = 0;
	init(punt);
}

struct closestToObj {
	punt2* found;
	int aantalKnopen;
};
closestToObj Boom2D::closestTo(punt2* query) {
	punt2* found = 0;
	int aantalKnopenBezocht = 0;
	int bestDist = 0;
	closestToHulp(query, root, found, bestDist, aantalKnopenBezocht);

	closestToObj ret;
	ret.found = found;
	ret.aantalKnopen = aantalKnopenBezocht;
	return ret;
}
void Boom2D::closestToHulp(punt2* query, punt2* curr, punt2*& bestGuess, int& bestDist, int& aantalKnopenBezocht) {
	if (curr == nullptr)
		return;

	++aantalKnopenBezocht;
	if (bestGuess == 0) {		
		bestGuess = curr;		
	} else {
		int curToQ = curr->kwadafstand(*query);
		int bgToQ = bestGuess->kwadafstand(*query);
		if (curToQ < bgToQ) {
			bestGuess = curr;
			bestDist = curToQ;
		}
	}

	if (curr->Xsplit ? curr->x > query->x : curr->y > query->y)
		closestToHulp(query, curr->puntL, bestGuess,bestDist,aantalKnopenBezocht);
	else
		closestToHulp(query, curr->puntR, bestGuess,bestDist,aantalKnopenBezocht);

	if ((curr->Xsplit ? abs(query->x - curr->x) : abs(query->y - curr->y) ) < std::sqrt(bestDist) )
		if (curr->Xsplit ? curr->x > query->x : curr->y > query->y)
			closestToHulp(query, curr->puntR, bestGuess,bestDist,aantalKnopenBezocht);
		else
			closestToHulp(query, curr->puntL, bestGuess,bestDist,aantalKnopenBezocht);
}

void Boom2D::schrijf() {
	std::stack<punt2*> s;
	s.push(root);
	while (!s.empty()) {
		punt2* loper = s.top();
		s.pop();
		if (loper->puntParent)
			cout << *(loper->puntParent) << "--"<< (loper->puntParent->puntL == loper ? "L" : "R") <<"--> ";
		cout << *loper << "   ";
		if (loper->puntL != 0) {
			s.push(loper->puntL);
		} 
		if (loper->puntR != 0) {
			s.push(loper->puntR);
		} 
	}
}
int Boom2D::hoogte() {
	return hoogte(root);
}
int Boom2D::hoogte(punt2* node) {
	if (!node) return 0;
	int l = hoogte(node->puntL);
	int r = hoogte(node->puntR);
	return 1 + (l > r? l : r);
}

bool Boom2D::bestaat(punt2* punt) {
	punt2* loper = zoek(punt);
	return (loper->x == punt->x && loper->y == punt->y);
}
punt2* Boom2D::zoek(punt2* punt) {
	punt2* loper = root;
	while (true) {
		if (loper->x == punt->x && loper->y == punt->y) {
			return loper;
		} else if (loper->Xsplit ? loper->x > punt->x : loper->y > punt->y) {
			if (loper->puntL != 0) {
				loper = loper->puntL; //return exact found node
			} else {
				return loper; //return parent
			}
		} else {
			if (loper->puntR != 0) {
				loper = loper->puntR;
			} else {
				return loper;
			}
		}
	}
	return 0;
}

bool Boom2D::voegToe(punt2* nieuw) {
	return true;
}
bool Boom2D::voegToe_deprecated(punt2* nieuw) {
	/*
		deze functie is niet correct omdat inwendige knopen geen data mogen bevatten
		maar enkel dienen om dimensie aan te duiden (of te berekenen) en de split-value ervan (waarde van x of y, ...)
		Als we iets moeten toevoegen onder een blad, dan zullen we een nieuwe inwendige knoop moeten maken met de twee kinderen/bladeren.
	*/
	if (root == nullptr) {
		++punten;
		init(nieuw);
		return 1;
	}

	nieuw->puntR = nieuw->puntL = 0;
	punt2* loper = zoek(nieuw);
	if (loper->x == nieuw->x && loper->y == nieuw->y) {
		return 0;//no duplicates
	} else if (loper->Xsplit ? loper->x > nieuw->x : loper->y > nieuw->y) {
		loper->puntL = nieuw;
		nieuw->Xsplit = !loper->Xsplit;
		nieuw->puntParent = loper;
	} else {
		loper->puntR = nieuw;
		nieuw->Xsplit = !loper->Xsplit;
		nieuw->puntParent = loper;
	}
	++punten;
	return 1;
}

