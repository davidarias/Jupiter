points
	points := 0 to: 10000 map: [ :number | Point random ].
    normalized := points map: [ :point | point normalize ]
    
  
    