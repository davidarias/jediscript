#ifndef __SCANNER_H
#define __SCANNER_H

#include "common.h"

namespace jediscript{

class Scanner{

public:
    Scanner( std::string source );
    char getNextChar();
    char getCurrentChar();
    char peek();
    bool eof();
private:
    char currentChar;
    std::string source;
    unsigned int position;
    unsigned int len;

};

}
#endif
