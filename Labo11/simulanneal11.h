#ifndef __SIMULANNEAL
#define __SIMULANNEAL
#include <cstdlib>
#include <memory>

//abstracte klasse voor simulated annealing
//Hier worden GEEN probleemafhankelijke functies gedefinieerd
//deze zijn allemaal virtual.
//Notaties: zie Blum and Roli
//Parameter S: De oplossing (zowel de voorlopige als de uiteindelijke)
//is een S. S staat voor Search space


template <class S>
class SimulatedAnnealing{
public:
    S oplossing(){
        T=T0();
        s=initialSolution();
        while (!terminateCond()){
            S sprime=pickAtRandom(s);
			print(s);
			print(sprime);
            double deltaf=f(sprime)-f(s);
			
			double prob = p(T,deltaf);
			double _prob = prob*RAND_MAX;
			double rnd = rand();			
            if (deltaf < 0 || rnd < _prob ){
                s=std::move(sprime);
            }            
            updateT();
        }
        return s;
    }
	virtual void print(const S&)=0;
protected:
    double T, _T0;//temperatuur;
    S s;//voorlopige en eindoplossing.
    virtual double T0()=0;//initi"ele temperatuur
    virtual bool terminateCond()=0;//true als stoppen met zoeken
    virtual double f(const S&)=0;//objectieffunctie. Moet > 0 blijven
    virtual S initialSolution()=0;
    virtual S pickAtRandom(const S&)=0;
    virtual double p(double T, double deltaf)=0;//probabiliteit verandering
    virtual void updateT()=0;
	
};

#endif
