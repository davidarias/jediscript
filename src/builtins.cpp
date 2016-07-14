#include "builtins.h"

namespace jediscript{

    SObject loadFn(std::vector<SObject>& args, Environment& env){
        Interpreter& interpreter = Interpreter::instance();

        interpreter.load( args.at(0)->toString() );

        return env.get("nil");
    }

    SObject loadScriptFn(std::vector<SObject>& args, Environment& env){
        Interpreter& interpreter = Interpreter::instance();

        interpreter.loadScript( args.at(0)->toString() );

        return env.get("nil");
    }


    SObject quote(std::vector<SObject>& rawArgs, Environment& env){
        return rawArgs.at(0);
    }

    bool isCallExpr(SObject expr){
        if ( expr->is( TYPE_LIST ) ){
            if ( head( expr->toVector() )->is( TYPE_SYMBOL ) ){
                return true;
            }
        }
        return false;
    }

    SObject backquote(SObject expr, Environment& env);

    SObject backquoteList(std::vector<SObject>& list, Environment& env){
        std::vector<SObject> returnList;

        std::vector<SObject>::iterator iterator = list.begin();
        for (; iterator != list.end(); iterator++) {
            SObject nextExpr = (*iterator);

            if ( isCallExpr( nextExpr ) ){

                std::string symbol = head( nextExpr->toVector() )->toString();
                SObject expr = nextExpr->toVector().at( 1 );

                if ( symbol == "unquote"){
                    returnList.push_back( expr->eval( env ) );
                    continue;
                }else if( symbol == "unquote-splice" ){
                    concat( returnList, expr->eval( env )->toVector() );
                    continue;
                }
            }
            // the next expresion is not a list or the first element is not a symbol
            // or the symbol is not evaluable inside backquote,
            // so backquote it and push to the result list
            returnList.push_back( backquote( nextExpr, env ) );

        }

        return Object::List( returnList );
    }

    SObject backquote(SObject expr, Environment& env){
        switch( expr->getType() ){
        case TYPE_LIST:
        case TYPE_VECTOR:
            {
                std::vector<SObject>& listExpr = expr->toVector();

                if ( listExpr.empty() ){
                    return expr;
                }else{
                    return backquoteList( listExpr, env );
                }

            }
        default:
            return expr;
        }
    }

    SObject backquote(std::vector<SObject>& rawArgs, Environment& env){

        return backquote( rawArgs.at(0), env );
    }

    SObject floorFn(std::vector<SObject>& args, Environment& env){
        return Object::Double( floor( args.at(0)->toDouble() ) );
    }

    SObject ceilFn(std::vector<SObject>& args, Environment& env){
        return Object::Double( ceil( args.at(0)->toDouble() ) );
    }

    SObject logFn(std::vector<SObject>& args, Environment& env){
        return Object::Double( log( args.at(0)->toDouble() ) );
    }

    SObject log10Fn(std::vector<SObject>& args, Environment& env){
        return Object::Double( log10( args.at(0)->toDouble() ) );
    }

    SObject sqrtFn(std::vector<SObject>& args, Environment& env){
        return Object::Double( sqrt( args.at(0)->toDouble() ) );
    }

    SObject moduleFn(std::vector<SObject>& args, Environment& env){
        return Object::Integer( args.at(0)->toInt() % args.at(1)->toInt() );
    }


    SObject plus(std::vector<SObject>& args, Environment& env){
        long long intResult = 0;
        long double doubleResult = 0.0;
        bool isDoubleSum = false;

        if ( args.size() < 2){
            throw RuntimeException("+ function expects 2 or more parameters.");
        }

        std::for_each(args.begin(), args.end(), [&](SObject n){

            if ( isDoubleSum ){
                doubleResult += n->toDouble();
            }else{
                if ( n->is( TYPE_DOUBLE )){
                    isDoubleSum = true;
                    doubleResult = double( intResult );
                    doubleResult += n->toDouble();
                }else{
                    intResult += n->toInt();
                }
            }
        });

        if ( isDoubleSum ){
            return Object::Double( doubleResult );
        }else{
            return Object::Integer( intResult );
        }
    }

