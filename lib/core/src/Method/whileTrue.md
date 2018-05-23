# whileTrue loop

Since the version of jupiter 0.2.0, the variable bindings are immutable,
limiting the usefulness of while loops ( we cannot use a variable counter
for example )

In any case, this can be useful for infinite loops, and side effect processing
like reading data from external sources.

## Examples
```smalltalk
[true] whileTrue: [ os cout writeln: 'initniteeee!' ].

"resource hasData is a hypothetical method that checks an external data source
that can be mutated over time, like network requests, files etc... "
[ resource hasData ] whileTrue: [ os cout writeln: resource data  ]

```

