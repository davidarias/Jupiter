points
	"TODO maybe this can be done better"
    "FIXME a big list crash!"
	points := 0 to: 3000 map: [ :number | Point random ].
    normalized := points map: [ :point | point normalize ]
    
  
    