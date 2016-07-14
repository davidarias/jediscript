#include <cstdio> 

#include "Scanner.h"

namespace jediscript{

    Scanner::Scanner( std::string source ){
        this->source = source;
        this->position = 0;
        this->len = source.length();
    }

    char Scanner::getNextChar(){
        char c = '\0';

        if ( this->position < this->len ){
            c = this->source.at(this->position);
            this->position++;
        }
        this->currentChar = c;
        return c;
    }

    char Scanner::getCurrentChar(){
        return this->currentChar;
    }

    char Scanner::peek(){
        char c = '\0';
        if ( this->position < this->len ){
            // position points to next char
            c = this->source.at(this->position);
        }
        
        return c;
    }

    bool Scanner::eof(){
        if ( this->currentChar == '\0' ||
             this->position > this->len  /*just in case..*/){
            return true;
        }

        return false;
    }

}
