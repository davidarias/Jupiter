points
    test Group name: 'Point' tests: {
        test Case description: 'Basic Point creation' assert: [

            p := Point x: 1 y: 3.

            ( (p x) == 1 ) & ( (p y) == 3 )
        ],

        test Case description: 'Point literal creation' assert: [

            p := 56 @ 234.

            ( (p x) == 56 ) & ( (p y) == 234 )
        ],

        test Case description: 'Point addition' assert: [

            p := Point x: 1 y: 3.
            p2 := Point x: 2 y: 4.
            p3 := p + p2.

            ( (p3 x) == 3 ) & ( (p3 y) == 7 )
        ],

        test Case description: 'Point toString' assert: [

            p := 42 @ 144.

            p toString == '(Point x: 42 y: 144)'
        ]

    }
