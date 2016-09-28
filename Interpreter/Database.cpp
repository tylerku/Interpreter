//
//  Database.cpp
//  lab3
//
//  Created by Tyler Udy on 7/13/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Database.h"


void Database::add_relation(Relation newRelation){
    database.insert(std::pair<std::string, Relation>(newRelation.get_name(),newRelation));
}

Relation& Database::get_relation(std::string relationName){
    return database.at(relationName);
}

void Database::add_tuple_to_relation(Tuple theTuple, std::string relationName){
    
    //if relationNames isnt in database an exception is thrown by the .at() function
    database.at(relationName).add_tuple(theTuple); //could implement a try catch block here.
}

int Database::get_number_of_tuples(){
    int count = 0;
    Relation currentRelation;
    
    //go through each relation and count its tuples.
    //add that number to the grand total
    for(std::map<std::string, Relation>::iterator itr = database.begin(); itr != database.end(); itr++){
        currentRelation = itr->second;
        count += currentRelation.get_tuples().size();
    }
    return count;
}