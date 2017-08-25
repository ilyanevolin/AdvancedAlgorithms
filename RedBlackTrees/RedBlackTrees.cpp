// RedBlackTrees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RBTrees.h"
using namespace std;

void bottomUp(), topDown();
int _tmain(int argc, _TCHAR* argv[])
{
	//bottomUp();
	topDown();
	return 0;
}

void bottomUp() {
	RBTree<int,int>* root = new RBTree<int,int>();
	root->voegtoe_bottomUp(10,1);	
	root->schrijf();
	root->voegtoe_bottomUp(20,1);
	root->schrijf();	

	root->verwijder_bottomUp(10); root->schrijf(); //-->only recoloring should happen

	root->voegtoe_bottomUp(30,1);
	root->schrijf();

	//root->verwijder_bottomUp(20); root->schrijf(); //-->only recoloring should happen

	root->voegtoe_bottomUp(3,1);
	root->schrijf();
	root->voegtoe_bottomUp(15,1);
	root->schrijf();
	root->voegtoe_bottomUp(5,1);
	root->schrijf();
	root->voegtoe_bottomUp(1,1);
	root->schrijf();
	root->voegtoe_bottomUp(27,1);
	root->schrijf();
	root->voegtoe_bottomUp(22,1);
	root->schrijf();
	root->voegtoe_bottomUp(17,1);
	root->schrijf();
	root->voegtoe_bottomUp(14,1);
	root->schrijf();

	root->verwijder_bottomUp(3); root->schrijf();

	root->voegtoe_bottomUp(7,1);
	root->schrijf();
	root->voegtoe_bottomUp(9,1);
	root->schrijf();
	root->voegtoe_bottomUp(8,1);
	root->schrijf();

	//root->verwijder_bottomUp(3); root->schrijf(); // fig1.9 --> fig1.6
}

void topDown() {
	RBTree<string,int>* root = new RBTree<string,int>();	
	root->voegtoe_topDown("x",1);
	root->schrijf();

	// root->verwijder_topDown("x");root->schrijf(); //attempt delete root
	
	root->voegtoe_topDown("b",1);
	root->schrijf();
	root->voegtoe_topDown("y",1);
	root->schrijf();

	// root->verwijder_topDown("y");root->schrijf(); //attempt delete right child of root

	root->voegtoe_topDown("d",1);
	root->schrijf();
	root->voegtoe_topDown("z",1);
	root->schrijf();
	root->voegtoe_topDown("f",1);
	root->schrijf();
	root->voegtoe_topDown("y",1);
	root->schrijf();
	root->voegtoe_topDown("y",1);
	root->schrijf();
	root->voegtoe_topDown("g",1);
	root->schrijf();
	//the colors could change (be different form visualisation tool)
	//but the property of RB trees is preserved; topdown works differently from bottomUp
	root->voegtoe_topDown("w",1);
	root->schrijf();

	root->voegtoe_topDown("r",1);
	root->schrijf();
	root->voegtoe_topDown("h",1);
	root->schrijf();
	root->voegtoe_topDown("i",1);
	root->schrijf();
	root->voegtoe_topDown("zz",1);
	root->schrijf();


	//root->verwijder_topDown("b");root->schrijf(); //attempt delete black node, all to the left
	//root->verwijder_topDown("w");root->schrijf(); //attempt delete black node whose broer has inner red node
	root->verwijder_topDown("y");root->schrijf(); //attempt delete black node whose broer has outer red node
}
