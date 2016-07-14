#ifndef __READER_H
#define __READER_H

#include "common.h"
#include "Object.h"
#include "Scanner.h"

namespace jediscript{

class Reader{

public:
    Reader();
    std::vector<SObject> read( std::string source );

private:
    static std::unordered_map<std::string, SObject> symbolTable;
    static SObject getOrcreateSymbol(const std::string& symbol);

    std::string buffer;
    
    std::vector<SObject> read( Scanner& scanner );
    SObject readNumber( Scanner& scanner );
    SObject readString( Scanner& scanner );
    SObject readSymbol( Scanner& scanner );


    char skipSpaces( Scanner& scanner );
    char skipComments( Scanner& scanner );
    void wrapNextExpresion(std::vector<SObject> expr, const std::string& fnName, std::vector<SObject>& list );

};

}
#endif
