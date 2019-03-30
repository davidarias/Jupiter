run
    ( '- Test group: {1}' format: { self name } ) print.
    io stdout endl.
	
    self tests do: [ :case | case toString print ].
    
    errors := self tests reduce: [ :errors :case |
        [ errors + 1 ] if: case fail else: errors
    ] initial: 0.

    total := self tests size.
    passed := total - errors.

    io stdout endl.

    ( '  {1}/{2} Test passed' format: { passed, total } ) print.

    io stdout endl.
    errors