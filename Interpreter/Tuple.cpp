//
//  Tuple.cpp
//  lab3
//
//  Created by Tyler Udy on 7/22/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Tuple.h"


void Tuple::add_elements_from_vect(std::vector<std::string> vect){
    for(unsigned int i = 0; i < vect.size(); i++){   //maybe add a clear before this becuase ill never want to incrament this
        this->push_back(vect[i]);
    }

}

