map: aBlock
    "TODO use transient array when implemented"
    returnArray := {} transient.
    1 to: self size do: [ :index |
        returnArray push: ( aBlock value: (self at: index) )].
    returnArray persist
