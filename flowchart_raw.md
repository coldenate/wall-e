# Arduino Script Logic Flowchart



```mermaid
flowchart TD
  subgraph idinit["Chip Init"]
  A[ATmegaChip Init] --> B["void setup()"] 
  B(["void setup()"]) ---  D[Debug_Mode] --> E["Check for button held (3 Seconds window)"]
  id1([Debug mode gives verbose serial output that prints out the on-board information technology readings.])
  E --> C["Begin Temp+Humidity Data Collection"]
  B --> C
  C --> G["Begin Ultrasonic Sensor pinging"]
  end
  subgraph idloopmaster["The Whole Loop Function"]
  
  G --> idloop

  idloop(["void loop()"])
  idloop --> id2
  
  
  subgraph id2 [on-board Sensory Data Computation]
  idping[Read the current Ultrasonic Sound Reading] --> idequ[calculations]
  iddht[Temperature and Humidity Data read] --> idequ
  idequ-->iddist[Distance Calculation]
  end

  id2 --> idthresh
  iddist --> idthresh
  subgraph idthresh["Dynamic Threshold Generation"]
  idblockhalt["Halt all movement for 5 seconds"] --> idreadsensors
  idreadsensors["Read the ultrasonic sensors assuming placed in a wall."] --> idanalyze["Analyze sensors EAST + WEST and create buffer range for them."]
  idsensor(["Northern Sensor Omitted"])
  idanalyze --> iddocument(["Input the calculations into variable that will never be changed until reset"])
  end
  idthresh --> id3
  iddocument --> id3
  subgraph id3["Decision tree declarations"]
  idN["Decide if North"]
  idN --> l["Is Northern wall closer than x cm?"]
  iddist --> l
  l -- Yes --> idDec["Declare Northern Side blocked to drive"]
  l -- No --> idDecno["Declare Northern Side safe to Drive"]
  idE["Decide if East"]
  idE --> k["Is Eastern Wall closer than x cm?"]
  iddist --> k
  k -- Yes --> idDecE["Declare Eastern blocked to drive"]
  k -- No --> idDecEno["Declare Eastern safe to drive"]
  idW["Decide if West"]
  idW --> I["Is Western Wall closer than x cm?"]
  iddist --> I
  I -- Yes --> idDecW["Declare Western blocked to drive"]
  I -- No --> idDecWno["Declare Western Safe to drive"]
  end
  id3 --> id4
  subgraph id4["Driving Execution"]
  idid["If it is safe to drive north"] -- yes --> p["Drive north until it is not"]
  idid -- no --> r["begin direction tree"]
  r --> q["Which direction to turn based on only one safe (if more than one, we should prioritize an option, or choose the longer hallway"]
  q -- left --> idblock["cache distance down the hallway that was deemed safe."]
  idblock --> id9["Turn left until North side matches a bubble of that cache"]
  id9 --> idblock
  q -- right --> idblockr["cache distance down the hallway that was deemed safe."]
  idblockr --> id8["Turn right until North side matches a bubble of that cache"]
  id8 --> idblockr
  end
  id4 --> idloop
  end
```
