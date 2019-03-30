run

    tests := {
        self numbers run,
        self strings run,
        self controlStructures run,
        self closureBlocks run,
        self arrays run,
        self objects run,
        self points run
    }.

    totalErrors := tests reduce: [ :acc :n | acc + n ].
    ('Total tests failing: {1}' format: { totalErrors }) print.
