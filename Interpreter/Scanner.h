//
//  Scanner.h
//  labs
//
//  Created by Tyler Udy on 6/27/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Scanner_h
#define Scanner_h
#include <iostream>
#include <vector>
#include "Token.h"
#include <fstream>
class Scanner {

private:
    
    std::vector<Token>tokens;
    std::stringstream tokenStream;
    std::stringstream currentToken;
    std::string stringOfTokens;
    int currentLine;
    bool ended;
    bool endedOnError;
    void skip_ws(std::ifstream& inputFile, char& c);
    void skip_comments(std::ifstream& inputFile, char& c);
    void check_for_new_line(std::ifstream& inputFile, char& c);
    void colon_state_machine(std::ifstream& inputFile, char& c);
    void schemes_state_machine(std::ifstream& inputFile, char& c);
    void facts_state_machine(std::ifstream& inputFile, char& c);
    void rules_state_machine(std::ifstream& inputFile, char& c);
    void queries_state_machine(std::ifstream& inputFile, char& c);
    void string_state_machine(std::ifstream& inputFile, char& c);
    void identifier_state_machine(std::ifstream& inputFile, char& c);
    void match_token_with_keyword(std::string keyword, std::ifstream& inputFile,char& c);

    //bool n
    void error();
    
    
public:
    
    Scanner(){ currentLine = 1; ended = false; endedOnError = false; }
    
    ~Scanner(){}
    
    void scan_token(std::ifstream& inputFile);
    
    std::string tokens_to_string();
    
    std::vector<Token> get_tokens(std::ifstream& inputFile);
    
    bool is_finished();
    
    bool ended_on_error();
    
    Token get_latest_token();


};



#endif /* Scanner_h */
