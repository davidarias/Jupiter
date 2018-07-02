name: groupName tests: testArray

    ( '- Test group: {1}' format: { groupName } ) print.
    io stdout endl.

    errors := testArray reduce: [ :errors :case |
        description := case at: 'description'.
        block := case at: 'assert'.
        block value
            ifTrue: [
                io stdout write: '  ✔ ';
                        write: description;
                        endl.
                        errors.
            ] ifFalse: [
                io stdout write: '  ✘ ';
                        write: description;
                        endl.
                        errors + 1
            ]
    ] initial: 0.

    total := testArray size.
    passed := total - errors.

    io stdout endl.

    ( '  {1}/{2} Test passed' format: { passed, total } ) print.

    io stdout endl.
    errors
