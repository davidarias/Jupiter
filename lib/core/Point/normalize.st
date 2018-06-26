normalize
	x := self x.
    y := self y.
    z := self z.
	norm := ( (x * x) + (y * y) + (z * z) ) sqrt.
    Point x: x / norm y: y / norm z: z / norm