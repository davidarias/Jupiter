name: groupName tests: tests

    ( '- Test group: {1}' format: { groupName } ) print.
    io stdout endl.
	
    tests do: [ :case | case toString print ].
    
    errors := tests reduce: [ :errors :case |
        [ errors + 1 ] if: case fail else: errors
    ] initial: 0.

    total := tests size.
    passed := total - errors.

    io stdout endl.

    ( '  {1}/{2} Test passed' format: { passed, total } ) print.

    io stdout endl.
    errors
