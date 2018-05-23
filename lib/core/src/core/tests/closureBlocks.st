closureBlocks
    upvalue := 'original value'.

    test Group name: 'Closure Blocks' tests: {

        test Case description: 'Closure upvalues retrieval 1' assert: [
            upvalue == 'original value'.
        ],

        test Case description: 'Closure upvalues modify 1' assert: [
            block := [ upvalue := 42 ]. block value.
            upvalue == 'original value'
        ],

        test Case description: 'Closure upvalues retrieval in nested closures' assert: [
            block := [
                otherUpvalue := 1010.

                blockNested := [
                    otherUpvalue + 1
                ].

                blockNested value
            ].
            block value == 1011
        ],

        test Case description: 'Closure upvalues modify in nested closures' assert: [
            block := [
                otherUpvalue := 'Nope'.

                blockNested := [
                    blockNested2 := [
                        otherUpvalue := 26
                    ].
                    blockNested2 value
                ].
                blockNested value.
                otherUpvalue
            ].
            block value == 'Nope'
        ],

        test Case description: 'Closure upvalues siblings blocks retrieval' assert: [
            block := [
                otherUpvalue := 'Nope'.

                blockNestedSibling := [

                    otherUpvalue := 0
                ].

                blockNestedSibling2 := [
                    otherUpvalue := otherUpvalue + 'Nope'.
                    otherUpvalue
                ].
                blockNestedSibling value.
                blockNestedSibling2 value
            ].
            block value == 'NopeNope'
        ],

        test Case description: 'Closure closed upvalues' assert: [
            add4Numbers := self helpers nestedAdder: 1.
            add4Numbers2 := add4Numbers value: 2.
            add4Numbers3 := add4Numbers2 value: 3.
            result := add4Numbers3 value: 4.
            result == 10
        ]

    }
