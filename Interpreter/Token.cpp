//
//  Token.cpp
//  labs
//
//  Created by Tyler Udy on 6/27/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <stdio.h>
#include "Token.h"


std::string Token::toString(){
    
    std::stringstream returnString;
    switch(type) {
        case COMMA: returnString << "(COMMA,\"" << value << "\"," << lineNumber << ")";
            break;
        case PERIOD: returnString << "(PERIOD,\"" << value << "\"," << lineNumber << ")";
            break;
        case Q_MARK: returnString << "(Q_MARK,\"" << value << "\"," << lineNumber << ")";
            break;
        case LEFT_PAREN: returnString << "(LEFT_PAREN,\"" << value << "\"," << lineNumber << ")";
            break;
        case RIGHT_PAREN: returnString << "(RIGHT_PAREN,\"" << value << "\"," << lineNumber << ")";
            break;
        case COLON: returnString << "(COLON,\"" << value << "\"," << lineNumber << ")";
            break;
        case COLON_DASH: returnString << "(COLON_DASH,\"" << value << "\"," << lineNumber << ")";
            break;
        case SCHEMES: returnString << "(SCHEMES,\"" << value << "\"," << lineNumber << ")";
            break;
        case FACTS: returnString << "(FACTS,\"" << value << "\"," << lineNumber << ")";
            break;
        case RULES: returnString << "(RULES,\"" << value << "\"," << lineNumber << ")";
            break;
        case QUERIES: returnString << "(QUERIES,\"" << value << "\"," << lineNumber << ")";
            break;
        case ID: returnString << "(ID,\"" << value << "\"," << lineNumber << ")";
            break;
        case STRING: returnString << "(STRING,\"" << value << "\"," << lineNumber << ")";
            break;
        case END: returnString << "(EOF,\"" << value << "\"," << lineNumber << ")";
            break;
        default: break;
    }
    return returnString.str();
}

std::string Token::get_value(){  //at times the token may be uninitialized
    return value;
}

int Token::get_line_number(){ //at times the token may be uninitialized
    return lineNumber;
}

TokenType Token::get_type(){
    return type;
}