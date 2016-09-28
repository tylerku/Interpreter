//
//  Rule.h
//  lab2
//
//  Created by Tyler Udy on 7/5/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Rule_h
#define Rule_h
#include "Rule.h"
#include "Predicate.h"


//Predicate name
//vector<predicate>data;

class Rule {
    
private:
    
    Predicate result;
    std::vector<Predicate> requirements;
    
public:
    Rule(Predicate theResult, std::vector<Predicate> data): result(theResult), requirements(data){}
    Rule(){}
    ~Rule(){}
    
    std::string toString();
    Predicate get_result(){ return result; }
    std::vector<Predicate> get_requirements() { return requirements; }
    bool has_multiple(Predicate value);

    
};

#endif /* Rule_h */
