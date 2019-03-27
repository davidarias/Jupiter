toString
	self succeed ifTrue: [
    	'  ✔ {1}' format: { self description }
    ] ifFalse: [
    	'  ✘ {1}' format: { self description }
    ]