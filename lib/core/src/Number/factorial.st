factorial
    "TODO <= 1 when implemented and - instead of + (-1)"
    self == 1 ifTrue: [ 1 ] ifFalse: [ self * ( self + (-1) ) factorial ].
