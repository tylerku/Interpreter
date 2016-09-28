//
//  DatalogProgram.h
//  lab2
//
//  Created by Tyler Udy on 7/5/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef DatalogProgram_h
#define DatalogProgram_h

#include "Rule.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Scanner.h"
#include <vector>
#include <set>


//vector<rule> Rule

class DatalogProgram {
    
private:
    Scanner tokenScanner;
    int position;
    Token currentToken;
    std::vector<Token> tokensToParse;
    std::set<std::string> domain;
    std::vector<Parameter> params;
    std::vector<Predicate> rulesPredicates;
    char currentKey;
    std::stringstream output;
    std::stringstream stringified;
    void datalog_program();
    void scheme_list();
    void fact_list();
    void rule_list();
    void query_list();
    void scheme();
    void fact();
    void rule();
    void query();
    void predicate_list();
    void predicate();
    void parameter_list();
    void parameter();
    void match(TokenType type);
    void error();
    bool errorOccured;
    void success();
    Token getTokenFromScanner();
    void push_predicate(Parameter name);
    void add_predicates_to_output(std::vector<Predicate> vect);

    
    
public:
    //wrtie a function for each of the rules
    
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
    std::vector<Predicate> queries;

    
    DatalogProgram(){
        position = 0;
        errorOccured = false;
    }
    
    ~DatalogProgram(){
        
    }
    
    DatalogProgram& parse(std::vector<Token> tokensFromScanner);
    
    std::string toString();
    
    std::string get_stringified_program(); // this should be implemented in toString but i dont have time right now
};

#endif /* DatalogProgram_h */
