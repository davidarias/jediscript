#include "Object.h"
#include "Environment.h"
#include "builtins.h"

namespace jediscript{

    Object::Object()                                          : type(TYPE_NIL) {}
    Object::Object(long long value)                           : type(TYPE_INTEGER)        , integerValue(value) {}
    Object::Object(long double value)                         : type(TYPE_DOUBLE)         , doubleValue(value) {}
    Object::Object(bool value)                                : type(TYPE_BOOL)           , boolValue(value) {}
    Object::Object(const std::string& value, Type t)          : type(t)                   , stringValue(value) {}
    Object::Object(const std::vector<SObject>& value, Type t) : type(t)                   , listValue(value) {}
    Object::Object(NativeFunctionPointer value, Type t)       : type(t)                   , nativeFunctionValue(value) {}


    SObject Object::Nil(){
        return std::make_shared<Object>();
    }

    SObject Object::Integer(long long value){
        return std::make_shared<Object>(value);
    }

    SObject Object::Double(long double value){
        return std::make_shared<Object>(value);
    }

    SObject Object::Bool(bool value){
        return std::make_shared<Object>(value);
    }

    SObject Object::String(const std::string& value){
        return std::make_shared<Object>(value, TYPE_STRING);
    }

    SObject Object::Symbol(const std::string& value){
        return std::make_shared<Object>(value, TYPE_SYMBOL);
    }

    SObject Object::List(const std::vector<SObject>& value){
        return std::make_shared<Object>(value, TYPE_LIST);
    }

    SObject Object::Vector(const std::vector<SObject>& value){
        return std::make_shared<Object>(value, TYPE_VECTOR);
    }

    SObject Object::Function(const std::vector<SObject>& value){
        return std::make_shared<Object>(value, TYPE_FUNCTION);
    }

    SObject Object::Macro(const std::vector<SObject>& value){
        return std::make_shared<Object>(value, TYPE_MACRO);
    }

    SObject Object::NativeFunction(NativeFunctionPointer value){
        return std::make_shared<Object>(value, TYPE_FUNCTION_NATIVE);
    }

    SObject Object::SpecialFunction(NativeFunctionPointer value){
        return std::make_shared<Object>(value, TYPE_FUNCTION_SPECIAL);
    }

    Type Object::getType(){
        return type;
    }

    bool Object::is( Type t ){
        return t == type;
    }

    long long Object::toInt(){
        switch(type){
        case TYPE_INTEGER:
            return integerValue;
        case TYPE_DOUBLE:
            return (long long) doubleValue;
        default:
            throw RuntimeException("Invalid conversion to int");
        }
    }

    long double Object::toDouble(){
        switch(type){
        case TYPE_INTEGER:
            return (long double) integerValue;
        case TYPE_DOUBLE:
            return doubleValue;
        default:
            throw RuntimeException("Invalid conversion to double");
        }
    }

    bool Object::toBool(){
        if ( type == TYPE_BOOL ){
            return boolValue;
        }else{
            throw RuntimeException("Invalid conversion to bool");
        }
    }

    NativeFunctionPointer Object::toNativeFunction(){
        switch(type){
        case TYPE_FUNCTION_NATIVE:
        case TYPE_FUNCTION_SPECIAL:
            return nativeFunctionValue;
        default:
            throw RuntimeException("Invalid conversion to NativeFunction");
        }
    }

    std::string& Object::toString(){
        switch(type){
        case TYPE_STRING:
        case TYPE_SYMBOL:
            return stringValue;
        default:
            throw RuntimeException("Invalid conversion to string");
        }
    }

    std::vector<SObject>& Object::toVector(){
        switch(type){
        case TYPE_LIST:
        case TYPE_VECTOR:
        case TYPE_FUNCTION:
        case TYPE_MACRO:
            return listValue;
        default:
            throw RuntimeException("Invalid conversion to list");
        }
    }

    SObject head(std::vector<SObject>& list){
        return list.front();
    }

    std::vector<SObject> tail(std::vector<SObject>& list){
        std::vector<SObject> tail;
        tail.insert( tail.end(), std::next( list.begin()), list.end() );
        return tail;
    }

    void concat(std::vector<SObject>& list1, std::vector<SObject>& list2){
        list1.insert( list1.end(), list2.begin(), list2.end() );
    }

