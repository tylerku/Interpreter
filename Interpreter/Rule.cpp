//
//  Rule.cpp
//  lab2
//
//  Created by Tyler Udy on 7/5/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Rule.h"

std::string Rule::toString(){
    std::stringstream ss;
    ss << result.toString() << " :- ";
    for(unsigned int i = 0; i < requirements.size(); i++){
        ss << requirements[i].toString();
        if (i != requirements.size() - 1){
            ss << ",";
        }
    }
    return ss.str();
}

bool Rule::has_multiple(Predicate value){
    
    int count = 0;
    for (unsigned int j = 0; j < requirements.size(); j++){
        if (requirements[j].toString() == value.toString()){
            count++;
        }
    }
    if (count > 1){
        return true;
    }
    
    return false;
}
