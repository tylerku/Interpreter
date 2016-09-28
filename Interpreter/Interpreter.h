//
//  Interpreter.h
//  lab3
//
//  Created by Tyler Udy on 7/13/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Interpreter_h
#define Interpreter_h
#include <iostream>
#include "DatalogProgram.h"
#include "Database.h"
#include <sstream>

class Interpreter {
    
private:
    
    DatalogProgram& program;
    Database theDatabase;
    std::stringstream output;
    
    Relation select_relation(Relation oldRelation, std::vector<Parameter> params, int queryIndex, bool isRule);
    Relation select_relation_for_rule(Relation oldRelation, std::vector<Parameter> queryParameters, int index);

    Relation project_relation(Relation oldRelation, std::vector<Parameter> params, std::vector<Parameter> paramsToProject);
    Relation project_relation_final(Relation oldRelation, std::vector<Parameter> prms2prjt);
    Relation rename_relation(Relation oldRelation, std::vector<Parameter> params);
    
    void output_project_tuples(Relation r, std::vector<int> duplicateValues);
    //-----------------------sub-tasks--------------------------------------------------------------------------
    
    Relation join_intermediate_results(std::vector<Relation> intermediateResults);
    std::vector<Parameter> combine_parameters(std::vector<Parameter> p1, std::vector<Parameter> p2);
    bool value_repeats_in_vector(std::vector<Parameter> requirementParameters, Parameter currentParameter);
    int second_ocurance_index_in_requirement(std::vector<Parameter> requirementParameters, Parameter currentParameter);


    
    
public:
    
    //create and interpreter with an already parsed datalogProgram
    Interpreter();
    
    Interpreter(DatalogProgram& prgrm): program (prgrm){}
    
    void evaluate_schemes();
    
    void evaluate_facts();
    
    void evaluate_rules();
    
    void evaluate_queries();
    
    void scheme_output();
    
    void facts_output();
    
    //there is no rules_output() function. The functionality is included in the evaluate rules;
    
    void project_query_output(Relation r,std::vector<Parameter> queryParameters);
    
    void rename_query_output(Relation r);
    
    void select_query_output(Relation r);
    
    void output_tuples(Relation r);
    
    void test_operations(Relation r);
    
    std::string toString();
};


#endif /* Interpreter_h */
