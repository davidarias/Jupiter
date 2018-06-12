naiveFib: n
    n == 0 ifTrue: [ 0 ] ifFalse: [
        n == 1 ifTrue: [ 1 ] ifFalse: [
            (self naiveFib: (n - 1)) + (self naiveFib: (n - 2))
        ].
    ].