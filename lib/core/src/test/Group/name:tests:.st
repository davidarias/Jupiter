name: groupName tests: testArray

    ( '- Test group: {0}' format: { groupName } ) print.
    os cout endl.

    errors := 0.
    testArray do: [ :case |
        description := case at: 'description'.
        block := case at: 'assert'.
        block value
            ifTrue: [
                os cout write: '  ✔ ';
                        write: description;
                        endl
            ] ifFalse: [
                os cout write: '  ✘ ';
                        write: description;
                        endl.
                        errors := errors + 1
            ]
    ].

    total := testArray size.
    passed := total - errors.

    os cout endl.

    ( '  {0}/{1} Test passed' format: { passed, total } ) print.

    os cout endl.
    errors
