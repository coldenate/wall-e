## Pseudo Code Logic

```mermaid
flowchart TB
  A[ATmegaChip Init] --> B["void setup()"] 
  B(["void setup()"]) ---  D[Debug_Mode] --> E["Check for button held (3 Seconds window)"]
  id1([Debug mode gives verbose serial output that prints out the on-board information technology readings.])
  E --> C["Begin Temp+Humidity Data Collection"]
  B --> C
  C --> G["Begin Ultrasonic Sensor pinging"]
  G --> idloop
  idloop(["void loop()"])
  idloop --> id2
  subgraph id2 [on-board Sensory Data Computation]
  idping[Read the current Ultrasonic Sound Reading] --> idequ[calculations]
  iddht[Temperature and Humidity Data read] --> idequ
  idequ-->iddist[Distance Calculation]
  end
  idloop --> id3
  id2 --> id3
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
  
  end

```
# Wall•E Robot!!
This repository holds the code for a 90° maze-solving robot car. Named after Wall•E because ultrasonic sensors look like the eyes of Wall•E!

# README TODO

- [x] Rudimentary Explanation
- [x] Layout
- [ ] Add diagram for logic
- [ ] Add diagram for wiring
- [ ] Grammar check
- [ ] Make repo public

## Purpose

The Robot Script is designed to have a constant flow of four-way proximity between the robot and walls surrounding it.
Using Ultrasonic sound transmitters, we can gather this information every (approx. :/) 2 seconds. Using the temperature sensor, we can calculate proximity using the Speed of Sound.



```cpp
Setup - Set correct pins
Setup - Get Temperature Once. Only once per initialization.
Setup - calibrate motors after a few seconds.
Setup - Define functions that allow for clean turning direction.

Loop - Begin pings
Loop - Make true/false safety statements flow
  in the loop - constantly getting speed of sound
  Gets distance of all four directions (Maybe a key:pair)
  If statement governs safety with proximity threshold 
  [Direction of which the robot came from should be ignored. Or not considered usable unless I add enough complexity to allow for more complex mazes.
  Return safety to go forward, or return a command to turn left or right.
Loop - answer t/f with either driving forward or not | or run the turn funcs
Loop - prioritize driving forward (north in terms of the robot's chassis)

```

## Why?

For a [Destination Imagination Challenge](https://www.destinationimagination.org) :). It also incorporates sound science and mathematics for the execution of movement functions.

### Faq

Are you aware your code is rudimentary?

> Yes, I barely have an idea of how to use a keyboard.
> Jokes aside, I am still a beginner in C++, and I am scared of classes.

What is the total cost if I bought everything as new?

> I am still working that one out. Hold on.

What chip is the hardest to come by?

> I would watch out for Motor Drivers. They are sneaky fellows. 

How do I wire the robot up?

> I forgot to post that. I will do that soon.

# Legal Jargon
`Copyright (c) 2022 Nate Solis`
