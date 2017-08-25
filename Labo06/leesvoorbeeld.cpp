#include "stdafx.h"
#include "pugixml.h"
#include <iostream>
#include <functional>

using namespace pugi;
using namespace std;
const char* node_types[] =
{
    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
};

//bezoekt een (deel)boom met een visitor in preorder
void DEZBezoekpre(pugi::xml_node n,std::function<void(pugi::xml_node)> bezoekKnoop){
    bezoekKnoop(n);
    for (auto&& k:n.children())
        DEZBezoekpre(k,bezoekKnoop);
};



void schrijfPugiboom(pugi::xml_node n){
    DEZBezoekpre(n,[](pugi::xml_node nn){
        std::cout << node_types[nn.type()] << ": '" << nn.name()<<"'";
        if (nn.value() && strlen(nn.value()) !=0)
            std::cout<< ", waarde='" << nn.value();
        std::cout<<"\n";

        if (nn.attributes_begin() != nn.attributes_end()){
            std::cout<<"  Attributen:\n    ";
            for (auto&& att : nn.attributes())
                std::cout << " " << att.name() << "=" << att.value();
            std::cout<<"\n";
        };
    });
    
};
/*
int main()
{
    pugi::xml_document doc;
    if (!doc.load_file("treingraaf.xml")) return -1;
    schrijfPugiboom(doc);
    // end::traverse[]
}
*/
// vim:et