    SObject minus(std::vector<SObject>& args, Environment& env){

        long long intResult = args.at(0)->toInt();
        long double doubleResult = args.at(0)->toDouble();
        bool isDoubleResult = false;
        if ( args.at(0)->is(TYPE_DOUBLE)){
            isDoubleResult = true;
        }

        for (std::vector<SObject>::iterator i = std::next( args.begin() ); i != args.end(); i++) {
            SObject n = (*i);

            if ( isDoubleResult ){
                doubleResult -= n->toDouble();
            }else{
                if ( n->is( TYPE_DOUBLE )){
                    // sum is transformed to double
                    isDoubleResult = true;
                    doubleResult = double( intResult );
                    doubleResult -= n->toDouble();
                }else{
                    intResult -= n->toInt();
                }
            }
        }

        if ( isDoubleResult ){
            return Object::Double( doubleResult );
        }else{
            return Object::Integer( intResult );
        }
    }

    SObject multiply(std::vector<SObject>& args, Environment& env){
        long long intResult = 1;
        long double doubleResult = 1.0;
        bool isDoubleResult = false;

        if ( args.size() < 2){
            throw RuntimeException("* function expects 2 or more parameters.");
        }

        std::for_each(args.begin(), args.end(), [&](SObject n){

            if ( isDoubleResult ){
                doubleResult *= n->toDouble();
            }else{
                if ( n->is( TYPE_DOUBLE )){
                    isDoubleResult = true;
                    doubleResult = double( intResult );
                    doubleResult *= n->toDouble();
                }else{
                    intResult *= n->toInt();
                }
            }
        });

        if ( isDoubleResult ){
            return Object::Double( doubleResult );
        }else{
            return Object::Integer( intResult );
        }
    }

    SObject divide(std::vector<SObject>& args, Environment& env){

        long long intResult = args.at(0)->toInt();
        long double doubleResult = args.at(0)->toDouble();
        bool isDoubleResult = false;
        if ( args.at(0)->is(TYPE_DOUBLE)){
            isDoubleResult = true;
        }

        for (std::vector<SObject>::iterator i = std::next( args.begin() ); i != args.end(); i++) {
            SObject n = (*i);

            if ( isDoubleResult ){
                doubleResult /= n->toDouble();
            }else{
                if ( n->is( TYPE_DOUBLE )){
                    // sum is transformed to double
                    isDoubleResult = true;
                    doubleResult = double( intResult );
                    doubleResult /= n->toDouble();
                }else{
                    intResult /= n->toInt();
                }
            }
        }

        if ( isDoubleResult ){
            return Object::Double( doubleResult );
        }else{
            return Object::Integer( intResult );
        }
    }

