formatStrings
    test Group name: 'Format Strings' tests: {
        test Case description: 'Format String 1' assert: [
            ( '{0}, {1}!' format: {'Hello', 'world'} ) == 'Hello, world!'
        ],

        test Case description: 'Format String 2' assert: [
            ( 'fruits: {0}, {1}, {2}' format: {'banana','orange', 'apple'} ) ==
                    'fruits: banana, orange, apple'
        ]

    }
