# Jupiter Programming Language

Jupiter is A dynamic object-oriented programming language based on Smalltalk, with a slightly diferent philosophy:

* A set of simple core types: Numbers, Strings, Arrays ( number indexed collections ) and Maps ( String indexed collections ).
* Immutable data structures.
* There are no classes. Objects are created copying other objects ( prototypes ). Thanks to immutability no special semantics is needed to achieve this.
* Code reuse through composition instead of inheritance. Deep hierarchies often are a source of incidental complexity.
* Unix friendly: Its source code is stored in files, which is not the 'Smalltalk way', but this allow us easy interaction with other Unix tools, like git, grep, IDE's/text editors etc..

## Build requirements

### A ```C++``` compiler with ```C++14``` support ( I tested it with ```g++ 7.2.0``` )
### [Scons](http://scons.org/) ( a better construction tool that use python scripts )

### mpdecimal http://www.bytereef.org/mpdecimal/

In Ubuntu and Debian can be installed with:
```bash
 $ sudo apt-get install libmpdec-dev
```
Alternatively compile it from source:
http://www.bytereef.org/mpdecimal/download.html

### immer https://github.com/arximboldi/immer

```bash
$ git clone https://github.com/arximboldi/immer.git
$ cd immer
$ mkdir -p build && cd build
$ cmake .. && sudo make install
```

## Setting the JUPITERHOME environment variable

The interpreter needs this environment variable to load the core library.
(In the future this won't be necesary, when proper installation scrips and/or packages are provided )

Navigate to the project folder:
```bash
$ cd jupiter-master/
$ export JUPITERHOME=$PWD/lib/core
```

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

## Docs and Tutorial

Coming soon...

## Differences with other Smalltalk systems

### Syntax
The Jupiter syntax is mostly the same as other smalltalk implementations, with a few minor differences:

* No need to declare local variables
* No Static Array syntax
* Elements in dynamic Arrays are separated by comas
* No return symbol. Methods return the last evaluated expression.

### Objects

The object model is similar to [Self](http://www.selflanguage.org/). Jupiter has no classes, all objects
are created copying other objects.

An Object in Jupiter is just an immutable [persistent](https://en.wikipedia.org/wiki/Persistent_data_structure) key-value collection of objects.
The messages sent to objects don't modify the state of the object, instead return a new object with the updated state, therefore creating instances
of objects ( cloing or copying prototypes ) is trivial.

## Internals

Jupiter VM is a simple bytecode interperter with a Mark&Sweep Garbage Collector.
There is much room for improvement in terms of performance and features. Contributions are very much welcome :)

## Status

This project is yet in alpha state, so it may crash/leak memory/explode from time to time.

## License

This project is licensed under the MPL-2.0 License. See the LICENSE file for details
