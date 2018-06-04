run

    tests := {
        self numbers,
        self strings,
        self controlStructures,
        self closureBlocks,
        self arrays,
        self objects,
        self points
    }.

    totalErrors := tests reduce: [ :acc :n | acc + n ].
    ('Total tests failing: {0}' format: { totalErrors }) print.