    SObject apply(SObject fn, std::vector<SObject>& args, Environment& env ){
        switch( fn->getType() ){
        case TYPE_FUNCTION:
        case TYPE_MACRO:
            {
                // build local scope
                Environment localScope;
                localScope.setPrototype( &env );
                std::vector<SObject>& argNames = fn->toVector().front()->toVector();

                std::vector<SObject>::iterator argNamesIter = argNames.begin();
                std::vector<SObject>::iterator argsIter = args.begin();

                while( argNamesIter != argNames.end() && argsIter != args.end() ){
                    localScope.set( (*argNamesIter)->toString(), (*argsIter) );
                    argsIter++; argNamesIter++;
                }

                // eval and return body
                return fn->toVector().at(1)->eval( localScope );
            }

        case TYPE_FUNCTION_NATIVE:
            try {
                return fn->toNativeFunction()( args, env );
            }catch (const std::out_of_range& e) {
                throw RuntimeException("Incorrect number of parameters");
            }

        default:
            throw RuntimeException("Function expected"); ;
        }
    }

    SObject evalCallExpr(std::vector<SObject>& callExpr, Environment& env ){
        // resolve symbol of first element of list ( the function name )
        SObject fn = head( callExpr )->eval( env );
        std::vector<SObject> args = tail( callExpr );

        switch( fn->getType() ){
        case TYPE_FUNCTION:
        case TYPE_FUNCTION_NATIVE:
            {
                // eval arguments
                auto eval = [&](SObject o){ return o->eval( env ); };
                std::transform(args.begin(), args.end(), args.begin(), eval );

                return apply( fn, args, env );
            }
        case TYPE_MACRO:
            // macroexpand is the same as calling a function but without evaling args
            {
                // the result of macroexpand is evaled
                SObject expandedMacro = apply(fn, args, env);
                return expandedMacro->eval( env );
            }
        case TYPE_FUNCTION_SPECIAL:
            // special functions receive args without eval
            try {
                return fn->toNativeFunction()( args, env );
            }catch (const std::out_of_range& e) {
                throw RuntimeException("Incorrect number of parameters");
            }
        default:
            throw RuntimeException("Function expected");
        }
    }

    SObject Object::eval(Environment& env){
        switch( type ){
        case TYPE_NIL:
        case TYPE_INTEGER:
        case TYPE_DOUBLE:
        case TYPE_BOOL:
        case TYPE_STRING:
            return shared_from_this();
        case TYPE_SYMBOL:
            return env.get( stringValue );
        case TYPE_LIST:
            {
                // function call
                if ( listValue.empty() ){
                    return shared_from_this();
                }

                return evalCallExpr( listValue, env );

            }
        case TYPE_VECTOR:
            {
                // vector eval all its members
                std::vector<SObject> evaledVector;
                std::vector<SObject>::iterator iterator = listValue.begin();
                for (; iterator != listValue.end(); iterator++) {
                    evaledVector.push_back( (*iterator)->eval( env ) );
                }
                return Object::Vector( evaledVector );
            }
        default:
            return shared_from_this();

        }
        return Object::Nil();
    }

    std::string Object::stringRepresentation(){
        switch( type ){
        case TYPE_NIL:
            return "nil";
        case TYPE_INTEGER:
            return std::to_string( integerValue );
        case TYPE_DOUBLE:
            return std::to_string( doubleValue );
        case TYPE_BOOL:
            return ( boolValue ? "true" : "false" );
        case TYPE_STRING:
            return "\"" + stringValue + "\"";
        case TYPE_SYMBOL:
            return stringValue;
        case TYPE_LIST:
            {
                std::string listString = "( ";
                std::vector<SObject>::iterator iterator = listValue.begin();
                for (; iterator != listValue.end(); iterator++) {
                    listString += (*iterator)->stringRepresentation();
                    listString += ' ';
                }
                listString += ")";
                return listString;
            }
        case TYPE_VECTOR:
            {
                std::string listString = "[ ";
                std::vector<SObject>::iterator iterator = listValue.begin();
                for (; iterator != listValue.end(); iterator++) {
                    listString += (*iterator)->stringRepresentation();
                    listString += ' ';
                }
                listString += "]";
                return listString;
            }
        case TYPE_FUNCTION:
            return "Function";
        case TYPE_MACRO:
            return "Macro";
        case TYPE_FUNCTION_NATIVE:
            return "NativeFunction";
        default:
            return "Unkown type D:";

        }
    }



}
