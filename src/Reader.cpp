#include "Reader.h"

namespace jediscript{

    // initalize symbol table
    std::unordered_map<std::string, SObject> Reader::symbolTable = std::unordered_map<std::string, SObject>();

    SObject Reader::getOrcreateSymbol(const std::string& symbol){
        if( SObject s = symbolTable[ symbol ]){
            return s;
        }
        SObject newSymbol = Object::Symbol( symbol );
        symbolTable[ symbol ] = newSymbol;
        return newSymbol;
    }

    bool isDoubleQuote( char character ){
        return character == '\"';
    }

    bool isQuote( char character ){
        return character == '\'';
    }

    bool isBackquote( char character ){
        return character == '`';
    }

    bool isUnquote( char character ){
        return character == '~';
    }

    bool isSplice( char character ){
        return character == '@';
    }

    bool isValidCharForSymbol( char character ){
        if ( std::isalpha( character ) ||  std::isdigit( character ) ){
            return true;
        }
        switch (character){
        case '+':
        case '-':
        case '*':
        case '/':
        case '_':
        case '>':
        case '<':
        case '=':
        case '!':
        case ':':
        case '.':
        case '#':
        case '&':
        case '%':
        case '$':
        case '|':
        case '?':
            return true;
        default:
            return false;
        }
    }

    Reader::Reader(){

    }

    SObject Reader::readString( Scanner& scanner ){
        char currentChar = scanner.getNextChar();
        buffer.clear();

        do{
            buffer.push_back( currentChar );
            currentChar = scanner.getNextChar();

        }while( ! isDoubleQuote( currentChar ) && !scanner.eof() );

        return Object::String( buffer );
    }



    SObject Reader::readSymbol( Scanner& scanner ){
        char currentChar = scanner.getCurrentChar();
        buffer.clear();

        do{

            buffer.push_back( currentChar );
            currentChar = scanner.getNextChar();

        }while( isValidCharForSymbol( currentChar ) );

        return getOrcreateSymbol( buffer );
    }

    SObject Reader::readNumber( Scanner& scanner ){
        char currentChar = scanner.getCurrentChar();
        buffer.clear();
        bool isDouble = false;
        do{
            if ( currentChar == '.'){
                isDouble = true;
            }
            buffer.push_back( currentChar );
            currentChar = scanner.getNextChar();

        }while( std::isdigit( currentChar ) || currentChar == '.' );

        if ( isDouble ){
            return Object::Double( std::stod( buffer ) );
        }else{
            return Object::Integer( std::stoi ( buffer ) );
        }
        
    }

    char Reader::skipSpaces( Scanner& scanner ){

        char currentChar = scanner.getCurrentChar();

        if ( isspace( currentChar ) ){
            do{
                currentChar = scanner.getNextChar();
            }while( isspace( currentChar ) );
        }

        return currentChar;
    }

    char Reader::skipComments( Scanner& scanner ){

        char currentChar = scanner.getCurrentChar();

        if ( currentChar == ';' ){
            do{
                currentChar = scanner.getNextChar();
            }while( currentChar != '\n' && ! scanner.eof() );
        }

        return currentChar;
    }

    // wraps the expresion in a function call ( fnName expr )
    // and insert it into the list
    void Reader::wrapNextExpresion(std::vector<SObject> expr, const std::string& fnName, std::vector<SObject>& list ){

        if ( expr.empty() ){
            return;
        }
        // build the function call  expresion (fnName ...)
        std::vector<SObject> fnCall;
        fnCall.push_back( getOrcreateSymbol( fnName ) );
        // only put into function call the first next expresion
        fnCall.push_back( expr.front() );
        
        list.push_back( Object::List( fnCall ) );

        // add to the list the rest of expresions, if any
        list.insert( list.end(), std::next( expr.begin()), expr.end() );
    }

    std::vector<SObject> Reader::read( Scanner& scanner ){
        std::vector<SObject> list;
        char currentChar = scanner.getNextChar();

        while( !scanner.eof() ){

            this->skipSpaces( scanner );
            this->skipComments( scanner );

            currentChar = scanner.getCurrentChar();

            if ( isQuote ( currentChar )){
                // quote syntax sugar
                wrapNextExpresion( read( scanner ), "quote", list );
                
            } else if( isBackquote( currentChar) ){
                // backquote syntax sugar
                wrapNextExpresion( read( scanner ), "backquote", list );
                
            }else if( isUnquote( currentChar) ){
                // unquote-slice sugar
                // std::cout << "peek: " << scanner.peek() << std::endl;
                if ( isSplice( scanner.peek() ) ){
                    
                    wrapNextExpresion( read( scanner ), "unquote-splice", list );
                    
                }else{
                    // unquote sugar
                    wrapNextExpresion( read( scanner ), "unquote", list );
                }
                
            }else if ( std::isdigit( currentChar ) || ( currentChar == '-' && std::isdigit( scanner.peek() ) ) ){

                list.push_back( this->readNumber( scanner ) );
                // current char is at the next non-digit char
                currentChar = scanner.getCurrentChar();

            } else if( isValidCharForSymbol( currentChar ) ){
                list.push_back( this->readSymbol( scanner ) );
                // current char is at the next non-alpha char
                currentChar = scanner.getCurrentChar();

            } else if( isDoubleQuote( currentChar )) {

                list.push_back( this->readString( scanner ) );
                // current char is at the ending quotes
                currentChar = scanner.getNextChar();

            }else if( currentChar == '(' ){

                list.push_back( Object::List( this->read( scanner ) ) );
                // current char is at the ending paren
                currentChar = scanner.getNextChar();

            }else if( currentChar == ')' ){

                break;

            }else if( currentChar == '[' ){

                list.push_back( Object::Vector( this->read( scanner ) ) );
                // current char is at the ending paren
                currentChar = scanner.getNextChar();

            }else if( currentChar == ']' ){

                break;

            }else{
                // ignore char
                currentChar = scanner.getNextChar();

            }

        }

        return list;

    }

    std::vector<SObject> Reader::read( std::string source ){
        Scanner scanner( source );
        return this->read( scanner );
    }

}
