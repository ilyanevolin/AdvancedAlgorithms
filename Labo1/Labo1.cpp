// Labo1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zoekboom11.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Zoekboom<int,int>*  zb = new Zoekboom<int,int>();
	zb->voegtoe(5,1);
	zb->voegtoe(3,1);
	zb->voegtoe(8,1);
	zb->voegtoe(1,1);
	zb->voegtoe(4,1);
	zb->voegtoe(6,1);
	zb->voegtoe(9,1);
	zb->voegtoe(10,1);

	/*zb->voegtoe(17,1);
	zb->voegtoe(15,1);
	zb->voegtoe(20,1);
	zb->voegtoe(10,1);
	zb->voegtoe(16,1);
	zb->voegtoe(18,1);
	zb->voegtoe(30,1);
	zb->voegtoe(5,1);*/

	cout << "Dit is onze boomstructuur:\n";
	zb->schrijf(); // A
	
	cout << "knopen:" << zb->knopen() << endl;
	cout << "diepte: " << zb->diepte() << "\n";
	cout << "gem. knoopdiepte: " << zb->gem_knoopdiepte() << "\n";
	cout <<"\n////////////////////////////////////////\n\n";
	
	cout << "We roteren root naar rechts:\n";
	zb->roteer(true);
	zb->schrijf(true); // B
	cout <<"\n////////////////////////////////////////\n\n";

	cout << "We roteren root naar links:\n";
	zb->roteer(false);
	zb->schrijf(true); // C moet gelijk zijn aan A   (A -> B -> A==C)
	cout <<"\n////////////////////////////////////////\n\n";

	cout << "We maken onevenwichtig:\n";
	zb->onevenwichtig();
	zb->schrijf();

	cout << "knopen:" << zb->knopen() << endl;
	cout << "diepte: " << zb->diepte() << "\n";
	cout << "gem. knoopdiepte: " << zb->gem_knoopdiepte() << "\n";
	if (zb->diepte() != zb->knopen()) cout << "ER IS EEN FOUT OPGETREDEN #44"; else cout << "ALLES INORDE!";
	cout <<"\n////////////////////////////////////////\n\n";

	/////////////////////
	cout << "We maken terug evenwichtig:\n";
	zb->evenwichtig();
	zb->schrijf();

	cout << "knopen:" << zb->knopen() << endl;
	cout << "diepte: " << zb->diepte() << "\n";
	cout << "gem. knoopdiepte: " << zb->gem_knoopdiepte() << "\n";
	cout <<"\n////////////////////////////////////////\n\n";

	return 0;
}