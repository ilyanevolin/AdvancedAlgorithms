// Labo05.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Boom2D.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

void t1(),t2(),t3(), opl(string file, bool verbose);
int _tmain(int argc, _TCHAR* argv[]) {
	t2();
	//t3();
	opl("puntenverz", false); 
	//opl("puntenverz1", true); 
	opl("punten_test_Tomes.txt", false); 
	return 0;
}

bool sortX(punt2* a, punt2* b) { return a->x < b->x; }
bool sortY(punt2* a, punt2* b) { return a->y < b->y; }

struct Range {
	Range(int l, int r) {
		left=l;
		right = r;
		int dx = (right-left);
		median = dx;
		median = left+floor(median/2);
	}
	int left, right, median;
};

/*
	Bedoeling is om een balanced KD tree op te stellen:
		twee arrays die alle waarden gesorteed bevatten (elk gesorteerd op een andere dimensie)
		we nemen telkens de mediaan van een sub-bereik 

*/
void opl(string file, bool verbose = false) {
	Boom2D* b = new Boom2D();
	std::ifstream infile(file);	
	int x, y;	
	while (infile >> x >> y) {
		punt2* punt = new punt2();
		punt->x = x; punt->y = y;			
		b->voegToe(punt);
	}
	//if (verbose) b->schrijf();
	infile.close();
	infile.clear();
	cout << endl<< "unbalanced height: " << b->hoogte() << " (#punten: " << b->punten << ")" <<  endl << endl;

	vector<punt2*> xsorted;
	vector<punt2*> ysorted;

	infile.open(file);
	while (infile >> x >> y) {
		punt2* punt = new punt2();
		punt->x = x; punt->y = y;		
		xsorted.push_back(punt);
		punt = new punt2();
		punt->x = x; punt->y = y;	
		ysorted.push_back(punt);
	}
	std::sort(xsorted.begin(), xsorted.end(), sortX);
	std::sort(ysorted.begin(), ysorted.end(), sortY);

	b = new Boom2D();
	int n = xsorted.size();	
	bool xSort = true;
	queue<Range> medians;
	vector<int> dupsX(40000);
	vector<int> dupsY(40000);
	int median;
	medians.push(Range(0,n-1)); //subregion 0->n  ; mediaan is midden
	int h = 0, addedLevel = 0;
	while (!medians.empty()) {
		Range r = medians.front(); 
		median = r.median;
		medians.pop();
		if (xSort ? dupsX[median] == 1 : dupsY[median] == 1)
				continue;
		
		bool added = b->voegToe(xSort ? xsorted[median] : ysorted[median]); //add median from a certain dimension
		if (added && verbose)
			cout << median << "\t" << *(xSort ? xsorted[median] : ysorted[median])<< endl;
		(xSort ? dupsX[median] : dupsY[median]) = 1;
		
		if (r.median+1 <= n)
			medians.push(Range(r.left, r.median+1)); //add median for left subregion
		if (r.median-1 >= 0)
			medians.push(Range(r.left, r.median-1)); //add median for left subregion
		medians.push(Range(r.left, r.median)); //add median for left subregion
		
		if (r.right+1 <= n)
			medians.push(Range(r.median, r.right+1)); //add median for right subregion		
		if (r.right-1 >= 0)
			medians.push(Range(r.median, r.right-1)); //add median for right subregion
		medians.push(Range(r.median, r.right)); //add median for right subregion

		if (pow(2,h) == ++addedLevel) {				
			++h;				//calculate height of tree
			addedLevel = 0;
			xSort = !xSort;				 //switch dimension on each level of tree
		}							
	}

	ofstream myfile;
	myfile.open ("output_" + file + ".txt");
	for (int x = 0; x <= 190; x+=10)
		for (int y = 0; y <= 190; y+=10) {
			punt2* q = new punt2();
			q->x = x; q->y = y;
			closestToObj nns = b->closestTo(q);			
			myfile  << *q << " heeft buur "<< *(nns.found) << "  gevonden in "<< nns.aantalKnopen <<" doorzochte knopen" << endl;
		}
	myfile.close();
	if (verbose) b->schrijf();
	cout << endl<< "balanced height: " <<b->hoogte() << " (#punten: " << b->punten << ")" <<  endl << endl;
}

void t2() {
	std::ifstream infile("puntenverz");
	
	int x, y;
	Boom2D* b = new Boom2D();
	while (infile >> x >> y) {
		punt2* punt = new punt2();
		punt->x = x; punt->y = y;
		b->voegToe(punt);
	}

	ofstream myfile;
	myfile.open ("output.txt");

	for (x = 0; x <= 190; x+=10)
		for (y = 0; y <= 190; y+=10) {
			punt2* q = new punt2();
			q->x = x; q->y = y;
			closestToObj nns = b->closestTo(q);			
			myfile  << *q << " heeft buur "<< *(nns.found) << "  gevonden in "<< nns.aantalKnopen <<" doorzochte knopen" << endl;
		}
	myfile.close();

	cout << "unbalanced height: " <<b->hoogte() << " (#punten: " << b->punten << ")" <<  endl;
}
void t1() {
	punt2* punt = new punt2();
	punt->x = 2; punt->y = 3;
	Boom2D* b = new Boom2D(punt);

	punt = new punt2();
	punt->x = 5; punt->y = 4;
	b->voegToe(punt);

	punt = new punt2();
	punt->x = 9; punt->y = 6;
	b->voegToe(punt);

	punt = new punt2();
	punt->x = 4; punt->y = 7;
	b->voegToe(punt);

	punt = new punt2();
	punt->x = 8; punt->y = 1;
	b->voegToe(punt);

	punt = new punt2();
	punt->x = 7; punt->y = 2;
	b->voegToe(punt);

	b->schrijf();
	
	cout << endl;
	cout << b->bestaat(punt) << endl;


	punt = new punt2();
	punt->x = 1; punt->y = 9;
	closestToObj nns = b->closestTo(punt);
	cout << "closest to "<< *punt <<" is " << *(nns.found) << " en "<<nns.aantalKnopen<<" knopen bezocht.";

	cout << endl << endl;
}

