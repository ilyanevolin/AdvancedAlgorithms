#ifndef __BTREE
#define __BTREE
#include "schijf.h"
#include <iostream>
#include <stack>
#include <string>
#include <queue>
using std::endl;
using std::cerr;
using std::ostream;
//Dit bestand bevat code en hoofdingen van
template<class T, class D,unsigned int m>
class Btree;
template<class T,class D,unsigned int m>
class Bknoop;
//betekenis m: zie cursus

//crashtest voorkomt dat er meer dan drie knopen in het 'geheugen' zitten.
//dit veronderstelt wel dat er nooit knopen op de Schijf gedeletete worden,
template<class T,class D,unsigned int m>
class Bknoop{
	friend class Btree<T,D,m>;
	public:
		Bknoop<T,D,m>(){
			4/(-4+ ++crashtest);
			k=0;
			for(int i = 0; i < m; i++)
				data[i] = blokindices[i] = 0;
		};
		~Bknoop<T,D,m>(){
			--crashtest;
		};

		const Bknoop<T,D,m>& operator=(const Bknoop<T,D,m>& b){
			k=b.k;
			isblad=b.isblad;
			for (unsigned int i=1;i<m;i++){
				sleutel[i]=b.sleutel[i];
				data[i]=b.data[i];
			}
			if (!isblad) {
				for (unsigned int i=0;i<m;i++){
					blokindices[i]=b.blokindices[i];
				}
			}
			return *this;
		}
		//opmerking: sleutel[0] en data[0] worden niet gebruikt, zodat indexen vanaf 1 lopen
		
		blokindex blokindices[m]; // 0-->k
		T sleutel[m]; // 1-->k
		D data[m]; // 1-->k 
		

		unsigned int k;
		bool isblad;
		void print(Schijf<Bknoop<T,D,m>>& schijf) {
			for(T t : sleutel) {
				if (strcmp(t.c_str(), "") != 0)
					cout << t << ",";
			}
			for (blokindex bi : blokindices) {				
				if (bi == 0) break;
				cout << "\t";
				Bknoop<T,D,m> tmp;
				schijf.lees(tmp, bi);
				tmp.print(schijf);				
			}
		}		

		bool detectInvalidBI() {			
			for (int i = 0; i <= k; i++)
				for (int j = i+1; j <= k; j++)
					if (blokindices[i] == blokindices[j] && blokindices[j]!=0)
						return true; // duplicate found
			
			for (int i = k; i >= 1; i--) {
				if (blokindices[i] > blokindices[i-1] && blokindices[i-1] == 0)
					return true; // invalid blokindex found (zero in middle for instance)
			}

			if (!isblad) {
			int nonEmptyBIs = 0;
			for (int i = 0; i <= k; i++)
				if (blokindices[i] > 0)
					nonEmptyBIs++; //zero for last key.
			if (nonEmptyBIs <= k) //#bi's must be greater than k
				return true;
			}
			return false;
		}

		int voegToeInArray(T key, D val, blokindex biL, blokindex biR) {	
			// Zoek waar sleutel moet koemn			
			int j = 1;
			while (j <= k && strcmp(sleutel[j].c_str(), key.c_str()) < 0) {
				j++;
			}

			k++;
			// Schuif alles erachter 1 plaats op.
			for (int v = k; v >= j+1; v--) {
				sleutel[v] = sleutel[v - 1];
				data[v] = data[v - 1];
				blokindices[v] = blokindices[v-1]; //if ! isblad
			}
			if (j == 1) {
				blokindices[1] = blokindices[0];
				blokindices[0] = 0;
			}

			// Plaats sleutel
			sleutel[j] = key;
			data[j] = val;			
			if (!isblad && biR!= 0)
				blokindices[j] = biR;
			if (!isblad && biL!= 0)
				blokindices[j-1] = biL;
			

			return j;
		}
		

	private:
		static int crashtest;
		int exists(T t) {
			for (int i = 1; i <= k; i++)
				if (strcmp(sleutel[i].c_str(),t.c_str()) == 0)
					return i;				
			return -1;
		}
};
template<class T,class D,unsigned int m>
int Bknoop<T,D,m>::crashtest=0;

