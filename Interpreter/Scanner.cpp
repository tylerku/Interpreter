//
//  Scanner.cpp
//  labs
//
//  Created by Tyler Udy on 6/27/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Scanner.h"

void Scanner::scan_token(std::ifstream& inputFile){
    char c = inputFile.get();
    Token newToken;
    
    check_for_new_line(inputFile, c);
    skip_comments(inputFile, c);
    skip_ws(inputFile, c);
    
    if (inputFile.eof()){ //check if the file is at its end
        tokens.push_back(Token(END, currentLine, ""));
        ended = true;
        return;
    }
    
    switch (c){
        case ',': tokens.push_back(Token(COMMA, currentLine, ","));
            break;
        case '.': tokens.push_back(Token(PERIOD, currentLine, "."));
            break;
        case '?': tokens.push_back(Token(Q_MARK, currentLine, "?"));
            break;
        case '(': tokens.push_back(Token(LEFT_PAREN, currentLine, "("));
            break;
        case ')': tokens.push_back(Token(RIGHT_PAREN, currentLine, ")"));
            break;
        case ':': colon_state_machine(inputFile, c);//checks for ":" or ":-"
            break;
        case 'S': schemes_state_machine(inputFile, c);
            break;
        case 'F': facts_state_machine(inputFile, c);
            break;
        case 'R': rules_state_machine(inputFile, c);
            break;
        case 'Q': queries_state_machine(inputFile, c);
            break;
        case '\'': string_state_machine(inputFile, c);     //change this one
            break;
        default: identifier_state_machine(inputFile, c);
            break;
    }
}

//checks to seee if character is white space
void Scanner::skip_ws(std::ifstream& inputFile, char& c){
    
    while(isspace(c)){
        c = inputFile.get();
        check_for_new_line(inputFile, c);//incraments lineNumber when c = '\n'
    }
    skip_comments(inputFile, c); //if theres a comment after white space skip that too
    
    if (isspace(c)){
        skip_ws(inputFile, c);
    }
}

//sets c to first character after commnet
void Scanner::skip_comments(std::ifstream& inputFile, char& c){
    while (c == '#'){
        while(c != '\n'){
            c = inputFile.get();
        }
        check_for_new_line(inputFile, c);
    }
    if (c == '#'){
        skip_comments(inputFile, c);
    }
}

//checks for new line
//sets c to next character after new line
void Scanner::check_for_new_line(std::ifstream& inputFile, char& c){
    while (c == '\n'){
        c = inputFile.get();
        currentLine++;
    }
}

void Scanner::colon_state_machine(std::ifstream& inputFile, char& c){
    char nextChar = char(inputFile.peek());
    if(nextChar == '-'){
        inputFile.get();
        tokens.push_back(Token(COLON_DASH, currentLine, ":-"));
    } else {
        tokens.push_back(Token(COLON, currentLine, ":"));
    }
}

void Scanner::schemes_state_machine(std::ifstream& inputFile, char& c){
    match_token_with_keyword("Schemes", inputFile, c);
}

void Scanner::facts_state_machine(std::ifstream& inputFile, char& c){
    match_token_with_keyword("Facts", inputFile, c);
}

void Scanner::rules_state_machine(std::ifstream& inputFile, char& c){
    match_token_with_keyword("Rules", inputFile, c);
}

void Scanner::queries_state_machine(std::ifstream& inputFile, char& c){
    match_token_with_keyword("Queries", inputFile, c);
}

void Scanner::string_state_machine(std::ifstream& inputFile, char& c){
    std::stringstream ss;
    ss << c;
    char x = inputFile.get();
    bool emptyString = false;
    if (x == '\''){//empty string case
        ss << x;
        emptyString = true;
    }
    while(x != '\''){
        if (inputFile.eof() || x == '\n'){
            error();
            return;
        } else {
            ss << x;
            x = inputFile.get();
        }
    }
    if (emptyString == false){
        ss << x;//push on last ' character if it wasnt empty string
    }
    tokens.push_back(Token(STRING, currentLine, ss.str()));
}

void Scanner::identifier_state_machine(std::ifstream& inputFile, char& c){
    //this happens when a single character is read that is invalid
    if (!isalpha(c) && (currentToken.str() == "")){
        error();
        return;
    }
    /*if the first letter wasnt pushed on by another state machine
     push it on
    */
    if (currentToken.str() == ""){
        currentToken << c;
    }
    
    /*other state machines take care of this but
     it is necessary for the above stated case
    */
     while(isalnum(char(inputFile.peek()))){
        c = inputFile.get();// myIdent
        currentToken << c;
    }
    tokens.push_back(Token(ID, currentLine, currentToken.str())); //'string'x:?
    currentToken.str("");
}

std::string Scanner::tokens_to_string(){
    std::string str;
    if (tokenStream.str() == ""){
        for(unsigned int i = 0; i < tokens.size(); i++){
            tokenStream << tokens[i].toString() << "\n";
        }
    }
    return tokenStream.str();
}

void Scanner::match_token_with_keyword(std::string keyword, std::ifstream& inputFile, char& c){
    char firstLetter = keyword[0];
    TokenType typeOfToken;
    
    switch(firstLetter){
        case 'S': typeOfToken = SCHEMES;
            break;
        case 'F': typeOfToken = FACTS;
            break;
        case 'R': typeOfToken = RULES;
            break;
        case 'Q': typeOfToken = QUERIES;
            break;
        default:  //This function is never called unless one of the above cases is true
                  //therefore typeOfToken will always be initialized
            break;
    }
    
    currentToken.str("");
    currentToken << c;
    while(isalnum(char(inputFile.peek()))){
        c = inputFile.get();
        currentToken << c;
    }
    if (currentToken.str() == keyword){
        tokens.push_back(Token(typeOfToken, currentLine, keyword));
    } else {
        identifier_state_machine(inputFile, c);
    }
    currentToken.str("");
}

void Scanner::error(){
    tokens_to_string(); //put all current tokens into tokenStream
    tokenStream << "Input Error on line " << currentLine << "\n";
    ended = true;
    endedOnError = true;
}

bool Scanner::is_finished(){
    return ended;
}

bool Scanner::ended_on_error(){
    return endedOnError;
}

Token Scanner::get_latest_token(){ //has unexpected behavior if tokens is empty
    return tokens.back();
}

std::vector<Token> Scanner::get_tokens(std::ifstream& inputFile){
    while (!this->is_finished()){
        this->scan_token(inputFile);
    }
    return tokens;
}

