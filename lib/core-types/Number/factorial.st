factorial
    self <= 1 ifTrue: [ 1 ] ifFalse: [ self * ( self - 1 ) factorial ].
