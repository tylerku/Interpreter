//
//  Interpreter.cpp
//  lab3
//
//  Created by Tyler Udy on 7/13/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Interpreter.h"
#include <algorithm>

//-------------------------------------------------------------------EVALUATERS--------------------------------------------


void Interpreter::evaluate_schemes(){
    std::string relationName;
    for(unsigned int i = 0; i < program.schemes.size(); i++){
        relationName = program.schemes[i].stringify_name();
        Scheme relationScheme;
        relationScheme.add_elements_from_vect(program.schemes[i].get_data());//add a vector into relation scheme
        theDatabase.add_relation(Relation(relationName, relationScheme));//this relationScheme kind of scares me
    }
    
    scheme_output();
}

void Interpreter::evaluate_facts(){
    std::string predicateName;
    Tuple tupleToAdd;
    for (unsigned int i = 0; i < program.facts.size(); i++){
        tupleToAdd.clear();
        //Use the predicate name from the fact to determine the Relation to which the Tuple should be added.
        predicateName = program.facts[i].stringify_name(); //this is the relation key
        //Use the values listed in the fact to provide the values for the Tuple.
        tupleToAdd.add_elements_from_vect(program.facts[i].get_data());
        //add a tuple to a relation
        theDatabase.add_tuple_to_relation(tupleToAdd, predicateName);
        
    }
    facts_output();
}

void Interpreter::evaluate_rules(){
    
    Relation currentRelation;
    Relation resultRelation;
    Predicate currentRequirement;
    std::string currentRequirementName;
    std::vector<Parameter> currentRequirementParameters;
    std::vector<Parameter> resultRelationParameters;
    std::vector<Predicate> requirements;
    std::vector<Relation> intermediateResults;
    Predicate head;
    std::vector<Parameter> headParams;
    int tuples_before_operation = 0;
    int passes = 0;
    std::set<Tuple> tuples_to_output;
    Tuple currentTuple;
    output << "Rule Evaluation\n" << std::endl;;
    /*
     For each rule that is evaluated, output the rule followed by the tuples generated by the rule. Output the rule in the same form that it appears in a Datalog program. Output each tuple in the same form as in the previous project. Output the tuples after the last step of the evaluation of the rule, after the attributes have been renamed to be union compatible with the relation that matches the head of the rule. Only output the tuples that don't already exist in the result relation.
     
     After outputting all the evaluated rules, output a blank line followed by a line with the string,
    */
    
    do{ //fixed point algorithm
        passes++;
        tuples_before_operation = theDatabase.get_number_of_tuples();
        for(unsigned int i = 0; i < program.rules.size(); i++){
            output << program.rules[i].toString() << std::endl;
           head = program.rules[i].get_result();
           requirements = program.rules[i].get_requirements();
           intermediateResults.clear();
           resultRelationParameters.clear();
        
           for(unsigned int j = 0; j < requirements.size(); j++){                                             //STEP 1
              
               currentRequirement = requirements[j];
               currentRequirementName = currentRequirement.stringify_name();
               currentRequirementParameters = currentRequirement.get_data_as_parameters();
            
               currentRelation = theDatabase.get_relation(currentRequirementName);
               resultRelation = select_relation(currentRelation, currentRequirementParameters, j, true);
               resultRelation = project_relation(resultRelation, currentRequirementParameters, currentRequirementParameters);
               resultRelation = rename_relation(resultRelation, currentRequirementParameters);

               intermediateResults.push_back(resultRelation);
               //resultRelationParameters = combine_parameters(resultRelationParameters, currentRequirementParameters);
               
           }
            resultRelation = join_intermediate_results(intermediateResults);                            //STEP 2
        
            headParams = head.get_data_as_parameters();                                                 //STEP 3
            resultRelation = project_relation_final(resultRelation, headParams);
            
            Relation tempR = theDatabase.get_relation(head.stringify_name());                           //STEP4
            resultRelation = resultRelation.rename(tempR.get_scheme());
            //resultRelation = rename_relation(resultRelation, headParams);
            
            Relation databaseRelation = theDatabase.get_relation(head.stringify_name());                //OUTPUT
            Relation relation_to_output = resultRelation.subtract_tuples(databaseRelation.get_tuples());
            output_tuples(relation_to_output);
            Relation& r = theDatabase.get_relation(head.stringify_name());
            resultRelation = resultRelation.unite(r);     //STEP 5
            
           
           
       }
    }
    while(theDatabase.get_number_of_tuples() != tuples_before_operation);
    
    
    output << "\n" << "Converged after " << passes << " passes through the Rules.\n\n";
    std::map<std::string, Relation> dataMap = theDatabase.get_database();
    for(std::map<std::string, Relation>::iterator it = dataMap.begin(); it != dataMap.end(); it++){
        currentRelation = it->second;
        output << currentRelation.get_name() << "\n";
        output_tuples(currentRelation);
        output << "\n";
    }


}

