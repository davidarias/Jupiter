strings
    test Group name: 'Strings' tests: {
        test Case description: 'String concat (+)' assert: [
            'string1' + 'string2' == 'string1string2'
        ],
        test Case description: 'Format String 1' assert: [
            ( '{1}, {2}!' format: {'Hello', 'world'} ) == 'Hello, world!'
        ],

        test Case description: 'Format String 2' assert: [
            ( 'fruits: {1}, {2}, {3}' format: {'banana','orange', 'apple'} ) ==
                    'fruits: banana, orange, apple'
        ]
    }
