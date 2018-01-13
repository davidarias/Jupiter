numbers
    test Group name: 'Numbers' tests: {
        test Case description: 'Integer addition 1' assert: [
            23 + 42 == 65
        ],

        test Case description: 'Integer addition 2' assert: [
            42 + (-11) == 31
        ],

        test Case description: 'Decimal addition 1' assert: [
            0.1 + 0.2 == 0.3
        ],

        test Case description: 'Decimal multiplication 1' assert: [
            12.42 * 0.2345 == 2.91249
        ],

        test Case description: 'Comparisions 1' assert: [
            0.0001 > 0.00001
        ],

        test Case description: 'Comparisions 2' assert: [
            10 < -2 == false
        ]
    }
