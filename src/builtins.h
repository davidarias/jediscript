#ifndef BUILTINS_H_
#define BUILTINS_H_

#include "Object.h"
#include "Environment.h"
#include "Interpreter.h"

namespace jediscript{

    SObject quote(std::vector<SObject>& rawArgs, Environment& env);
    SObject backquote(std::vector<SObject>& rawArgs, Environment& env);

    SObject loadFn(std::vector<SObject>& rawArgs, Environment& env);
    SObject loadScriptFn(std::vector<SObject>& rawArgs, Environment& env);

    SObject defineSymbol(std::vector<SObject>& args, Environment& env);
    SObject setSymbolValue(std::vector<SObject>& args, Environment& env);

    SObject fn(std::vector<SObject>& args, Environment& env);
    SObject macro(std::vector<SObject>& args, Environment& env);

    SObject list(std::vector<SObject>& args, Environment& env);
    SObject macroexpand(std::vector<SObject>& args, Environment& env);
    SObject evalFn(std::vector<SObject>& args, Environment& env);

    SObject nth(std::vector<SObject>& args, Environment& env);
    SObject headFn(std::vector<SObject>& args, Environment& env);
    SObject tailFn(std::vector<SObject>& args, Environment& env);
    SObject push_D(std::vector<SObject>& args, Environment& env);
    SObject pop_D(std::vector<SObject>& args, Environment& env);
    SObject concat_D(std::vector<SObject>& args, Environment& env);

    SObject floorFn(std::vector<SObject>& args, Environment& env);
    SObject ceilFn(std::vector<SObject>& args, Environment& env);
    SObject logFn(std::vector<SObject>& args, Environment& env);
    SObject log10Fn(std::vector<SObject>& args, Environment& env);
    SObject sqrtFn(std::vector<SObject>& args, Environment& env);
    SObject moduleFn(std::vector<SObject>& args, Environment& env);

    SObject plus(std::vector<SObject>& args, Environment& env);
    SObject minus(std::vector<SObject>& args, Environment& env);
    SObject multiply(std::vector<SObject>& args, Environment& env);
    SObject divide(std::vector<SObject>& args, Environment& env);

    SObject gt(std::vector<SObject>& args, Environment& env);
    SObject lt(std::vector<SObject>& args, Environment& env);
    SObject eq(std::vector<SObject>& args, Environment& env);

    SObject notFn(std::vector<SObject>& args, Environment& env);
    SObject andFn(std::vector<SObject>& args, Environment& env);
    SObject orFn(std::vector<SObject>& args, Environment& env);

    SObject print(std::vector<SObject>& args, Environment& env);

    SObject ifFn(std::vector<SObject>& args, Environment& env);

    SObject letFn(std::vector<SObject>& args, Environment& env);

    SObject whileFn(std::vector<SObject>& args, Environment& env);

}

#endif
