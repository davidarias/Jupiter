assign: pairsArray
    map := self transient.
    pairsArray do: [ :pair |
        map at: pair key put: pair value
    ].
    map persist
