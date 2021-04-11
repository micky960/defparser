#include<sstream>
#include<vector>
#include <boost/algorithm/string.hpp>
#include<ctype.h>
#include<string>

#include "net.h"

NET::NET(std::string net){

    src = "";
    std::vector<std::string> tokens;
    boost::split(tokens, net, boost::is_any_of("+"));
    boost::split(tokens, tokens[0], boost::is_any_of(" "));
    name = tokens[1];
    std::replace(name.begin(), name.end(), '(', ' ');
    boost::algorithm::trim(name);
    boost::split(tokens, net, boost::is_any_of("+"));
    boost::split(tokens, tokens[0], boost::is_any_of("("));

    for(unsigned i=1; i<tokens.size(); i++){//parse cell/pin names; 1st token is always net name 
        std::string f, s;
        std::stringstream ss1;
        ss1<<tokens[i];
        ss1>>f;
        ss1<<tokens[i];
        ss1>>s;
        objList.push_back({f, s});
    }

    boost::split(tokens, net, boost::is_any_of("+"));
    //std::cout << tokens[1] << std::endl;
    boost::split(tokens, tokens[1], boost::is_any_of("\n"));
    for(int i=0; i<tokens.size(); i++){
        boost::algorithm::trim(tokens[i]);
        if(tokens[i].find(";") == std::string::npos && !tokens[i].empty()){
            netSegmentList.push_back(tokens[i]);
        }
    }
}

void NET::setSrc(std::string name){
    src=name;
}

void NET::parseOpens(std::string splitLayer){
    //std::cout << name << std::endl;
    std::unordered_map<std::string, int> locList;
    for(auto n: netSegmentList){
        std::vector<std::string> tokens;
        boost::split(tokens, n, boost::is_any_of("("));
        std::string xPrev, yPrev;

        for(int i=1; i<tokens.size(); i++){
            std::string x, y;
            std::stringstream ss1;
            ss1<<tokens[i];
            ss1>>x;
            if(x == "*")
                x = xPrev;
            ss1<<tokens[i];
            ss1>>y;
            if(y == "*")
                y = yPrev;
            if(locList.find((x+","+y).c_str()) == locList.end())
                locList.insert({(x+","+y).c_str(), 0});
            else{
                locList[(x+","+y).c_str()] = locList[(x+","+y).c_str()]+1;
                //std::cout << x << "," << y << " collision: " << locList[(x+","+y).c_str()] << std::endl;
            }
            
            xPrev = x;
            yPrev = y;
            //std::cout << x << ", " << y << std::endl;
        }
    }
    for(auto n: netSegmentList){
        if(n.find("metal"+splitLayer) != std::string::npos){
            //std::cout << n << std::endl;
            std::vector<std::string> tokens;
            boost::split(tokens, n, boost::is_any_of("("));
            std::string xPrev, yPrev;

            for(int i=1; i<tokens.size(); i++){
                std::string x, y;
                std::stringstream ss1;
                ss1<<tokens[i];
                ss1>>x;
                if(x == "*")
                    x = xPrev;
                ss1<<tokens[i];
                ss1>>y;
                if(y == "*")
                    y = yPrev;
                if(locList[(x+","+y).c_str()] == 0)
                    openList.push_back(x+","+y);
                    //std::cout << x << ", " << y << "--> open" << std::endl;
                xPrev = x;
                yPrev = y;
                //std::cout << x << ", " << y << std::endl;
            }
        }
    }
}