template<class T, class D,unsigned int m>
class CompareKnoop {
	typedef Bknoop<T,D,m> Knoop;
	public:
		bool operator()(Knoop& a, Knoop& b) {
			return (a.data[1] > b.data[1]);
		}
};
//T is onze key (string) ; D is data (frequency van voorkomen) ==> no duplicates
template<class T, class D,unsigned int m>
class Btree{
    typedef Bknoop<T,D,m> Knoop;
	public:
		Btree(Schijf<Knoop>& s):schijf(s){
			wortel.k=0;
			wortel.isblad=true;
			wortelindex=schijf.schrijf(wortel);
			wortel.crashtest--;
		}    
		void shiftRightAdd(Knoop* addK, T &t, D &d, blokindex lt, blokindex gt) {
			int j = 1;
			while (j <= addK->k && strcmp(addK->sleutel[j].c_str(), t.c_str()) < 0) {
				j++;
			}		
			addK->k++;					
			int k = addK->k;
			while(k > j) { //alles 1 positie opschuiven naar rechts
				addK->sleutel[k] = addK->sleutel[k-1];
				addK->sleutel[k-1] = "";
				addK->data[k] = addK->data[k-1];
				addK->data[k-1] = 0;
				addK->blokindices[k] = addK->blokindices[k-1];
				addK->blokindices[k-1] = 0;
				k--;								
			}			
			addK->sleutel[j] = t; // fill new (empty) spot
			addK->data[j] = d;
			
			for (int i = 0; i <= addK->k;i++)
				if (lt > 0 && addK->blokindices[i]==lt)
					lt = 0; //if we delete lt, then no action on addK needs to be performed because k-1 is already filled and gt will fill k
				else if (gt > 0 && addK->blokindices[i]==gt) {
					gt = 0; //if we delete gt, then we have to move the blokindex k-1 to k, then lt will fill k-1
					addK->blokindices[k] = addK->blokindices[k-1];
					addK->blokindices[k-1] = 0;
				}
			
			if (lt > 0)
				addK->blokindices[j-1] = lt;
			if (gt > 0)
				addK->blokindices[j] = gt;
			
		}
		void deleteShiftLeft(Knoop* loper, int j) {

			loper->sleutel[j] = "";
			loper->data[j] = 0;
			loper->blokindices[j-1] = 0;

			int k = j;
			while(k <= loper->k) { //alles 1 positie opschuiven naar links
				loper->sleutel[k-1] = loper->sleutel[k];
				loper->sleutel[k] = "";
				loper->data[k-1] = loper->data[k];
				loper->data[k] = 0;
				k++;								
			}
			k = j;
			while(k <= loper->k) { //alles 1 positie opschuiven naar links				
				loper->blokindices[k-1] = loper->blokindices[k];
				loper->blokindices[k] = 0;	
				k++;
			}		
			loper->k--;
		}
		void voegToe(T t, D d, bool verbose = true) {						
			if (wortel.k == 0) {
				wortel.voegToeInArray(t, d, 0, 0);
				wortel.isblad = true;
				schijf.herschrijf(wortel, wortelindex);				
				if (verbose) { cout << "add("<< t << "): \t";this->print(); }
				return;
			}

			stack<blokindex> path;						
			zoekBlokIndex(t, path);

			bool added = 0;
			blokindex addBi = 0;
			Knoop* ptp = new Knoop(); //PushToParent node (contains T, D, (Lbi), (Rbi))
			ptp->isblad  = false;
			while (!added && !path.empty()) {				
				blokindex bi = path.top();	
				path.pop();
				Knoop* loper = new Knoop();
				schijf.lees(*loper, bi); //fill Knoop				
				if (loper->exists(t) != -1) {  //case duplicate
					int f = loper->exists(t);
					loper->data[f]++;
					schijf.herschrijf(*loper, bi);		
					return;
				}
				bool overflow = loper->k == m-1;				
				if (!overflow) { //case not full (merge)
					if (addBi != 0)
						shiftRightAdd(loper,ptp->sleutel[1], ptp->data[1], ptp->blokindices[0], ptp->blokindices[1]);
					else
						shiftRightAdd(loper, t, d, 0, 0);					
					schijf.herschrijf(*loper, bi);
					schijf.verwijder(addBi);
					added = true;
				} else {					
					Knoop* addK = new Knoop();
					if (addBi == 0) {
						addK->voegToeInArray(t,d,0,0);
						addBi = schijf.schrijf(*addK);
					} else
						schijf.lees(*addK, addBi);
					addK->isblad = loper->isblad;

					int mid = (loper->k/2)+1;
					int addK_pos = mid;
					if (strcmp(addK->sleutel[1].c_str(), loper->sleutel[mid].c_str()) > 0)
						++addK_pos;
					else if (	strcmp(addK->sleutel[1].c_str(), loper->sleutel[mid].c_str()) < 0 &&
								strcmp(addK->sleutel[1].c_str(), loper->sleutel[mid-1].c_str()) < 0 )
						--addK_pos;
					

					Knoop* tmp = new Knoop();
					tmp->isblad = false;
					if (addK_pos < mid) {
						//  (a..c) + [addK] + d..m..z
						--mid;
						tmp->voegToeInArray(loper->sleutel[mid], loper->data[mid], 0, 0);	 //assign median
											
						for (int i = 1; i < mid; i++) //add all left of median::loper to addK
							shiftRightAdd(addK, loper->sleutel[i], loper->data[i], loper->blokindices[i-1], loper->blokindices[i]);

						schijf.herschrijf(*addK, addBi);
						addK->crashtest--;

						int k = loper->k;
						for (int i = 1; i <= mid; i++) { //delete range [start...mid]
							deleteShiftLeft(loper, 1);
						}

						schijf.herschrijf(*loper, bi);		
						tmp->blokindices[0] = addBi; // addK goes left
						tmp->blokindices[1] = bi; //loper goes right
					} else if (addK_pos > mid) { 
						//  a..m..w + [addK] + (x..z)						
						tmp->voegToeInArray(loper->sleutel[mid], loper->data[mid], 0, 0);	 //parent is median						
						
						for (int i = mid+1; i <= loper->k; i++) //add all right of median::loper to addK
							shiftRightAdd(addK, loper->sleutel[i], loper->data[i],loper->blokindices[i-1], loper->blokindices[i]);
						
						schijf.herschrijf(*addK, addBi);
						addK->crashtest--;

						int j = loper->k;
						for (int i = mid; i <= j; i++) { //delete all right of mid-1
							loper->sleutel[i] = "";
							loper->data[i] = 0;
							loper->blokindices[i] = 0;
							loper->k--;
						}

						schijf.herschrijf(*loper, bi);
						tmp->blokindices[0] = bi; // loper goes left
						tmp->blokindices[1] = addBi; // addK goes right
					} else {
						if (addK_pos==mid) {
							// a..d + [addK] + f..z							
							tmp->voegToeInArray(addK->sleutel[1], addK->data[1], 0, 0);	 //parent is median
							addK = new Knoop();
							addK->isblad = loper->isblad;
							
							for (int i = mid, j = 1; i <= loper->k; i++, j++){ //add all right of median::loper to addK
								addK->sleutel[j] = loper->sleutel[i];
								addK->data[j] = loper->data[i];
								if (addK->blokindices[j] == 0)
									addK->blokindices[j] = loper->blokindices[i];
								addK->k++;
							}
							addK->blokindices[0] = ptp->blokindices[1]; // !!!

							schijf.herschrijf(*addK, addBi);
							addK->crashtest--;

							int j = loper->k;
							for (int i = mid; i <= j; i++) { //delete all right of mid-1
								loper->sleutel[i] = "";
								loper->data[i] = 0;
								loper->blokindices[i] = 0;
								loper->k--;
							}
							
							loper->blokindices[mid-1] = ptp->blokindices[0]; // !!!

							schijf.herschrijf(*loper, bi);
							tmp->blokindices[0] = bi; // loper goes left
							tmp->blokindices[1] = addBi; // addK goes right
						}
					}
					ptp = tmp;
					addBi = schijf.schrijf(*ptp);
				}

				if (!added && path.empty()) {
					wortelindex = addBi;
					added = 1;
				}

			}
			schijf.lees(wortel, wortelindex);
			if (verbose) { cout << "add("<< t << "): \t";this->print(); }
		}		
		void print() {
			schijf.lees(wortel, wortelindex);
			wortel.print(schijf);
			cout << endl << endl;
		}
		std::priority_queue<Knoop, std::vector<Knoop>, CompareKnoop<T,D,m>>* geefDataSorted() {
			std::priority_queue<Knoop, std::vector<Knoop>, CompareKnoop<T,D,m>>* pq = new std::priority_queue<Knoop, std::vector<Knoop>, CompareKnoop<T,D,m>>();

			queue<blokindex> pool;
			pool.push(wortelindex);
			blokindex bi = 0;
			while (!pool.empty()) {
				bi = pool.front();
				pool.pop();
				if (bi == 0) continue;
				Knoop* loper = new Knoop();
				schijf.lees(*loper, bi);
			
				if (loper->blokindices[0] != 0)
						pool.push(loper->blokindices[0]);
				for (int i = 1; i <= loper->k; i++) {					
					Knoop k;
					k.sleutel[1] = loper->sleutel[i];	
					k.data[1] = loper->data[i];
					pq->push(k);
					if (loper->isblad == false && loper->blokindices[i] != 0)
						pool.push(loper->blokindices[i]);
				}

				while (pq->size() > 20)
					pq->pop();
			}

			return pq;
		}
	private:		
		Schijf<Knoop>& schijf;
		Knoop wortel;
		blokindex wortelindex;
		blokindex zoekBlokIndex(T el, std::stack<blokindex> &parents) {
			Knoop* loper = new Knoop();
			blokindex index = wortelindex;
			blokindex prev = index;
			schijf.lees(*loper, index);
			parents.push(index);

			
			while(!loper->isblad) {				
				if (int f = loper->exists(el) != -1)
					return index; //duplicate found, return immediately				

				for(int i = loper->k; i >= 1; i--) {
					T t = loper->sleutel[i];					
					if (strcmp(el.c_str(), t.c_str()) > 0 && loper->blokindices[i] != 0) {	
						prev = index;					
						index = loper->blokindices[i];												
						break;
					}
				}	

				if (index!=prev) {					
					schijf.lees(*loper, index); 
					parents.push(index);
					prev = index;
				} else {
					blokindex check =
						(loper->blokindices[0] != 0 ? loper->blokindices[0] :
							(loper->blokindices[1] != 0? loper->blokindices[1] : -1));					
					prev = index;					
					index = check;
					schijf.lees(*loper, index); 
					parents.push(index);
					prev = index;
					
				}
			}

			return index;
		}
};



#endif
