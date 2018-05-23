reduce: aBlock initial: acc

	self empty ifTrue: [ acc ] else: [
        self tail reduce: aBlock initial: (aBlock value: acc value: self head) ]
