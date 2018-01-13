reduce: aBlock
    acc := self front.
    1 to: self lastIndex do: [ :index |
        acc := aBlock value: acc value: ( self at: index) ].
    acc
