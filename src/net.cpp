#include<sstream>
#include<vector>
#include <boost/algorithm/string.hpp>
#include<ctype.h>
#include<string>

#include "net.h"

NET::NET(std::string net){

    std::cout << net << std::endl;
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
    //std::cout << "################### Calling parseOpens" << std::endl;
    //std::cout << name << std::endl;
    std::unordered_map<std::string, std::pair<int, std::string> > locList;
    for(auto n: netSegmentList){
        bool isSplitLayer = (n.find("metal"+splitLayer) != std::string::npos);
        //std::cout << "netSegmentList: " << n << ", layer: " << isSplitLayer << std::endl;
        std::vector<std::string> tokens;
        boost::split(tokens, n, boost::is_any_of("("));
        std::string xPrev, yPrev;

        bool isStackedVia = 0;

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
            //Patch for parsing stacked vias
            //if (isSplitLayer && (tokens.size() == 2)){
            //    isStackedVia = true;
            //    std::cout << "Net: " << name << ", Loc: " << x << "," << y << ", Source: " << getSrc() << ", stacked via" << std::endl;
            //}
            if(locList.find((x+","+y).c_str()) == locList.end()){
                if (isSplitLayer){
                    if (tokens.size() == 2)
                        locList.insert({(x+","+y).c_str(), std::make_pair (0, "isStackedVia")});
                    else if (tokens.size() == 3)
                        locList.insert({(x+","+y).c_str(), std::make_pair (0, "notStackedVia")});
                }
                else
                    locList.insert({(x+","+y).c_str(), std::make_pair (0, "")});
            }
            else{
                //locList[(x+","+y).c_str()] = locList[(x+","+y).c_str()] + 1;
                std::pair <int, std::string > myPair = locList[(x+","+y).c_str()];
                if (isSplitLayer){
                    if(myPair.second.empty()){
                        myPair.first = myPair.first + 1;
                        if (tokens.size() == 2)
                            myPair.second = "isStackedVia";
                        else if (tokens.size() == 3)
                            myPair.second = "notStackedVia";

                        locList[(x+","+y).c_str()] = myPair;
                    }
                    else{
                        myPair.first = myPair.first + 1;
                        myPair.second = "notStackedVia";
                        locList[(x+","+y).c_str()] = myPair;
                    }   
                }
                else{
                    myPair.first = myPair.first + 1;
                    locList[(x+","+y).c_str()] = myPair;
                }
                //std::cout << x << "," << y << " collision: " << locList[(x+","+y).c_str()] << std::endl;
            }
            
            xPrev = x;
            yPrev = y;
            //std::cout << x << ", " << y << std::endl;
        }
    }
    for(auto n: netSegmentList){
        //if(n.find("metal"+splitLayer) != std::string::npos){
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
                if(locList[(x+","+y).c_str()].first == 0)
                    openList.push_back(x+","+y);
                else if(locList[(x+","+y).c_str()].first > 0 && locList[(x+","+y).c_str()].second == "isStackedVia")
                    openList.push_back(x+","+y);
                    //std::cout << x << ", " << y << "--> open" << std::endl;
                xPrev = x;
                yPrev = y;
                //std::cout << x << ", " << y << std::endl;
            }
        //}
    }
}
