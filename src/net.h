#ifndef NET_H
#define NET_H

#include<iostream>
#include<string>
#include <boost/algorithm/string.hpp>

class NET{
    std::string name;
    std::string src;
    std::vector<std::string> sinkList;
    std::vector<std::string> netSegmentList;
    std::vector<std::string> openList;
    std::vector<std::pair<std::string, std::string> > objList;
    public:
        NET(const std::string net);
        std::string getName() const {return name;}
        std::vector<std::pair<std::string, std::string> > getObjList() const {return objList;}
        void setSrc(const std::string name);
        void setSink(const std::string name){sinkList.push_back(name);}
        std::string getSrc() const {return src;}
        std::vector<std::string> getSink() const {return sinkList;}
        std::vector<std::string> getOpen() const {return openList;}
        void parseOpens(std::string);
};

#endif
