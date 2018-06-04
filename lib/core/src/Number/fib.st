fib
	"naive recursive fib implementation with terrible performance:"
    "self == 0 ifTrue: [ 0 ] ifFalse: [
        self == 1 ifTrue: [ 1 ] ifFalse: [
            (self - 1) fib + (self - 2) fib 
        ].
    ]."
    
    "tail recursive version:"
    self fibWithFirst: 0 andSecond: 1
