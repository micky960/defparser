#include<string.h>
#include <boost/algorithm/string.hpp>

#include "def.h"

DEF::DEF(std::ifstream& def){

   _parseComponents(def); 
   std::cout << "Cell parse passed" << std::endl;
   _parsePins(def); 
   std::cout << "Pin parse passed" << std::endl;
   _parseNets(def); 
   std::cout << "Net parse passed" << std::endl;
   _connectAll();

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

    while(line.find("END SPECIALNETS")==std::string::npos)
        getline(def, line);

    getline(def, line);
    getline(def, line);
    getline(def, line);

    while(line.find("END NETS")==std::string::npos){
        std::string lineCon;
        do{
            lineCon += line;
            getline(def, line);
        }while(line.find(";")==std::string::npos);
        lineCon += line;
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

