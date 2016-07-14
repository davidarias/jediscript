#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Object.h"

namespace jediscript {

    class Environment {
    public:
        Environment();

        void set(const std::string& key, SObject value);
        SObject get(const std::string& key);

        void setPrototype(Environment* prototype);
    private:
        Environment* prototype;
        std::unordered_map <std::string, SObject> map;
    };

}

#endif
