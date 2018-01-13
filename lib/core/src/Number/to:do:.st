to: stop do: aBlock
    nextValue := self.
    [nextValue <= stop]
        whileTrue:
            [aBlock value: nextValue.
            nextValue := nextValue + 1]
