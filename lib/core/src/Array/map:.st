map: aBlock
    "TODO use transient array when implemented"
    returnArray := {} transient.
    0 to: self lastIndex do: [ :index |
        returnArray push: ( aBlock value: (self at: index) )].
    returnArray persist
