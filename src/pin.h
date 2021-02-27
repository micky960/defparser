#ifndef PIN_H
#define PIN_H

#include<iostream>
#include<vector>
#include <boost/algorithm/string.hpp>

class PIN{

    std::string name;
    bool dir;//input-->0, output-->1
    int x,y;
    public:
        PIN(const std::string line);
        PIN(const PIN &obj);
        bool getDir() const {return dir;}
        std::string getName() const {return name;}
        std::pair<int, int> getLoc() const {return std::make_pair (x,y);}
};

#endif
