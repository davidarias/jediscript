# JediScript

## What?

A simple lisp interpreter with clojure-like syntax.

## Why?

![xkcd](http://imgs.xkcd.com/comics/lisp_cycles.png "xkcd lisp")

## Ok, show me the code!

```clojure
(defn say-hello-to [name]
  (print "Hello " name))

(say-hello-to "Obi-Wan Kenobi") ;; this should print "Hello Obi-Wan Kenobi" without the quotes
```

Check the file scripts/examples.cljs for more examples

## Awesome! How do I use it?

### Requirements

* A ```C++``` compiler with ```C++11``` support ( I tested it with ```g++ 5.3.1``` )
* [Scons](http://scons.org/) ( a better construction tool that use python scripts )

### Compile the source

```bash
$ scons
```
### Setting the JEDIPATH environment variable

The interpreter needs this environment variable to search for libs ( at the moment just the core lib ).

Navigate to the project folder:
```bash
$ cd jediscript-master/
$ export JEDIPATH=$PWD/libs
```

### Executing the examples
```bash
$ cd jediscript-master/
$ ./bin/jediscript ./scripts/examples.cljs
```

## Missing features ( that maybe some day I'll implement )

* Hash type
* namespaces
* Multi-arity functions
* varadic functions
* FFI
* a lot of nice clojure features...

## FAQ

### It will work on Windows or Mac?
Probably, but since I don't use Windows nor Mac, I can't tell you for sure. If you want to test it in those platforms and/or make adjustment to make it work, you are welcome to open a pull request :)

### How fast is it?
Well, is an AST interpreter with no special care for performance, so slow.
