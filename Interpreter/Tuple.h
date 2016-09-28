//
//  Tuple.h
//  lab3
//
//  Created by Tyler Udy on 7/13/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Tuple_h
#define Tuple_h
#include <iostream>
#include <vector>

class Tuple : public std::vector<std::string> {
    
public:
    
    void add_elements_from_vect(std::vector<std::string> vect);
    
};

#endif /* Tuple_h */
