reduce: aBlock initial: acc
    self do: [ :n | acc := aBlock value: acc value: n ].
    acc
