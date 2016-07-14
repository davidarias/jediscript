#include "Environment.h"


namespace jediscript{

    Environment::Environment(){
        prototype = NULL;
    }


    void Environment::set(const std::string& key, SObject value){
        this->map[key] = value;
    }

    SObject Environment::get(const std::string& key){
        SObject value = map[ key ];
        if ( value ){
            return value;
        }else if ( prototype ){
            return prototype->get( key );
        }
        throw KeyNotFound( key );
    }

    void Environment::setPrototype(Environment* prototype){
        this->prototype = prototype;
    }
}
