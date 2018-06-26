to: stop do: aBlock
	self <= stop ifTrue: [ 
    	aBlock value: self. 
        self + 1 to: stop do: aBlock ]
