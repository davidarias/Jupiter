run

    tests := {
        self numbers,
        self strings,
        self closureBlocks,
        self arrays,
        self objects,
        self points
    }.

    totalErrors := tests reduce: [ :acc :n | acc + n ] initial: 0.
    ('Total tests failing: {0}' format: { totalErrors }) print.
