#ifndef CELL_H
#define CELL_H

#include<iostream>
#include<vector>
#include<unordered_map>

class CELL{

    std::string name, type;
    //stores location
    int x,y;

    public:
        CELL(const std::string line);
        std::string getName() const {return name;}
        std::string getType() const {return type;}
        std::pair<int, int> getLoc() const {return std::make_pair (x,y);}
    private:
        void _parseNameType(std::string token);
        void _parseLoc(std::string token);
};

#endif
