#include<string.h>
#include <boost/algorithm/string.hpp>

#include "def.h"

DEF::DEF(std::ifstream& def){

    // Requires order, if order is different fail!!! Reset file handler
   _parseComponents(def); 
   std::cout << "Cell parse passed" << std::endl;
   _parsePins(def); 
   std::cout << "Pin parse passed" << std::endl;
   _parseNets(def); 
   std::cout << "Net parse passed" << std::endl;
   _connectAll();
   std::cout << "Connected all components" << std::endl;
}

void DEF::_parseComponents(std::ifstream& def){
    std::string line;
    getline(def, line);
    while(line.find("COMPONENTS")==std::string::npos)
        getline(def, line);


    std::vector<std::string> tokens;
    while(line.find("END COMPONENTS")==std::string::npos){//parse all complist

        getline(def, line);
        if(line.find(";")!=std::string::npos||line.find("END COMPONENTS")!=std::string::npos)
            continue;
        CELL* cell = new CELL(line);
        cellList.insert({cell->getName(), cell});
    }

}

void DEF::_parsePins(std::ifstream& def){
    std::string line;
    getline(def, line);
    while(line.find("PINS")==std::string::npos)
        getline(def, line);
    getline(def, line);

    while(line.find("END PINS")==std::string::npos){
        std::string lineCon;
        do{
            lineCon += line;
            getline(def, line);
        }while(line.find(";")==std::string::npos);
        lineCon += line;
        PIN* pin = new PIN(lineCon);
        pinList.insert({pin->getName(), pin});
        getline(def, line);
    }
}

void DEF::_parseNets(std::ifstream& def){
   
    std::string line;

    while(line.find("NETS")==std::string::npos)
        getline(def, line);

    //getline(def, line);
    //getline(def, line);
    getline(def, line);

    //std::cout << line << std::endl;

    while(line.find("END NETS")==std::string::npos){
        std::string lineCon;
        do{
            lineCon += line+"\n";
            getline(def, line);
        }while(line.find(";")==std::string::npos);
        lineCon += line+"\n";
        NET* net = new NET(lineCon);
        netList.insert({net->getName(), net});
        getline(def, line);
    } 
}

void DEF::_connectAll(){

    #pragma omp parallel for schedule(dynamic) num_threads(128) 
    for(auto [name, net]: netList){
        std::vector<std::pair<std::string, std::string> > objList = net->getObjList();
        for(auto obj: objList){
            if(obj.first.find("PIN")!=std::string::npos){
                PIN* pin = pinList.at(obj.second);
                if(pin->getDir() == false){
                    net->setSrc(pin->getName());
                }
                else{
                    net->setSink(pin->getName());
                }
            }
            else{
                CELL* cell = cellList.at(obj.first);
                bool srcCond = (obj.second.find("Z") != std::string::npos) || (obj.second.find("ZN") != std::string::npos) || (obj.second.find("Q") != std::string::npos) || (obj.second.find("QN") != std::string::npos) || (obj.second.find("CO") != std::string::npos) || (obj.second.find("S") != std::string::npos && (cell->getType().find("HA") != std::string::npos || cell->getType().find("FA") != std::string::npos));
                if(srcCond){
                    net->setSrc(cell->getName());
                }
                else{
                    net->setSink(cell->getName());
                }
            }
        }
    }
}

std::vector <std::string> DEF::getOpens(std::string splitLayer){

    for(auto [name, net]: netList){
        std::string src = net->getSrc();
        net->parseOpens(splitLayer);
        std::vector<std::string> netOpenList = net->getOpen();
        openList.insert(openList.end(), netOpenList.begin(), netOpenList.end());
        for(auto o: netOpenList){
            if(!(net->getSrc()).empty())
                std::cout << "Net: " << name << ", Loc: " << o << ", Source: " << net->getSrc() << std::endl;
            else{
                std::cout << "Net: " << name << ", Loc: " << o << ", ";
                std::vector <std::string> sinkList = net->getSink();
                std::cout << "Sink: ";
                for (auto s: sinkList)
                    std::cout << s << ", ";
                std::cout << std::endl;                
            }
        }
    }
    return openList;
}
