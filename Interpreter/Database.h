//
//  Database.h
//  lab3
//
//  Created by Tyler Udy on 7/13/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Database_h
#define Database_h
#include <map>
#include "Relation.h"

//add interpretur class that does work and runs everything
    //other classes are all the data basically

//Database
    //essentially a map
    //map <string,relation>     //string represent

//relation
    //string name;
    //scheme
    //set<tuple>


//use inheritance on tuple and scheme

//class scheme: public vector <string> {}   //basically this is all i need

class Database {
    
private:
    
    std::map <std::string, Relation> database;
    
    
public:
    
    Database(){}
    
    ~Database(){}
    
    void add_relation(Relation newRelation);
    
    Relation& get_relation(std::string name);
    
    void add_tuple_to_relation(Tuple theTuple, std::string relationName);
    
    std::map <std::string,Relation> get_database() { return database; }
    
    int get_number_of_tuples();
    
    
    
};

#endif /* Database_h */
