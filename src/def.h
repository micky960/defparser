#ifndef DEF_H
#define DEF_H
#include<iostream>
#include<fstream>
#include<unordered_map>
#include<omp.h>

#include "cell.h"
#include "pin.h"
#include "net.h"

class DEF{

    public:
        std::unordered_map<std::string, CELL*> cellList;
        std::unordered_map<std::string, PIN*> pinList;
        std::unordered_map<std::string, NET*> netList;
        std::vector<std::string> openList;
        DEF(std::ifstream& def);
        std::unordered_map<std::string, CELL*> getCells() const {return cellList;}
        std::unordered_map<std::string, PIN*> getPins() const {return pinList;}
        std::unordered_map<std::string, NET*> getNets() const {return netList;}
        std::vector<std::string> getOpens(std::string);

    private:
        void _parseComponents(std::ifstream& def);
        void _parsePins(std::ifstream& def);
        void _parseNets(std::ifstream& def);
        void _connectAll();
};

#endif
