//
//  Scheme.h
//  lab3
//
//  Created by Tyler Udy on 7/13/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Scheme_h
#define Scheme_h
#include <iostream>
#include <vector>


class Scheme : public std::vector<std::string> {
    
public:
    //adds elements from the vector of strings into the scheme
    void add_elements_from_vect(std::vector<std::string> vect);
    bool contains_multiple(std::string value);
    bool index_of_duplicate_occurance(int index);
    std::string elements_toString();
};


#endif /* Scheme_h */
