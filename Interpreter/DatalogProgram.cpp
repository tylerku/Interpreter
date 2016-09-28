//
//  DatalogProgram.cpp
//  lab2
//
//  Created by Tyler Udy on 7/5/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "DatalogProgram.h"


DatalogProgram& DatalogProgram::parse(std::vector<Token> tokensFromScanner){ //take vector of tokens
    try{
        tokensToParse = tokensFromScanner;
        currentToken = tokensToParse[position];
        datalog_program();
    }
    catch(const char* e){
        stringified << e;
        stringified << "  " << currentToken.toString() << "\n";
    }
    
    if (errorOccured == false){
        stringified << "Success!\n";
        stringified << this->toString();
    }
    return *this;
}


//-------------------------------------------------Parser-Functions--------------------------------------------------

void DatalogProgram::datalog_program(){
    if (currentToken.get_type() == SCHEMES){
        //Schemes---------------
        currentKey = 'S';
        match(SCHEMES);
        match(COLON);
        scheme();
        scheme_list();
        
        //Facts----------------
        currentKey = 'F';
        match(FACTS);
        match(COLON);
        fact_list();
        
        //Rules----------------
        currentKey = 'R';
        match(RULES);
        match(COLON);
        rule_list();
        
        //Queries--------------
        currentKey = 'Q';
        match(QUERIES);
        match(COLON);
        query();
        query_list();
        
    } else {
        error();
    }
}

void DatalogProgram::scheme_list(){
    if (currentToken.get_type() == ID){
        scheme();
        scheme_list();
    } else if (currentToken.get_type() == FACTS){
        //do nothing
    } else {
        error();
    }
}

void DatalogProgram::fact_list(){
    if (currentToken.get_type() == ID){
        fact();
        fact_list();
    } else if (currentToken.get_type() == RULES){ //idk if this will work the ways its implemented
        //do nothing
    } else {
        error();
    }
}
void DatalogProgram::rule_list(){
    if (currentToken.get_type() == ID){
        rule();
        rule_list();
    } else if (currentToken.get_type() == QUERIES){
        //do nothing
    } else {
        error();
    }
}

void DatalogProgram::query_list(){
    if (currentToken.get_type() == ID){
        query();
        query_list();
    } else if (currentToken.get_type() == END){
        //do nothing
    } else {
        error();
    }
}

void DatalogProgram::scheme(){
    if(currentToken.get_type() == ID){
        predicate();
    } else {
        error();
    }
}
void DatalogProgram::fact(){
    if (currentToken.get_type() == ID){
        predicate();
        match(PERIOD);
    } else {
        error();
    }
}

void DatalogProgram::rule(){
    if (currentToken.get_type() == ID){
        predicate();
        Predicate result = rulesPredicates[0];
        rulesPredicates.clear();//the rest of the predicates after this will be the requirements
        match(COLON_DASH);
        predicate();
        predicate_list();
        match(PERIOD);
        rules.push_back(Rule(result, rulesPredicates));
        rulesPredicates.clear(); //clear the vector for the next rule
    } else {
        error();
    }
}

void DatalogProgram::query(){
    if (currentToken.get_type() == ID){
        predicate();
        match(Q_MARK);
    } else {
        error();
    }
}
void DatalogProgram::predicate_list(){
    if (currentToken.get_type() == COMMA){
        match(COMMA);   //maybe
        predicate();
        predicate_list();
    } else if (currentToken.get_type() == PERIOD){
        //do nothing
    } else {
        error();
    }
}

void DatalogProgram::predicate(){
    if (currentToken.get_type() == ID){
        Parameter name = Parameter(currentToken.get_value(), "");
        match(ID);
        match(LEFT_PAREN);
        parameter();
        parameter_list();
        match(RIGHT_PAREN);
        push_predicate(name);
    } else {
        error();
    }
    
}

void DatalogProgram::parameter_list(){
    if (currentToken.get_type() == COMMA){
        match(COMMA);
        parameter();
        parameter_list();
    } else if (currentToken.get_type() == RIGHT_PAREN){
        //do nothing
    } else {
        error();
    }
}

void DatalogProgram::parameter(){
    if (currentToken.get_type() == STRING){
        params.push_back(Parameter("", currentToken.get_value()));
        domain.insert(currentToken.get_value());
        match(STRING);
    } else if (currentToken.get_type() == ID){
        params.push_back(Parameter(currentToken.get_value(), ""));
        match(ID);
    } else {
        error();
    }
}

//----------------------------------------------Helper-Parser-Functions----------------------------------------------

void DatalogProgram::match(TokenType type){
    if(currentToken.get_type() == type){
        position++;
        currentToken = tokensToParse[position];
    } else {
        error();
    }
}

void DatalogProgram::error(){
    errorOccured = true;
    throw "Failure!\n";
}


//----------------------------------------------Other-Helper-Functions----------------------------------------------

std::string DatalogProgram::toString(){
    output.str("");
    output << "Schemes(" << schemes.size() << "):\n";
    add_predicates_to_output(schemes);
    
    output << "Facts(" << facts.size() << "):\n";
    add_predicates_to_output(facts);
    
    output << "Rules(" << rules.size() << "):\n";
    for (unsigned int i = 0; i < rules.size(); i++){
        output << "  " << rules[i].toString() << "\n";
    }
    
    output << "Queries(" << queries.size() << "):\n";
    add_predicates_to_output(queries);
    
    output << "Domain(" << domain.size() << "):\n";
    
    for (std::set<std::string>::iterator i = domain.begin(); i != domain.end(); i++){
        output << "  " << *i << "\n";
    }
    return output.str();
}

std::string DatalogProgram::get_stringified_program(){
    return stringified.str();
}

Token DatalogProgram::getTokenFromScanner(){
    return tokenScanner.get_latest_token();
}

void DatalogProgram::push_predicate(Parameter name){
    switch (currentKey){
        case 'S':   schemes.push_back(Predicate(name, params));
            break;
        case 'F':   facts.push_back(Predicate(name, params));
            break;
        case 'Q':   queries.push_back(Predicate(name, params));
            break;
        case 'R':   rulesPredicates.push_back(Predicate(name, params));
            break;
    }
    params.clear(); //clear params to get ready for the next set of params
}

void DatalogProgram::add_predicates_to_output(std::vector<Predicate> vect){
    for (unsigned int i = 0; i < vect.size(); i++){
        output << "  " << vect[i].toString() << "\n";
    }
}

