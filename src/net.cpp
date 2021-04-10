#include<sstream>
#include<vector>
#include <boost/algorithm/string.hpp>
#include<ctype.h>

#include "net.h"

NET::NET(std::string net){

    src = "";
    std::vector<std::string> tokens;
    boost::split(tokens, net, boost::is_any_of("+"));
    boost::split(tokens, tokens[0], boost::is_any_of(" "));
    name = tokens[1];
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
