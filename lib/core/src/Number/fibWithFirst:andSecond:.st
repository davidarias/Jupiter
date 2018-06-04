fibWithFirst: a andSecond: b
	"tail recursive version of fib"
	self > 0 ifFalse: [ a ] else: [
  		self - 1 fibWithFirst: b andSecond: a + b
	]