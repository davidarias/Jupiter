# test Group

A convenient way of create a group of tests.

## Example

```smalltalk
test Group name: 'test group' tests: {
	test Case description: 'test 1' assert: [ true ],
    test Case description: 'test 2' assert: [ false ],
}
```
