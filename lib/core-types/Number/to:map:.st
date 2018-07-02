to: stop map: aBlock
	result := {} transient.
	self to: stop do: [ :number |
        result !push: (aBlock value: number) ].
  	result persist