name: groupName tests: testArray

    ( '- Test group: {0}' format: { groupName } ) print.
    os cout endl.

    errors := testArray reduce: [ :errors :case |
        description := case at: 'description'.
        block := case at: 'assert'.
        block value
            ifTrue: [
                os cout write: '  ✔ ';
                        write: description;
                        endl.
                        errors.
            ] ifFalse: [
                os cout write: '  ✘ ';
                        write: description;
                        endl.
                        errors + 1
            ]
    ] initial: 0.

    total := testArray size.
    passed := total - errors.

    os cout endl.

    ( '  {0}/{1} Test passed' format: { passed, total } ) print.

    os cout endl.
    errors
