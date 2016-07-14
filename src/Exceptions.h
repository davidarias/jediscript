#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

#include <exception>
#include <stdexcept>

namespace jediscript{

    class RuntimeException : public std::runtime_error {
    public:
        RuntimeException ( std::string what ) : runtime_error( "Runtime error: " + what ){}
    };

    class KeyNotFound : public RuntimeException{
    public:
        KeyNotFound ( std::string key ) : RuntimeException( "Key \'" + key + "\' not found" ){}
    };

}

#endif
