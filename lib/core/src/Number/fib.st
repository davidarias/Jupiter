fib
    self == 0 ifTrue: [ 0 ] ifFalse: [
        self == 1 ifTrue: [ 1 ] ifFalse: [
            (self - 1) fib + (self - 2) fib 
        ].
    ].
