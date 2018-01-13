objects
    text := 'text'.
    object := Map from: {
        'number' -> ( 2 + 3 ),
        'aText' -> text,
        'otherText' -> 'other text'
    }.

    test Group name: 'Objects' tests: {
        test Case description: 'Basic literal object' assert: [

            ( (object number) == 5 ) &
            ( (object aText) == 'text' ) &
            ( (object otherText) == 'other text' )
        ],

        test Case description: 'Object at method' assert: [

            ( (object at: 'number') == 5 ) &
            ( (object at: 'aText') == 'text' ) &
            ( (object at: 'otherText') == 'other text' )
        ],

        test Case description: 'Object assign immutable method' assert: [

            o2 := object assign: { 'number' -> 42 }.

            ( (object at: 'number') == 5 ) &
            ( (object at: 'aText') == 'text' ) &
            ( (object at: 'otherText') == 'other text' ) &
            ( (o2 at: 'number') == 42 ) &
            ( (o2 at: 'aText') == 'text' ) &
            ( (o2 at: 'otherText') == 'other text' )
        ],

        test Case description: 'Objects equality and identity' assert: [

            o2 := Map from: {
                'number' -> 5,
                'aText' -> text,
                'otherText' -> 'other text'
            }.

            ( object == o2 ) & ( ( object isIdenticalTo: o2 ) == false)
        ]

    }
