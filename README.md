# Jupiter Programming Language

Jupiter is A dynamic object-oriented programming language based on Smalltalk/Self.

## Features

* A set of simple core types:
  - Decimal floating point Numbers: See: [https://0.30000000000000004.com/](https://0.30000000000000004.com/).
  - Strings.
  - Arrays: Number indexed collections.
  - Maps: String indexed collections.
* Immutable data structures.
* There are no classes. Objects are created copying other objects ( prototypes ).
  Thanks to immutability no special semantics is needed to achieve this.
* Unix friendly: Its source code is stored in files, which is not the 'Smalltalk way',
  but this allow us easy interaction with other Unix tools, like git, grep, IDE's/text editors etc..
  See [Juno IDE](https://github.com/davidarias/juno).
* Generational garbage collector: The immutable data structures generate a lot a short-lived objects,
  using a generational GC improves the performance of collecting and reusing these temporal objects.
* Tail call optimization: Recursive methods are the only way to make loops in Jupiter. This optimization
  make tail recursive calls as efficient as a imperative loops.
* Compile inline if expressions. If expressions can be implemented as a regular message/method call in Smalltalk,
  but this is not very efficient, so Jupiter transform if expression into jump bytecodes.

## Juno IDE

For a better programming experience (or at least more similar to other Smalltalk environments),
you can use the [Juno IDE](https://github.com/davidarias/juno)


## Some examples

```smalltalk
"The traditional hello world program"
'Hello world!' print
```


```smalltalk
"double a list of numbers"
numbers := {1,2,3}.
double := numbers map: [ :number | number * 2 ].
double == {2,4,6}
```

```smalltalk
"sum two point objects"
aPoint := 1 @ 2. otherPoint := 2 @ 2. sum := aPoint + otherPoint
```

Check the tests ( in lib/core/src/core/tests/ ) for more examples

## Differences with other Smalltalk systems

### Syntax
The Jupiter syntax is mostly the same as other smalltalk implementations, with a few differences:

* No need to declare local variables
* Local variables cannot be reassigned ( a := a + 1 won't work )
* No Static Array syntax
* Elements in dynamic Arrays are separated by comas
* No return symbol. Methods return the last evaluated expression.

### Objects

The object model is similar to [Self](http://www.selflanguage.org/). Jupiter has no classes, all objects
are created copying other objects.

An Object in Jupiter is just an immutable [persistent](https://en.wikipedia.org/wiki/Persistent_data_structure) key-value collection of objects.
The messages sent to objects don't modify the state of the object, instead return a new object with the updated state, therefore creating instances
of objects ( cloning or copying prototypes ) is trivial.


## Build requirements

- A ```C++``` compiler with ```C++14``` support ( I tested it with ```g++ 7.2.0``` )
- [Scons](http://scons.org/) ( a better construction tool that use python scripts )
- mpdecimal http://www.bytereef.org/mpdecimal/ (sudo apt-get install libmpdec-dev for Ubuntu/Debian users)
- immer https://github.com/arximboldi/immer

## Setting the JUPITERHOME environment variable

The interpreter needs this environment variable to load the core library.
(In the future this won't be necesary, when proper installation scrips and/or packages are provided )

Navigate to the project folder:
```bash
$ cd jupiter-master/
$ export JUPITERHOME=$PWD/lib
```

## Docs and Tutorial

Coming soon...


## Status

This project is yet in alpha state, so it may crash/leak memory/explode from time to time.

## License

This project is licensed under the MPL-2.0 License. See the LICENSE file for details
