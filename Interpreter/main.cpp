//
//  main.cpp
//  labs
//
//  Created by Tyler Udy on 6/24/16.
//  Copyright © 2016 Tyler Udy. All rights reserved.
//

#include <iostream>
#include "Token.h"
#include "Scanner.h"
#include "DatalogProgram.h"
#include "Interpreter.h"


/*int main(int argc, const char* argv[]) { //This main is for testing with cout statements

    Scanner myScanner;
    DatalogProgram myProgram;
    std::ifstream strm("/Users/TyUdy/Developer/CS236/labs/lab4/lab4/prac00.txt"); //put name of file herre
    std::ofstream output(argv[2]);
    myProgram.parse(myScanner.get_tokens(strm));
    Interpreter myInterpreter(myProgram);
    myInterpreter.evaluate_schemes();
    myInterpreter.evaluate_facts();
    myInterpreter.evaluate_rules();
    myInterpreter.evaluate_queries();
    std::cout << myInterpreter.toString() << std::endl;
    

    
    
    
    return 0;
}
*/


int main(int argc, const char* argv[]) { //this main is for compiling in terminal and enables bash args
   
    Scanner myScanner;
    DatalogProgram myProgram;
    std::ifstream strm(argv[1]);
    std::ofstream output(argv[2]);
    myProgram.parse(myScanner.get_tokens(strm));
    Interpreter myInterpreter(myProgram);
    myInterpreter.evaluate_schemes();
    myInterpreter.evaluate_facts();
    myInterpreter.evaluate_rules();
    myInterpreter.evaluate_queries();
    output << myInterpreter.toString();
    
    return 0;
}
 
