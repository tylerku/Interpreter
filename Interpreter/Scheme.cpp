//
//  Scheme.cpp
//  lab3
//
//  Created by Tyler Udy on 7/21/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Scheme.h"
#include <algorithm>
#include <sstream>

void Scheme::add_elements_from_vect(std::vector<std::string> vect){
    for(unsigned int i = 0; i < vect.size(); i++){   //maybe add a clear before this becuase ill never want to incrament this
        this->push_back(vect[i]);
    }
}

bool Scheme::contains_multiple(std::string value){
    int count = 0;
    for(unsigned int i = 0; i < this->size(); i++){
        if (this->at(i) == value){
            count++;
        }
        if (count > 1){
            return true;
        }
    }
    return false;
}

bool Scheme::index_of_duplicate_occurance(int index){ //returns true if this is the second occurance of the index value
    
    if (std::find(this->begin(), this->end(), this->at(index)) != this->end() - this->size() + index){
        return true;
    }
    return false;
}

std::string Scheme::elements_toString(){
    std::stringstream ss;
    for(unsigned int i = 0; i < this->size(); i++){
        ss << this->at(i);
    }
    return ss.str();
}