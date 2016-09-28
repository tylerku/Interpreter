//
//  Relation.cpp
//  lab3
//
//  Created by Tyler Udy on 7/13/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Relation.h"
#include <algorithm>
#include <sstream>



Relation Relation::select(int index, std::string value){
    //look at every tuple and see if it contains the value at the given index
    Relation resultRelation(name, myscheme);
    Tuple tempTuple;
    unsigned int x(index);
    for(std::set<Tuple>::iterator it = tuples.begin(); it != tuples.end(); it++){
        tempTuple = *it;
        if (x > tempTuple.size()){
            throw "out of range of tuple in the relation";
        }
        if (tempTuple[index] == value){
            //add temp tuple to the relation
            resultRelation.add_tuple(tempTuple);
        }
    }
    
    return resultRelation; //empty relation if no matches were found
}

Relation Relation::select(int index1, int index2){ //i guess this is for if its a variable
    
    //Another select function could take two parameters that are both positions. The two given positions in a tuple would need to have equal values for that tuple to be included in the result.
    Relation resultRelation(name, myscheme);
    Tuple tempTuple;
    for(std::set<Tuple>::iterator it = tuples.begin(); it != tuples.end(); it++){
        tempTuple = *it;
        if (tempTuple.at(index1) == tempTuple.at(index2)){
            resultRelation.add_tuple(tempTuple);
        }
    }
    return resultRelation;
}

Relation Relation::project(std::vector<int>indexes){ //this vectors is a vector of the indexes to project

    Scheme resultScheme;
    for (unsigned int i = 0; i < indexes.size(); i++){//create the scheme to be projected
        if (myscheme.size() != 0 && size_t(indexes[i]) < myscheme.size()){//avoid bad access
            std::string temp = myscheme.at(indexes[i]);
            resultScheme.push_back(temp);
        }
        
    }
    Relation resultRelation(name, resultScheme);
    //add appropriate relations
    Tuple tupleToAdd;
    Tuple tempTuple;
    for(std::set<Tuple>::iterator it = tuples.begin(); it != tuples.end(); it++){
        //add relation with the specified columns
        tempTuple = *it;
        tupleToAdd.clear();
        for (unsigned int j = 0; j < indexes.size(); j++){//initialize tuple to add
            
            
            tupleToAdd.push_back(tempTuple[indexes[j]]);
        }
        resultRelation.add_tuple(tupleToAdd);
    }
    return resultRelation;
}

Relation Relation::rename(Scheme newScheme){
    Relation resultRelation = *this;
    resultRelation.set_scheme(newScheme);
    return resultRelation;
}

Relation Relation::join(Relation r2){
    
    Relation r1 = *this;
    Scheme resultScheme = get_joint_scheme(r1.get_scheme(), r2.get_scheme());
    
    Relation resultRelation("", resultScheme);
    
    
    Tuple t1;
    Tuple t2;
    Tuple newTuple;
    Scheme s1 = r1.get_scheme();
    Scheme s2 = r2.get_scheme();
    std::set<Tuple> r1_tuples = r1.get_tuples();
    std::set<Tuple> r2_tuples = r2.get_tuples();
     //for each tuple t1 in r1
    int x = 0;
    for(std::set<Tuple>::iterator i = r1_tuples.begin(); i != r1_tuples.end(); i++){
        // for each tuple t2 in r2
    
        x++;
        t1 = *i;
        if(t1.size() == 0){
            continue;
        }
        for(std::set<Tuple>::iterator j = r2_tuples.begin(); j != r2_tuples.end(); j++){
            t2 = *j;
            if (joinable(t1, t2, s1, s2)){
                newTuple = join_tuples(t1, t2, s1, s2);
                resultRelation.add_tuple(newTuple);
            }
        }
    }

    return resultRelation;
}

Relation Relation::unite(Relation& r2){
    
    Relation r1 = *this;
    std::string returnRelationName = r2.get_name();
    Relation returnRelation = r1;
    Tuple currentTuple;
    //if r1.scheme and r2.scheme arent the same throw an error
    if (r1.get_scheme() != r2.get_scheme()){
        throw("ERROR! these two relations are not union compatable");
    }
    
    //go through tuples of r2
    std::set<Tuple> r2_tuples = r2.get_tuples();
    std::set<Tuple> r1_tuples = r1.get_tuples();
    for (std::set<Tuple>::iterator itr = r2_tuples.begin(); itr != r2_tuples.end(); itr++){
        currentTuple = *itr;
        if(!(returnRelation.contains(currentTuple))){
            returnRelation.add_tuple(currentTuple);
        }
    }
    returnRelation.set_name(returnRelationName);
    r2 = returnRelation; //this is a reference, therefore the acutal relation in the database will be updated.
    return returnRelation;
}



void Relation::add_tuple(Tuple tpl){
    if (this->contains(tpl) == false){
        tuples.insert(tpl);
    }
}

void Relation::remove_tuple(Tuple tpl){
  if (this->contains(tpl) == true){
        tuples.erase(tpl);
    }
}

bool Relation::contains(Tuple tpl){
    for (std::set<Tuple>::iterator itr = tuples.begin(); itr != tuples.end(); itr++){
        if (*itr == tpl){
            return true;
        }
    }
    return false;
}



//----------------------------------------Helper-Functions--------------------------------------------------------


Scheme Relation::get_joint_scheme(Scheme s1, Scheme s2){
    Scheme resultScheme = s1;
    //if an element of s2 is in s1 continue
    
    for (unsigned int i = 0; i < s2.size(); i++){
        if (std::find(s1.begin(), s1.end(), s2[i]) != s1.end()){
            continue;
        } else {
            resultScheme.push_back(s2[i]);
        }
    }
     

    return resultScheme;
}


bool Relation::joinable(Tuple t1, Tuple t2, Scheme s1, Scheme s2){
    
    std::vector<int> matchingIndexesForS1;
    std::vector<int> matchingIndexesForS2;
    std::string value1;
    std::string value2;
    for(unsigned int i = 0; i < s1.size(); i++ ){
        for(unsigned int j = 0; j < s2.size(); j++){
            if (s1[i] == s2[j]){
                matchingIndexesForS1.push_back(i);//these will always be the same size
                matchingIndexesForS2.push_back(j);
            }
        }
    }
    
    for (unsigned int i = 0; i < matchingIndexesForS1.size(); i++){//if one of the values in the tuple has matching element
        //in the scheme but not in the tuple, return false
        std::string y = t1[0];
        value1 = t1[matchingIndexesForS1[i]];
        value2 = t2[matchingIndexesForS2[i]];
        if (value1 != value2){
            return false;
        }
    }
    
    return true;
}


Tuple Relation::join_tuples(Tuple t1, Tuple t2, Scheme s1, Scheme s2){
    
    Tuple t = t1;
    std::vector<int> s2IndexesToAdd;
    
    for(unsigned int i = 0; i < s2.size(); i++){
        std::string s = s2[i];
        if(!(std::find(s1.begin(), s1.end(), s) != s1.end())){//if s1 doesnt contain s2[i]
            //add the related value to the result tuple
            t.push_back(t2[i]);
        }
    }
    return t;
}

Relation Relation::subtract_tuples(std::set<Tuple> tuples_to_subtract){
    Tuple currentTuple;
    Relation returnRelation = *this;
    for(std::set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++){
        currentTuple = *i;
        if (std::find(tuples_to_subtract.begin(), tuples_to_subtract.end(), currentTuple) != tuples_to_subtract.end()){
            returnRelation.remove_tuple(currentTuple);
        }
    }
    return returnRelation;
}



