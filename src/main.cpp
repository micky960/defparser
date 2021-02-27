#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<cassert>
#include<cmath>
#include<numeric>
#include <list>
#include <omp.h>
#include <boost/algorithm/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>

#include "def.h"

class NODE{
    std::string name, type;
    int x, y;
    public:
        NODE(std::string name, std::string type, std::pair<int, int > loc): name(name), type(type), x(loc.first), y(loc.second){}
        std::string getName() const {return name;}
        std::string getType() const {return type;}
        std::pair<int, int> getLoc() const {return std::make_pair (x,y);}
};

typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty; 
typedef boost::adjacency_list<boost::setS, boost::vecS, boost::directedS, NODE*, EdgeWeightProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;
typedef boost::graph_traits<Graph>::edge_iterator edge_iter;

vertex_t findVertex(std::string name, Graph g){
    std::pair<vertex_iter, vertex_iter> vp;
    for(vp = vertices(g); vp.first != vp.second; vp.first++){
        vertex_t v = *vp.first;
        if(g[v]->getName() == name)
            return v;
    }

    try{
        throw 1;
    }catch(int e){
        std::cout << "Error: did not find vertex: " << name << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]){

	assert(argc==2);

    std::ifstream def(argv[1]);

    if(def.fail()){
        std::cerr << "splitdef file not found"<<std::endl;
        exit(1);
    }

    DEF defParse = DEF(def);
    std::cout << "Def parsed" <<std::endl;

    std::unordered_map<std::string, CELL*> cellList = defParse.getCells();
    Graph g;
    for(auto [name, cell]: cellList){
        vertex_t v = boost::add_vertex(g);
        NODE* n = new NODE(cell->getName(), cell->getType(), cell->getLoc());
        g[v] = n;
    }
    std::unordered_map<std::string, PIN*> pinList = defParse.getPins();
    for(auto [name, pin]: pinList){
        vertex_t v = boost::add_vertex(g);
        std::string type = (pin->getDir())? "OUTPUT": "INPUT";
        NODE* n = new NODE(pin->getName(), type, pin->getLoc());
        g[v] = n;
    }
    std::unordered_map<std::string, NET*> netList = defParse.getNets();

    for(auto [name, net]: netList){
        vertex_t u = findVertex(net->getSrc(), g); 
        std::pair<int, int> uLoc = g[u]->getLoc();
        std::vector<std::string> sinkList = net->getSink();
        for(auto s: sinkList){
            vertex_t v = findVertex(s, g); 
            std::pair<int, int> vLoc = g[v]->getLoc();
            boost::add_edge(u, v, abs(vLoc.first - uLoc.first) + abs(vLoc.second - uLoc.second), g);
        }
    }

    boost::write_graphviz(std::cout, g);
    
    return 0;
}
