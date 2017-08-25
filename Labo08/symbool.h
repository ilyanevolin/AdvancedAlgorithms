#ifndef __SYMBOOL_H
#define __SYMBOOL_H
#include <string>


class Symbool: public std::string{
public:
    Symbool():std::string("S"){};
    Symbool& operator++(){
        int i=size();
        while (i>0 &&operator[](i-1)=='R'){
            i--;
            operator[](i)='S';
        }
        if (i==0)
            push_back('T');
        else{
            char& nuchar=operator[](i-1);
            nuchar=( nuchar=='Z'?
                        'A':
                        nuchar+1);
        }
    return *this;
    }; 
};
#endif

