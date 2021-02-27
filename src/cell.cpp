#include<sstream>
#include<boost/algorithm/string.hpp>
#include<boost/algorithm/string/detail/trim.hpp>

#include "cell.h"

CELL::CELL(std::string line){
    
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of("+"));
    if(tokens.size() == 3){
        _parseNameType(tokens[0]);
        _parseLoc(tokens[2]);
    }
    else{
        _parseNameType(tokens[0]);
        _parseLoc(tokens[1]);
    }
}

void CELL::_parseNameType(std::string token){
    std::vector<std::string> tokens2;
    boost::split(tokens2, token, boost::is_any_of(" "));
    boost::algorithm::trim(tokens2[1]);
    name = tokens2[1];
    boost::algorithm::trim(name);
    boost::algorithm::trim(tokens2[2]);
    type = tokens2[2];
    boost::algorithm::trim(type);
}

void CELL::_parseLoc(std::string token){
    std::vector<std::string> tokens2;
    boost::split(tokens2, token, boost::is_any_of(" "));
    x = atoi(tokens2[3].c_str());
    y = atoi(tokens2[4].c_str());
}