void Interpreter::evaluate_queries(){
    Relation currentRelation;
    Relation resultRelation;
    std::string queryName;
    output << "Query Evaluation\n" << std::endl;
    
    std::vector<Parameter> queryParameters;
    for(unsigned int i = 0; i < program.queries.size(); i++){
        //output query and a space
        output << program.queries[i].toString() << "? ";

        queryName = program.queries[i].stringify_name();
        queryParameters = program.queries[i].get_data_as_parameters();
        currentRelation = theDatabase.get_relation(queryName);
        
        resultRelation = select_relation(currentRelation, queryParameters, i, false);
        
        if (resultRelation.get_tuples().size() == 0){
            output << "No\n" << std::endl;
            continue;
        } else {
            output << "Yes(" << resultRelation.get_tuples().size() << ")\n";
        }
        
        select_query_output(resultRelation);
        resultRelation = project_relation(resultRelation, queryParameters, queryParameters);
        project_query_output(resultRelation, queryParameters);
        resultRelation = rename_relation(resultRelation, queryParameters);
        rename_query_output(resultRelation);
    }
}


//------------------------------------------------RELATION-OPERATORS---------------------------------------------------------

/*  Use the select operation to select the tuples from the Relation that match the query. Iterate over the parameters of the query: If the parameter is a constant, select the tuples from the Relation that have the same value as the parameter in the same position as the parameter. If the parameter is a variable and the same variable appears later in the query, select the tuples from the Relation that have the same value in both positions where the variable appears.
 */

Relation Interpreter::select_relation(Relation oldRelation, std::vector<Parameter> queryParameters, int index, bool isRule){
    
    Parameter currentParameter;
    Relation returnRelation;
    int currentParameterIndex;
    int secondInstance;

    for (unsigned int j = 0; j < queryParameters.size(); j++){
        
        currentParameter = queryParameters[j];
        if (isRule){
            returnRelation = select_relation_for_rule(oldRelation, queryParameters, index);
            //returnRelation = oldRelation;

        
        } else if (currentParameter.is_constant()){
            
            returnRelation = oldRelation.select(j, currentParameter.toString());
            oldRelation = returnRelation;
            
        } else if (!queryParameters[j].is_constant() && program.queries[index].has_multiple(currentParameter)){
            currentParameterIndex = j;
            secondInstance = program.queries[index].second_occurence_index(currentParameter);//if no second occurance returns first index
            if (currentParameterIndex != secondInstance){
                returnRelation = oldRelation.select(currentParameterIndex, secondInstance);
            }
            
        } else if (!queryParameters[j].is_constant()){
            returnRelation = oldRelation;
        }
    }
    
    return returnRelation;

}
Relation Interpreter::select_relation_for_rule(Relation oldRelation, std::vector<Parameter> requirementParameters, int index){
    Parameter currentParameter;
    Relation returnRelation;
    int currentParameterIndex;
    int secondInstance;
    bool currentParameterRepeats;
    
    for (unsigned int j = 0; j < requirementParameters.size(); j++){
        currentParameterRepeats = value_repeats_in_vector(requirementParameters, currentParameter);

        currentParameter = requirementParameters[j];
        if (currentParameter.is_constant()){
            
            returnRelation = oldRelation.select(j, currentParameter.toString());
            oldRelation = returnRelation;
            
        } else if (!requirementParameters[j].is_constant() && currentParameterRepeats){
            currentParameterIndex = j;
            //secondInstance = program.queries[index].second_occurence_index(currentParameter);//if no second occurance returns first index
            secondInstance = second_ocurance_index_in_requirement(requirementParameters, currentParameter);
            if (currentParameterIndex != secondInstance){
                returnRelation = oldRelation.select(currentParameterIndex, secondInstance);
            }
            
        } else if (!requirementParameters[j].is_constant()){
            returnRelation = oldRelation;
        }
    }
    
    return returnRelation;

}


