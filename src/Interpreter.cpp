#include "Interpreter.h"

#ifdef DEBUG
#include <chrono>
#endif

namespace jediscript {

    Interpreter::Interpreter() {

        if ( const char* path = getenv( "JEDIPATH" )) {
            jedipath = path;
        }else{
            std::cout << "WARNING: JEDIPATH is not set" << std::endl;
        }

        env.set( "nil"           , Object::Nil() );
        env.set( "true"          , Object::Bool( true ) );
        env.set( "false"         , Object::Bool( false ) );

        env.set( "define-symbol" , Object::NativeFunction( defineSymbol ) );
        env.set( "set-symbol!"   , Object::NativeFunction( setSymbolValue ) );
        env.set( "list"          , Object::NativeFunction( list ) );
        env.set( "macroexpand"   , Object::NativeFunction( macroexpand ) );
        env.set( "eval"          , Object::NativeFunction( evalFn ) );

        env.set( "load"          , Object::NativeFunction( loadFn ) );
        env.set( "load-script"   , Object::NativeFunction( loadScriptFn ) );

        env.set( "nth"           , Object::NativeFunction( nth ) );
        env.set( "head"          , Object::NativeFunction( headFn ) );
        env.set( "tail"          , Object::NativeFunction( tailFn ) );
        env.set( "push!"         , Object::NativeFunction( push_D ) );
        env.set( "pop!"          , Object::NativeFunction( pop_D ) );
        env.set( "concat!"       , Object::NativeFunction( concat_D ) );

        env.set( "floor"         , Object::NativeFunction( floorFn ) );
        env.set( "ceil"          , Object::NativeFunction( ceilFn ) );
        env.set( "log"           , Object::NativeFunction( logFn ) );
        env.set( "log10"         , Object::NativeFunction( log10Fn ) );
        env.set( "sqrt"          , Object::NativeFunction( sqrtFn ) );

        env.set( "%"             , Object::NativeFunction( moduleFn ) );
        env.set( "+"             , Object::NativeFunction( plus ) );
        env.set( "-"             , Object::NativeFunction( minus ) );
        env.set( "*"             , Object::NativeFunction( multiply ) );
        env.set( "/"             , Object::NativeFunction( divide ) );

        env.set( ">"             , Object::NativeFunction( gt ) );
        env.set( "<"             , Object::NativeFunction( lt ) );
        env.set( "=="            , Object::NativeFunction( eq ) );

        env.set( "not"           , Object::NativeFunction( notFn ) );
        env.set( "and"           , Object::NativeFunction( andFn ) );
        env.set( "or"            , Object::NativeFunction( orFn ) );

        env.set( "print"         , Object::NativeFunction( print ) );

        env.set( "let"           , Object::SpecialFunction( letFn ) );
        env.set( "while"         , Object::SpecialFunction( whileFn ) );
        env.set( "if"            , Object::SpecialFunction( ifFn ) );
        env.set( "quote"         , Object::SpecialFunction( quote ) );
        env.set( "backquote"     , Object::SpecialFunction( backquote ));
        env.set( "fn"            , Object::SpecialFunction( fn ));
        env.set( "macro"         , Object::SpecialFunction( macro ) );

    }

    void Interpreter::load(std::string filePath){
        loadScript( jedipath + "/" + filePath );
    }

    void Interpreter::loadScript(std::string filePath){
        std::ifstream fileReader(  filePath  );

        std::string source = "";

        if ( fileReader ){
            while ( fileReader.good() ){
                std::string line;
                std::getline ( fileReader, line );
                line += "\n";
                source += line;
            }

            SObject result;

            result = evalString( source, filePath );

        }else{

            std::cout << "File " << filePath << " does not exist." << std::endl;
        }


        fileReader.close ();
    }

    void replIntro(){

        std::cout << "JediScript v0.1 By the Jedi ( Code ) Master David Arias. Copyleft 2016." << std::endl;
        std::cout << std::endl;

        // The Jedi code
        std::cout << "There is no emotion, there is peace." << std::endl;
        std::cout << "There is no ignorance, there is knowledge." << std::endl;
        std::cout << "There is no passion, there is serenity." << std::endl;
        std::cout << "There is no chaos, there is harmony." << std::endl;
        std::cout << "There is no death, there is the Force." << std::endl;

        std::cout << std::endl;
    }

    void Interpreter::repl(std::string prompt){

        SObject result;

        std::string input;

        replIntro();

        do{
            std::cout << prompt;
            std::getline(std::cin, input, '\n');

            result = evalString( input, "REPL" );

            std::cout << result->stringRepresentation() << std::endl;

        } while (!std::cin.eof());

    }

    SObject Interpreter::evalString(std::string sourceCode, std::string filePath){
        SObject result = env.get("nil");

        try{
#ifdef DEBUG

            auto t1 = std::chrono::high_resolution_clock::now();

            auto code = reader.read( sourceCode );

            auto t2 = std::chrono::high_resolution_clock::now();

            result = eval( code );

            auto t3 = std::chrono::high_resolution_clock::now();

            std::chrono::duration<float> readerTime =  t2 - t1;
            std::chrono::duration<float> interpreterTime =  t3 - t2;
            std::chrono::duration<float> totalTime =  t3 - t1;

            std::cout << std::endl;
            std::cout << "--------------------- DEBUG ------------------"        << std::endl;
            std::cout << "Execution time of " << filePath                        << std::endl;
            std::cout << "Reader Time:      " << readerTime.count()      << " s" << std::endl;
            std::cout << "Interpreter Time: " << interpreterTime.count() << " s" << std::endl;
            std::cout << "Total:            " << totalTime.count()       << " s" << std::endl;
            std::cout << "--------------------- DEBUG ------------------"        << std::endl;
            std::cout << std::endl;

#else

            auto code = reader.read( sourceCode );
            result = eval( code  );
#endif
        }catch (const std::exception& e) {

            std::cout << filePath << ": " << e.what() << std::endl;

        }

        return result;
    }

    SObject Interpreter::eval(std::vector<SObject>& code){
        if( code.empty() ){
            return env.get("nil");
        }
        SObject result = env.get("nil");

        SObject last = code.back();
        code.pop_back();

        std::vector<SObject>::iterator iterator = code.begin();

        // loop until next-to-last
        for (; iterator != code.end(); iterator++) {

            (*iterator)->eval( env );

        }
        // return the last element
        result = last->eval( env );


        return result;

    }

}
