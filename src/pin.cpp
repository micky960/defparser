#include<sstream>
#include<boost/algorithm/string.hpp>

#include "pin.h"

PIN::PIN(std::string line){
    
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of("+"));
    boost::split(tokens, tokens[0], boost::is_any_of(" "));
    name = tokens[1];
    boost::algorithm::trim(name);
    boost::split(tokens, line, boost::is_any_of("+"));
    boost::split(tokens, tokens[5], boost::is_any_of(" "));
    x = atoi(tokens[3].c_str());
    y = atoi(tokens[4].c_str());

    boost::split(tokens, line, boost::is_any_of("+"));
    dir = (tokens[2].find("INPUT") != std::string::npos)? false : true;

}

PIN::PIN(const PIN &obj){
    name = obj.getName();
    dir = obj.getDir();
    std::pair<int, int> loc = obj.getLoc();
    x = loc.first;
    y = loc.first;

}