/*
 projects the columns of the oldRelation that are found inthe prms2prjt. Matches their order as well.
*/
Relation Interpreter::project_relation(Relation oldRelation, std::vector<Parameter> params, std::vector<Parameter> prms2prjt){

    Relation returnRelation;
    std::vector<int> variableIndexes;
    
    for(unsigned int i = 0; i < prms2prjt.size(); i++){
        for(unsigned int j = 0; j < params.size(); j++){
            if (!params[j].is_constant() && prms2prjt[i].toString() == params[j].toString()){
                variableIndexes.push_back(j);
                break;
            }
        }
    }
    
    returnRelation = oldRelation.project(variableIndexes);
    return returnRelation;
}

Relation Interpreter::project_relation_final(Relation oldRelation, std::vector<Parameter> prms2prjt){
    
    Relation returnRelation;
    std::vector<int> variableIndexes;
    Scheme schm = oldRelation.get_scheme();
    
    for(unsigned int i = 0; i < prms2prjt.size(); i++){
        for(unsigned int j = 0; j < schm.size(); j++){
            //if (!params[j].is_constant() && prms2prjt[i].toString() == params[j].toString()){
            if (schm[j] == prms2prjt[i].toString()){
                variableIndexes.push_back(j);
                break;
            }
            
        }
    }
    
    returnRelation = oldRelation.project(variableIndexes);
    return returnRelation;
}


/*
 changes the Scheme of the oldRelation to match the elements of params
 use this if its easier to pass in the params of the scheme instead of the scheme itself
 */

Relation Interpreter::rename_relation(Relation oldRelation, std::vector<Parameter> params){
    //i need a vect of strings with the elements in the query
    Relation returnRelation;
    Scheme newScheme;
    std::vector<std::string> paramsAsStrings;
    for(unsigned int i = 0; i < params.size(); i++){
        if (!params[i].is_constant()){
            std::string q = params[i].toString();
            paramsAsStrings.push_back(params[i].toString());
        }
    }
    newScheme.add_elements_from_vect(paramsAsStrings);
    
    returnRelation = oldRelation.rename(newScheme);
    return returnRelation;
}


//-------------------------------------------OUTPUT-FUNCTIONS---------------------------------------------------------------

void Interpreter::scheme_output(){
    output << "Scheme Evaluation\n" << std::endl;
}

void Interpreter::facts_output(){
    output << "Fact Evaluation\n" << std::endl;
    
    std::map<std::string, Relation> relations = theDatabase.get_database();
    Relation currentRelation;
    std::set<Tuple> relationFacts;
    Tuple currentFact;
    Scheme scheme;
    
    for( std::map<std::string, Relation>::iterator itr = relations.begin(); itr != relations.end(); itr++) {
        output << itr->first << std::endl;
        currentRelation = itr->second;
        relationFacts = currentRelation.get_tuples();
        scheme = currentRelation.get_scheme();
        
        for (std::set<Tuple>::iterator i = relationFacts.begin(); i != relationFacts.end(); i++){
            currentFact = *i;
            output << "  ";
            for(unsigned int j = 0; j < currentRelation.get_scheme().size(); j++){

                
                output << scheme[j] << "=" << currentFact[j] << " ";
            }
            output << "\n";
            
        }
        output << "\n";
    }
}

void Interpreter::project_query_output(Relation r, std::vector<Parameter> queryParameters){
    output <<"project\n";
    std::vector<int> duplicateValue; //indexes of query where the value is a duplicate
                                     //its a vector with all the positions to not project
    int count = 0;
    for(unsigned int i = 0; i < queryParameters.size(); i++){
        count = 0;
        for (unsigned int j = 0; j < queryParameters.size(); j++){
            if (queryParameters[i].toString() == queryParameters[j].toString()){
                count++;
                if (count > 1){
                    duplicateValue.push_back(j);
                }
            }
        }
    }
    output_project_tuples(r, duplicateValue);
}

