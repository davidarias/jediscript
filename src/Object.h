#ifndef __OBJECT_H
#define __OBJECT_H

#include "common.h"
#include "Exceptions.h"

namespace jediscript{

    class Object;
    typedef std::shared_ptr<Object> SObject;

    enum Type{
        TYPE_NIL,
        TYPE_INTEGER,
        TYPE_DOUBLE,
        TYPE_BOOL,
        TYPE_STRING,
        TYPE_SYMBOL,
        TYPE_LIST,
        TYPE_VECTOR,
        TYPE_FUNCTION,
        TYPE_MACRO,
        TYPE_FUNCTION_NATIVE,
        TYPE_FUNCTION_SPECIAL
    };

    class Environment;
    typedef SObject (*NativeFunctionPointer)(std::vector<SObject>& args, Environment& env);

    class Object : public std::enable_shared_from_this<Object> {
    private:
        
        Type type;
                
        union {
            long long integerValue;
            long double doubleValue;
            bool boolValue;
            NativeFunctionPointer nativeFunctionValue;
        };
        std::string stringValue;
        std::vector<SObject> listValue;

    public:

        static SObject Nil();
        static SObject Integer(long long value);
        static SObject Double(long double value);
        static SObject Bool(bool value);
        static SObject String(const std::string& value);
        static SObject Symbol(const std::string& value);
        static SObject List(const std::vector<SObject>& value);
        static SObject Vector(const std::vector<SObject>& value);
        static SObject Function(const std::vector<SObject>& value);
        static SObject Macro(const std::vector<SObject>& value);
        static SObject NativeFunction(NativeFunctionPointer value);
        static SObject SpecialFunction(NativeFunctionPointer value);
        
        Object();
        Object(long long value);
        Object(long double value);
        Object(bool value);
        Object(const std::string& value, Type t);
        Object(const std::vector<SObject>& value, Type t) ;
        Object(NativeFunctionPointer value, Type t);

        Type getType();
        bool is( Type t );
        
        SObject eval(Environment& env);
        std::string stringRepresentation();


        long long toInt();
        long double toDouble();
        bool toBool();
        NativeFunctionPointer toNativeFunction();
        std::string& toString();
        std::vector<SObject>& toVector();

    };

    SObject head(std::vector<SObject>& list);
    std::vector<SObject> tail(std::vector<SObject>& list);
    void concat(std::vector<SObject>& list1, std::vector<SObject>& list2);
    
    SObject apply(SObject fn, std::vector<SObject>& args, Environment& env );

}

#endif
