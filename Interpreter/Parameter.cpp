//
//  Parameter.cpp
//  lab2
//
//  Created by Tyler Udy on 7/5/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Parameter.h"



std::string Parameter::toString(){
    std::stringstream ss;
    if (value != "" && name == ""){
        ss << value;
    } else if (name != "" && value == ""){
        ss << name;
    }
    return ss.str();
}

bool Parameter::is_constant(){
    if(value != ""  && name == ""){
        return true;
    } else {
        return false;
    }
}