arrays
    test Group name: 'Arrays' tests: {
        test Case description: 'Basic literal Array 1' assert: [

            text := 'text'.
            array := { 1 + 3, text, 'other text' }.

            ( (array at: 0) == 4 ) &
            ( (array at: 1) == 'text' ) &
            ( (array at: 2) == 'other text' )
        ],

        test Case description: 'Array map' assert: [

            numbers := { 1, 2, 3 }.

            doubleNumbers := numbers map: [ :number | number * 2 ].

            doubleNumbers == { 2, 4, 6 }

        ]

    }
