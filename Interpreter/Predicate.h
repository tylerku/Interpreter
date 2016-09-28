//
//  Predicate.h
//  lab2
//
//  Created by Tyler Udy on 7/5/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Predicate_h
#define Predicate_h
#
#include "Parameter.h"
#include<vector>




//parameter name;
//vector<parameters> data;


//overload lessthan oporator for sort function at end

class Predicate {

private:
    
    Parameter name;
    std::vector<Parameter> data;
    
    void addParenIfNeeded(char parenToAdd, std::stringstream& ss);
    
    
public:
    
    Predicate(Parameter theName, std::vector<Parameter> params): name(theName), data(params){}
    Predicate(){}
    ~Predicate(){}
    
    void addParameter(Parameter param);
    
    
    std::string toString();
    std::string stringify_name();
    std::vector<std::string> get_data();
    std::vector<Parameter> get_data_as_parameters(){ return data; };
    bool has_multiple(Parameter value);
    int second_occurence_index(Parameter value);
};

#endif /* Predicate_h */