void Interpreter::rename_query_output(Relation r){
    output <<"rename\n";
    output_tuples(r);
    output << "\n";
}

void Interpreter::select_query_output(Relation r){
    
    output << "select\n";
    output_tuples(r);
}

void Interpreter::output_tuples(Relation r){
    Scheme scheme = r.get_scheme();
    std::set<Tuple> tuplesInR = r.get_tuples();
    Tuple currentTuple;
    //for each tuple in the relation
    for(std::set<Tuple>::iterator i = tuplesInR.begin(); i != tuplesInR.end(); i++){
        currentTuple = *i;
        if (scheme.size() == 0){
            continue;
        }
        output << "  ";
        for (unsigned int j = 0; j < scheme.size(); j++){
            if(scheme.index_of_duplicate_occurance(j)){
                continue;
            }
            output << scheme[j] << "=" << currentTuple.at(j) << " ";
        }
        output << "\n";
        
        //scheme[i] = tuple[i]
    }
}

void Interpreter::output_project_tuples(Relation r, std::vector<int> duplicateValues){
    Scheme scheme = r.get_scheme();
    std::set<Tuple> tuplesInR = r.get_tuples();
    Tuple currentTuple;
    //for each tuple in the relation
    for(std::set<Tuple>::iterator i = tuplesInR.begin(); i != tuplesInR.end(); i++){
        currentTuple = *i;
        if (scheme.size() == 0){
            continue;
        }
        output << "  ";
        for (unsigned int j = 0; j < scheme.size(); j++){
            if (std::find(duplicateValues.begin(), duplicateValues.end(), j) != duplicateValues.end())
                continue;
            output << scheme[j] << "=" << currentTuple.at(j) << " ";
        }
        output << "\n";
        
        //scheme[i] = tuple[i]
    }
}

std::string Interpreter::toString(){
    return output.str();
}
//----------------------------------------------------HELPER-FUNCTIONS------------------------------------------------------

Relation Interpreter::join_intermediate_results(std::vector<Relation> intermediateResults){
    
    Relation lastRelation;
    
    if (intermediateResults.size() == 1){
        return intermediateResults.at(0);
    } else if (intermediateResults.size() == 0){
        return lastRelation;
    } else {
        lastRelation = intermediateResults.at(0);
        for (unsigned int i = 1; i < intermediateResults.size(); i++){
            lastRelation = lastRelation.join(intermediateResults[i]); //join last relation with the current relation
        }
    }
    
    return lastRelation;
}

/*
    adds all elemnts of p2 to p1 and returns a new vector. Duplicates are not added.
 */
std::vector<Parameter> Interpreter::combine_parameters(std::vector<Parameter> p1, std::vector<Parameter> p2){
   
    std::vector<Parameter> returnParams = p1;
    bool contains = false;
    
    for (unsigned int i = 0; i < p2.size(); i++){
        contains = false;
        for (unsigned int j = 0; j < p1.size(); j++){
            if(p2[i].toString() == p1[j].toString()){
                contains = true;
                break;
            }
        }
        if (contains == false){
            returnParams.push_back(p2[i]);
        }
    }
    //            returnParams.push_back(p2[i]);

    return returnParams;
}

bool Interpreter::value_repeats_in_vector(std::vector<Parameter> requirementParameters, Parameter value){
    int count = 0;
    for(unsigned int i = 0; i < requirementParameters.size(); i++){
        if (requirementParameters[i].toString() == value.toString()){
            count++;
        }
        if (count > 1){
            return true;
        }
    }
    return false;
}

int Interpreter::second_ocurance_index_in_requirement(std::vector<Parameter> requirementParams, Parameter value){
    int secondOccuranceIndex = 0;
    int count = 0;
    for(unsigned int i = 0; i < requirementParams.size(); i++){
        if (requirementParams[i].toString() == value.toString()){
            count++;
        }
        if (count == 2){
            secondOccuranceIndex = i;
            break;
        }
    }
    return secondOccuranceIndex;
}


