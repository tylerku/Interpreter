//
//  Predicate.cpp
//  lab2
//
//  Created by Tyler Udy on 7/5/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Predicate.h"



void Predicate::addParameter(Parameter param){
    data.push_back(param);
}


std::string Predicate::toString(){
    std::stringstream ss;
    ss << name.toString();
    addParenIfNeeded('(', ss);
    for (unsigned int i = 0; i < data.size(); i++){
        ss << data[i].toString();
        if (i != data.size() - 1){
            ss << ",";
        }
    }
    addParenIfNeeded(')', ss);
    
    
    return ss.str();
}


void Predicate::addParenIfNeeded(char parenToAdd, std::stringstream& ss){
    if(data.size() > 0){
        ss << parenToAdd;
    }
}

std::vector<std::string> Predicate::get_data(){
    
    std::vector<std::string> returnVect;
    for(unsigned int i = 0; i < data.size(); i++){
        returnVect.push_back(data[i].toString());
    }
    
    return returnVect;
}

std::string Predicate::stringify_name(){
    return name.toString();
}

bool Predicate::has_multiple(Parameter value){
    
    int count = 0;
    for (unsigned int j = 0; j < data.size(); j++){
        if (data[j].toString() == value.toString()){
            count++;
        }
    }
    if (count > 1){
        return true;
    }
    
    return false;
}


int Predicate::second_occurence_index(Parameter value){ //returns the initial index if no second is found
    int count = 0;
    int startPoint = 0;
    for (unsigned int i = 0; i < data.size(); i++){
        if(data[i].toString() == value.toString()){
            count++;
            startPoint = i;
        }
        if (count == 2){
            return i;
        }
    }
    
    return startPoint;
}
