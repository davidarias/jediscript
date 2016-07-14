#include "Interpreter.h"


int main(int argc, char* argv[]){
    jediscript::Interpreter& interpreter = jediscript::Interpreter::instance();

    interpreter.load( "core.cljs" );

    if ( argc == 1 ){
       interpreter.repl( "(JediScript) " );
    }else{
        std::string filePath = argv[1];
        interpreter.loadScript( filePath );
 
    }
    return 0;
}
