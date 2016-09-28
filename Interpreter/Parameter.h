//
//  Parameter.h
//  lab2
//
//  Created by Tyler Udy on 7/5/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Parameter_h
#define Parameter_h
#include <iostream>
#include <sstream>

class Parameter {

private:
    
    std::string name; //for variables
    std::string value; //for constants
    
public:
    
    //one of the data members will be intitialized as the empty string
    Parameter(std::string theName, std::string theValue): name(theName), value(theValue){}
    Parameter(){}
    ~Parameter(){}
    
    std::string toString();
    std::string get_name(){ return name; }
    std::string get_value(){ return value; }
    bool is_constant();
    
};

#endif /* Parameter_h */
