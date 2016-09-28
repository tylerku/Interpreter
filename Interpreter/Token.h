//
//  Header.h
//  labs
//
//  Created by Tyler Udy on 6/24/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#ifndef Header_h
#define Header_h
#include <iostream>
#include <sstream>


enum TokenType {
    COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, END
};

class Token {

private:
    
    TokenType type;
    int lineNumber;
    std::string value;
    
public:
    
    Token(){
        
    }
    
    Token(TokenType t, int line, std::string val) : type(t), lineNumber(line), value(val){
        
    }
    
    ~Token(){
        
    }
    
    int get_line_number();
    
    std::string get_value();
    
    TokenType get_type();
    
    std::string toString();
    
};


#endif /* Header_h */
