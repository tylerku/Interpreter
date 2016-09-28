//
//  Relation.h
//  lab3
//
//  Created by Tyler Udy on 7/13/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Relation_h
#define Relation_h
#include<iostream>
#include "Tuple.h"
#include <set>
#include "Scheme.h"

class Relation {
    
private:
    
    std::string name;
    Scheme myscheme;
    std::set<Tuple> tuples;
    
    void set_scheme(Scheme newScheme){ myscheme = newScheme; }
    Scheme get_joint_scheme(Scheme s1, Scheme s2);
    bool joinable(Tuple t1, Tuple t2, Scheme s1, Scheme s2);
    Tuple join_tuples(Tuple t1, Tuple t2, Scheme s1, Scheme s2);

    
public:
    
    Relation(){}
    Relation(std::string theName, Scheme theScheme) : name(theName), myscheme(theScheme){}
    
    //each of these functions operates on an existing relation and returns a new relation (result of the operation)
    Relation select(int index, std::string value);
    Relation select(int index1, int index2);
    Relation project(std::vector<int>indexes);
    Relation rename(Scheme newScheme);
    Relation join(Relation r2);
    Relation unite(Relation& r2);
    
    std::string get_name(){ return name; }
    void set_name(std::string n){ name = n; }
    Scheme get_scheme() { return myscheme; }
    void add_tuple(Tuple tpl);
    void remove_tuple(Tuple tpl);
    std::set<Tuple> get_tuples() { return tuples; }
    bool contains(Tuple tpl);
    Relation subtract_tuples(std::set<Tuple> tuples_to_subtract);
    int get_number_of_tuples(){ return int(tuples.size()); }
    
    
};


#endif /* Relation_h */
