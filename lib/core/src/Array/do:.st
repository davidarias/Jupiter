do: aBlock
    0 to: self lastIndex do: [ :index | aBlock value: ( self at: index) ]