    SObject gt(std::vector<SObject>& args, Environment& env){
        // kind of messy compare the diferrent number types...
        switch(args.at(0)->getType()){
        case TYPE_INTEGER:
            switch(args.at(1)->getType()){
            case TYPE_INTEGER:
                if( args.at(0)->toInt() > args.at(1)->toInt() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            case TYPE_DOUBLE:
                if( args.at(0)->toInt() > args.at(1)->toDouble() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            default:
               throw RuntimeException("Cannot convert to number");
            }
        case TYPE_DOUBLE:
            switch(args.at(1)->getType()){
            case TYPE_INTEGER:
                if( args.at(0)->toDouble() > args.at(1)->toInt() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            case TYPE_DOUBLE:
                if( args.at(0)->toDouble() > args.at(1)->toDouble() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            default:
               throw RuntimeException("Cannot convert to number");
            }

        case TYPE_STRING:
            if ( args.at(1)->is( TYPE_STRING )){
                if ( args.at(0)->toString() > args.at(1)->toString() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            }
        case TYPE_SYMBOL:
            if ( args.at(1)->is( TYPE_SYMBOL )){
                if ( args.at(0)->toString() > args.at(1)->toString() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            }
        default:
            throw RuntimeException("Cannot compare imcompatible types");
        }


    }

    SObject lt(std::vector<SObject>& args, Environment& env){
        // kind of messy compare the diferrent number types...
        switch(args.at(0)->getType()){
        case TYPE_INTEGER:
            switch(args.at(1)->getType()){
            case TYPE_INTEGER:
                if( args.at(0)->toInt() < args.at(1)->toInt() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            case TYPE_DOUBLE:
                if( args.at(0)->toInt() < args.at(1)->toDouble() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            default:
                throw RuntimeException("Cannot convert to number");
            }
        case TYPE_DOUBLE:
            switch(args.at(1)->getType()){
            case TYPE_INTEGER:
                if( args.at(0)->toDouble() < args.at(1)->toInt() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            case TYPE_DOUBLE:
                if( args.at(0)->toDouble() < args.at(1)->toDouble() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            default:
                throw RuntimeException("Cannot convert to number");
            }

        case TYPE_STRING:
            if ( args.at(1)->is( TYPE_STRING )){
                if ( args.at(0)->toString() < args.at(1)->toString() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            }
        case TYPE_SYMBOL:
            if ( args.at(1)->is( TYPE_SYMBOL )){
                if ( args.at(0)->toString() < args.at(1)->toString() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            }
        default:
            throw RuntimeException("cannot compare imcompatible types");
        }

    }

    SObject eq(std::vector<SObject>& args, Environment& env){
        if ( args.size() < 2){
            throw RuntimeException("== function expects 2 parameters.");
        }

        // kind of messy compare the diferrent number types...
        switch(args.at(0)->getType()){
        case TYPE_INTEGER:
            switch(args.at(1)->getType()){
            case TYPE_INTEGER:
                if( args.at(0)->toInt() == args.at(1)->toInt() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            case TYPE_DOUBLE:
                if( args.at(0)->toInt() == args.at(1)->toDouble() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            default:
                throw RuntimeException("cannot convert to number");
            }
        case TYPE_DOUBLE:
            switch(args.at(1)->getType()){
            case TYPE_INTEGER:
                if( args.at(0)->toDouble() == args.at(1)->toInt() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            case TYPE_DOUBLE:
                if( args.at(0)->toDouble() == args.at(1)->toDouble() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            default:
                throw RuntimeException("cannot convert to number");
            }
        case TYPE_STRING:
            if ( args.at(1)->is( TYPE_STRING )){
                if ( args.at(0)->toString() == args.at(1)->toString() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            }
        case TYPE_SYMBOL:
            if ( args.at(1)->is( TYPE_SYMBOL )){
                if ( args.at(0)->toString() == args.at(1)->toString() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            }
        case TYPE_BOOL:
            if ( args.at(1)->is( TYPE_BOOL )){
                if ( args.at(0)->toBool() == args.at(1)->toBool() ){
                    return env.get("true");
                }else{
                    return env.get("false");
                }
            }
        case TYPE_NIL:
            if ( args.at(1)->is( TYPE_NIL ) ){
                return env.get("true");
            }else{
                return env.get("false");
            }
        default:
            throw RuntimeException("cannot compare imcompatible types");
        }


    }

    SObject notFn(std::vector<SObject>& args, Environment& env){
        if ( args.at(0)->toBool() ){
            return env.get("false");
        }
        return env.get("true");
    }

    SObject andFn(std::vector<SObject>& args, Environment& env){
        if ( args.at(0)->toBool() && args.at(1)->toBool() ){
            return env.get("true");
        }
        return env.get("false");
    }

    SObject orFn(std::vector<SObject>& args, Environment& env){
        if ( args.at(0)->toBool() || args.at(1)->toBool() ){
            return env.get("true");
        }
        return env.get("false");
    }

    SObject ifFn(std::vector<SObject>& rawArgs, Environment& env){

        if ( rawArgs.at(0)->eval( env )->toBool() ){
            return rawArgs.at(1)->eval( env );
        }else{
            return rawArgs.at(2)->eval( env );
        }

    }

    SObject print(std::vector<SObject>& args, Environment& env){

        std::vector<SObject>::iterator iterator =  args.begin();

        for (; iterator != args.end(); iterator++) {
            if ( (*iterator)->is(TYPE_STRING )){
                std::cout << (*iterator)->toString();
            }else{
                std::cout << (*iterator)->stringRepresentation();
            }
        }
        std::cout << std::endl;
        return env.get("nil");
    }

    SObject list(std::vector<SObject>& args, Environment& env){
        return Object::List( args );
    }

    SObject defineSymbol(std::vector<SObject>& args, Environment& env){
        SObject symbol = args.at(0);
        SObject value = args.at(1);

        env.set( symbol->toString(), value );

        return value;
    }

    SObject setSymbolValue(std::vector<SObject>& args, Environment& env){
        SObject symbol = args.at(0);
        SObject newValue = args.at(1);

        SObject prevValue = env.get( symbol->toString() );
        if ( prevValue->getType() != newValue->getType() ){
            throw RuntimeException("Cannot change type of " + symbol->toString() );
        }

        env.set( symbol->toString(), newValue  );

        return newValue;

    }


    SObject fn(std::vector<SObject>& rawArgs, Environment& env){
        return Object::Function( rawArgs );
    }

    SObject macro(std::vector<SObject>& args, Environment& env){
        return Object::Macro( args );
    }
    
    SObject macroexpand(std::vector<SObject>& args, Environment& env){
        // TODO DRY && check types
        SObject macroExpr = args.at(0);
        SObject macro = macroExpr->toVector().at(0)->eval( env );
        std::vector<SObject> macroArgs;
        macroArgs.insert( macroArgs.end(), std::next( macroExpr->toVector().begin()), macroExpr->toVector().end() );
        return apply(macro, macroArgs, env);
    }

    SObject evalFn(std::vector<SObject>& args, Environment& env){
        return args.at(0)->eval( env );
    }

    SObject nth(std::vector<SObject>& args, Environment& env){
        std::vector<SObject>& l = args.at(1)->toVector();
        long long nth = args.at(0)->toInt();
        if ( l.size() > nth ){
            return l.at( nth );
        }
        throw RuntimeException("Trying to access out of range element");
    }

    SObject headFn(std::vector<SObject>& args, Environment& env){
        std::vector<SObject>& l = args.at(0)->toVector();
        if ( l.empty() ){
            return env.get("nil");
        }

        return head( l );
    }

    SObject tailFn(std::vector<SObject>& args, Environment& env){
        std::vector<SObject>& l = args.at(0)->toVector();
        if ( l.empty() ){
            return env.get("nil");
        }

        return Object::List( tail( l ) );
    }

    SObject push_D(std::vector<SObject>& args, Environment& env){
        std::vector<SObject>& l = args.at(1)->toVector();
        
        l.push_back( args.at(0) );
        return args.at(1);
    }

    SObject pop_D(std::vector<SObject>& args, Environment& env){
        std::vector<SObject>& l = args.at(0)->toVector();
        if ( l.empty() ){
            return env.get("nil");
        }
        SObject element = l.back();
        l.pop_back();
        return element;
    }

    SObject concat_D(std::vector<SObject>& args, Environment& env){
        std::vector<SObject>& l = args.at(0)->toVector();
        concat(args.at(0)->toVector(), args.at(1)->toVector());
        return args.at(0);
    }

    SObject letFn(std::vector<SObject>& args, Environment& env){
        Environment letScope;
        letScope.setPrototype( &env );

        //build the let scope
        std::vector<SObject>& bindings = args.at(0)->toVector();
        std::vector<SObject>::iterator iterator = bindings.begin();
        while (iterator != bindings.end()) {
            // get name
            std::string& name = (*iterator)->toString();
            // get value
            iterator++;
            if ( iterator == bindings.end() ){
                throw RuntimeException("Let bindings must be in pairs");
            }
            letScope.set( name, (*iterator)->eval( letScope ));
            iterator++;
        }
        //eval the expressions and return the last one
        SObject last = args.back();
        args.pop_back();
        // loop until next-to-last
        iterator = std::next( args.begin() );
        while (iterator != args.end() ) {
            (*iterator)->eval( letScope );
            iterator++;
        }
        // return the last expresion
        return  last->eval( letScope );
    }
    
    SObject whileFn(std::vector<SObject>& args, Environment& env){
        SObject test = args.at(0);

        std::vector<SObject>::iterator iterator;
        while ( test->eval( env )->toBool() ) {

            iterator = std::next( args.begin() );
            for(; iterator != args.end(); iterator++ ){
                (*iterator)->eval( env );
            }
        }
        return env.get("nil");
    }
}
