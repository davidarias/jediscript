#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "Object.h"
#include "Reader.h"
#include "Environment.h"
#include "builtins.h"

namespace jediscript {

    class Interpreter {
    public:
        static Interpreter& instance() {
            static Interpreter i; 
            return i;
        }

        void load( std::string filePath );
        void loadScript( std::string filePath );
        void repl( std::string prompt );
        SObject eval( std::vector<SObject>& code );
        SObject evalString(std::string sourceCode, std::string filePath = "");


    private:
        Interpreter( );
        Interpreter(const Interpreter& ) = delete;
        void operator=(const Interpreter& ) = delete;

        
        Environment env;
        Reader reader;
        
        std::string jedipath;
    };

}

#endif
