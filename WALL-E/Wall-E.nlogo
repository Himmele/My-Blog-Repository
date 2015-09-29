extensions [array]

globals [
  GENOME-SIZE
  NUM-TURTLES
  PERCENTAGE-GENERATIVE-TURTLES
  
  ; gene positions
  GENE-OBJ-HERE-POS
  GENE-OBJ-AHEAD-POS
  GENE-OBJ-RIGHT-POS
  GENE-OBJ-BEHIND-POS
  GENE-OBJ-LEFT-POS
  GENE-ACTIVITY-POS
  GENE-SIZE
  
  ; object ids
  OBJ-ID-NOTHING
  OBJ-ID-WASTE
  OBJ-ID-WALL
  NUM-OBJ-IDS
  
  ; activities
  ACTIVITY-GO-RIGHT
  ACTIVITY-GO-LEFT
  ACTIVITY-GO-BACKWARD
  ACTIVITY-GO-FORWARD
  ACTIVITY-STAY
  ACTIVITY-PICK-UP-WASTE
  ACTIVITY-GO-RAND-DIR
  NUM-ACTIVITIES
  
  num-generations
]

turtles-own
[
  genome
  fitness
  die?
]

patches-own
[
  waste?
  wall?
]

; setup

to setup
  clear-all
  
  set GENOME-SIZE 243
  set NUM-TURTLES 1000
  set PERCENTAGE-GENERATIVE-TURTLES 0.1
  
  set GENE-OBJ-HERE-POS 0
  set GENE-OBJ-AHEAD-POS 1
  set GENE-OBJ-RIGHT-POS 2
  set GENE-OBJ-BEHIND-POS 3
  set GENE-OBJ-LEFT-POS 4
  set GENE-ACTIVITY-POS 5
  set GENE-SIZE 6
  
  set OBJ-ID-NOTHING 0
  set OBJ-ID-WASTE 1
  set OBJ-ID-WALL 2
  set NUM-OBJ-IDS 3
  
  set ACTIVITY-GO-RIGHT 0
  set ACTIVITY-GO-LEFT 1
  set ACTIVITY-GO-BACKWARD 2
  set ACTIVITY-GO-FORWARD 3
  set ACTIVITY-STAY 4
  set ACTIVITY-PICK-UP-WASTE 5
  set ACTIVITY-GO-RAND-DIR 6
  set NUM-ACTIVITIES 7
  
  set num-generations 0

  setup-turtles
  setup-patches
  ask turtles [
    init-genome
  ]
end

to setup-turtles
  create-turtles NUM-TURTLES
  ask turtles [
    if die? != true [
      set die? false
      set fitness 0
      setxy 0 0
      facexy 1 0
      set genome array:from-list n-values GENOME-SIZE [array:from-list n-values GENE-SIZE [0]]
    ]
  ]
end

to setup-patches
  clear-patches
  let waste-probability 0
  ifelse rand-waste-probability = true [
    set waste-probability ((random 5) + 1) * 10 ; 10% to 50% probability
  ] [
    set waste-probability 50
  ]
  ask patches [
    setup-waste (waste-probability)
    setup-walls
  ]
end

to setup-waste [waste-probability]
  set waste? (random 100) < waste-probability
  if waste? [ set pcolor blue ]
end

to setup-walls
  set wall? false
  if pxcor = max-pxcor or pxcor = min-pxcor [
    set wall? true
    set pcolor green
  ]
  if pycor = max-pycor or pycor = min-pycor [
    set wall? true
    set pcolor green
  ]
end

; turtles

to go
  let turtle-id-base num-generations * NUM-TURTLES
  foreach n-values NUM-TURTLES [?] [    
    let i ?
    
    foreach n-values 4 [?1] [
      setup-patches
      repeat 1000 [
        go-wall-e (turtle-id-base + i)      
      ] 
    ]
    
    if logging [
      ask turtle (turtle-id-base + i) [
        write turtle-id-base + i
        write ": "
        print fitness
      ]
    ]
    
    ask turtle (turtle-id-base + i) [
      set die? true
    ]
  ]

  let fittest-turtles sort max-n-of (NUM-TURTLES * PERCENTAGE-GENERATIVE-TURTLES) turtles [ fitness ]
  recombine fittest-turtles turtle-id-base
  mutate turtle-id-base
  
  set-current-plot "Fitness"
  let max-fitness 0
  foreach (sort max-n-of 1 turtles [ fitness ]) [
    ask ? [ set max-fitness fitness ]
  ]
  plotxy num-generations max-fitness
  
  ask turtles [
    if die? = true [ die ]
  ]
  
  set num-generations num-generations + 1
end

to go-right
  rt 90
  forward 1
end

to go-left
  rt 270
  forward 1
end

to go-backward
  rt 180
  forward 1
end

to go-forward
  forward 1
end

to go-rand-dir
  rt (random 4) * 90
  forward 1
end

to stay
end

to pick-up-waste
  if waste? = true [
    set waste? false
    set pcolor black
    set fitness fitness + 10
  ]
end

to go-wall-e [turtle-id]
  ask turtle turtle-id [
    do-activity
  ]
end

to do-activity
  let gene array:from-list n-values (GENE-SIZE - 1) [0]
  array:set gene GENE-OBJ-HERE-POS classify-object-at-angle 0 0
  array:set gene GENE-OBJ-AHEAD-POS classify-object-at-angle 0 1
  array:set gene GENE-OBJ-RIGHT-POS classify-object-at-angle 90 1 
  array:set gene GENE-OBJ-BEHIND-POS classify-object-at-angle 180 1
  array:set gene GENE-OBJ-LEFT-POS classify-object-at-angle 270 1
  
  let activity lookup-activity gene
  if activity = ACTIVITY-GO-RIGHT [
    go-right
  ]
  if activity = ACTIVITY-GO-LEFT [
    go-left
  ]
  if activity = ACTIVITY-GO-BACKWARD [
    go-backward
  ]
  if activity = ACTIVITY-GO-FORWARD [
    go-forward
  ]
  if activity = ACTIVITY-STAY [
    stay
  ]
  if activity = ACTIVITY-PICK-UP-WASTE [
    pick-up-waste
  ]
  if activity = ACTIVITY-GO-RAND-DIR [
    go-rand-dir
  ]
  if activity = -1 [
    forward 1
  ]
end

to-report classify-object-at-angle [angle dist]
  let p patch-right-and-ahead angle dist
  if p = nobody [ report 0 ]
  let object-id OBJ-ID-NOTHING
  if [waste?] of p [ set object-id OBJ-ID-WASTE ]
  if [wall?] of p [ set object-id OBJ-ID-WALL ]
  report object-id
end

to init-genome
  ; init all possible object combinations with random activities
  foreach n-values GENOME-SIZE [?] [
    ; init column 0 (GENE-OBJ-HERE-POS)
    if ? mod 243 >= 0 and ? mod 243 <= 80 [
      set-gene-value ? GENE-OBJ-HERE-POS OBJ-ID-NOTHING
    ]
    if ? mod 243 >= 81 and ? mod 243 <= 161 [
      set-gene-value ? GENE-OBJ-HERE-POS OBJ-ID-WASTE
    ]
    if ? mod 243 >= 162 and ? mod 243 <= 242 [
      set-gene-value ? GENE-OBJ-HERE-POS OBJ-ID-WALL
    ]
    
    ; init column 1 (GENE-OBJ-AHEAD-POS)
    if ? mod 81 >= 0 and ? mod 81 <= 26 [
      set-gene-value ? GENE-OBJ-AHEAD-POS OBJ-ID-NOTHING
    ]
    if ? mod 81 >= 27 and ? mod 81 <= 53 [
      set-gene-value ? GENE-OBJ-AHEAD-POS OBJ-ID-WASTE
    ]
    if ? mod 81 >= 54 and ? mod 81 <= 80 [
      set-gene-value ? GENE-OBJ-AHEAD-POS OBJ-ID-WALL
    ]
    
    ; init column 2 (GENE-OBJ-RIGHT-POS)
    if ? mod 27 >= 0 and ? mod 27 <= 8 [
      set-gene-value ? GENE-OBJ-RIGHT-POS OBJ-ID-NOTHING
    ]
    if ? mod 27 >= 9 and ? mod 9 <= 17 [
      set-gene-value ? GENE-OBJ-RIGHT-POS OBJ-ID-WASTE
    ]
    if ? mod 27 >= 18 and ? mod 27 <= 26 [
      set-gene-value ? GENE-OBJ-RIGHT-POS OBJ-ID-WALL
    ]
    
    ; init column 3 (GENE-OBJ-BEHIND-POS)
    if ? mod 9 >= 0 and ? mod 9 <= 2 [
      set-gene-value ? GENE-OBJ-BEHIND-POS OBJ-ID-NOTHING
    ]
    if ? mod 9 >= 3 and ? mod 9 <= 5 [
      set-gene-value ? GENE-OBJ-BEHIND-POS OBJ-ID-WASTE
    ]
    if ? mod 9 >= 6 and ? mod 9 <= 8 [
      set-gene-value ? GENE-OBJ-BEHIND-POS OBJ-ID-WALL
    ]
    
    ; init column 4 (GENE-OBJ-LEFT-POS)
    if ? mod 3 = 0 [
      set-gene-value ? GENE-OBJ-LEFT-POS OBJ-ID-NOTHING
    ]
    if ? mod 3 = 1 [
      set-gene-value ? GENE-OBJ-LEFT-POS OBJ-ID-WASTE
    ]
    if ? mod 3 = 2 [
      set-gene-value ? GENE-OBJ-LEFT-POS OBJ-ID-WALL
    ]
   
    set-gene-value ? GENE-ACTIVITY-POS random NUM-ACTIVITIES
  ]
 
; init all possible object combinations randomly and with random activities
;  foreach n-values GENOME-SIZE [?] [
;    set-gene-value ? 0 random NUM-OBJ-IDS
;    set-gene-value ? 1 random NUM-OBJ-IDS
;    set-gene-value ? 2 random NUM-OBJ-IDS
;    set-gene-value ? 3 random NUM-OBJ-IDS
;    set-gene-value ? 4 random NUM-OBJ-IDS
;    set-gene-value ? GENE-ACTIVITY-POS random NUM-ACTIVITIES
;  ]
end

; GA procedures

to recombine [fittest-turtles turtle-id-base]
  setup-turtles
  
  let fittest-turtles-ids array:from-list n-values (NUM-TURTLES * PERCENTAGE-GENERATIVE-TURTLES) [0]
  let i 0
  foreach fittest-turtles [
    ask ? [ array:set fittest-turtles-ids i who ]
    set i i + 1
  ]
  
  foreach n-values (2 * NUM-TURTLES) [?] [
    ask turtle (turtle-id-base + ?) [
      if die? != true [
        do-recombine fittest-turtles-ids
      ]
    ]
  ]
end

to do-recombine [fittest-turtles-ids]
  let a random (NUM-TURTLES * PERCENTAGE-GENERATIVE-TURTLES)
  let b random (NUM-TURTLES * PERCENTAGE-GENERATIVE-TURTLES)
  let pivot random GENOME-SIZE
 
  foreach n-values GENOME-SIZE [?] [
    let i ?
    foreach n-values GENE-SIZE [?1] [
      let j ?1
      let value 0
      ifelse i < pivot [
        set value get-gene-value-of-turtle (array:item fittest-turtles-ids a) i j
      ] [
        set value get-gene-value-of-turtle (array:item fittest-turtles-ids b) i j
      ]
      set-gene-value i j value
    ]
  ]
end

to mutate [ turtle-id-base ]
  foreach n-values (2 * NUM-TURTLES) [?] [
    ask turtle (turtle-id-base + ?) [
      if die? != true [
        do-mutate
      ]
    ]
  ]
end

to do-mutate
  foreach n-values GENOME-SIZE [?] [
    let value random 100
    if value >= 95 and value < 100 [
      set-gene-value ? GENE-ACTIVITY-POS random NUM-ACTIVITIES
    ]
  ]
  
;  repeat 4 [
;    let pos random GENOME-SIZE
;    set-gene-value pos GENE-ACTIVITY-POS random NUM-ACTIVITIES
;  ]
end

to set-gene-value [ #row #col #value ] 
   array:set (array:item genome #row) #col #value
end 

to-report get-gene-value [ #row #col ] 
   report array:item (array:item genome #row) #col 
end

to-report get-gene-value-of-turtle [turtle-id #row #col]
  let value 0
  ask turtle turtle-id [
    set value get-gene-value #row #col
  ]
  report value
end

to-report lookup-activity [ gene ]
  foreach n-values GENOME-SIZE [?] [
    let i ?
    let gene-at-i array:item genome i
    let break false
    foreach n-values (GENE-SIZE - 1) [?1] [
      if not break [
        let j ?1
        if (array:item gene-at-i j) != (array:item gene j) [
          set break true
        ]
      ]
    ]
    let contains-gene (not break)
    if contains-gene [
      report array:item gene-at-i GENE-ACTIVITY-POS;
    ]
  ]
  report -1
end

; debugger

to debug
  set rand-waste-probability false
  
  let turtle-id-base num-generations * NUM-TURTLES
  foreach n-values NUM-TURTLES [?] [    
    let i ?
    
    random-seed 17
    setup-patches
    
    repeat 200 [
      go-wall-e (turtle-id-base + i) 
    ]
    
    ask turtle (turtle-id-base + i) [
      write turtle-id-base + i
      write ": "
      print fitness
    ]
  ]
  
  ask turtles [
     set fitness 0
     setxy 0 0
     facexy 1 0
  ]
  
  random-seed 17
  setup-patches
end

to print-genome
  foreach n-values GENOME-SIZE [?] [
    write "Gene: "
    write ?
    write " "
    write get-gene-value ? GENE-OBJ-HERE-POS
    write get-gene-value ? GENE-OBJ-AHEAD-POS
    write get-gene-value ? GENE-OBJ-RIGHT-POS
    write get-gene-value ? GENE-OBJ-BEHIND-POS
    write get-gene-value ? GENE-OBJ-LEFT-POS
    write " : "
    print get-gene-value ? GENE-ACTIVITY-POS
  ]
end
@#$#@#$#@
GRAPHICS-WINDOW
205
10
644
470
16
16
13.0
1
10
1
1
1
0
1
1
1
-16
16
-16
16
0
0
1
ticks

CC-WINDOW
5
484
653
579
Command Center
0

BUTTON
3
10
69
43
Setup
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL

BUTTON
77
10
140
43
Go
go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL

INPUTBOX
3
133
158
193
debugger-turtle-id
0
1
0
Number

BUTTON
3
197
119
230
Init Debugger
debug\n
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL

BUTTON
127
197
199
230
Debug
go-wall-e debugger-turtle-id
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL

MONITOR
3
84
124
129
NIL
num-generations
17
1
11

SWITCH
3
234
107
267
logging
logging
1
1
-1000

PLOT
3
320
203
470
Fitness
NIL
NIL
0.0
100.0
0.0
4000.0
true
false
PENS
"default" 1.0 0 -16777216 true

SWITCH
3
47
204
80
rand-waste-probability
rand-waste-probability
0
1
-1000

@#$#@#$#@
WHAT IS IT?
-----------
This section could give a general understanding of what the model is trying to show or explain.


HOW IT WORKS
------------
This section could explain what rules the agents use to create the overall behavior of the model.


HOW TO USE IT
-------------
This section could explain how to use the model, including a description of each of the items in the interface tab.


THINGS TO NOTICE
----------------
This section could give some ideas of things for the user to notice while running the model.


THINGS TO TRY
-------------
This section could give some ideas of things for the user to try to do (move sliders, switches, etc.) with the model.


EXTENDING THE MODEL
-------------------
This section could give some ideas of things to add or change in the procedures tab to make the model more complicated, detailed, accurate, etc.


NETLOGO FEATURES
----------------
This section could point out any especially interesting or unusual features of NetLogo that the model makes use of, particularly in the Procedures tab.  It might also point out places where workarounds were needed because of missing features.


RELATED MODELS
--------------
This section could give the names of models in the NetLogo Models Library or elsewhere which are of related interest.


CREDITS AND REFERENCES
----------------------
This section could contain a reference to the model's URL on the web if it has one, as well as any other necessary credits or references.
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

airplane
true
0
Polygon -7500403 true true 150 0 135 15 120 60 120 105 15 165 15 195 120 180 135 240 105 270 120 285 150 270 180 285 210 270 165 240 180 180 285 195 285 165 180 105 180 60 165 15

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

box
false
0
Polygon -7500403 true true 150 285 285 225 285 75 150 135
Polygon -7500403 true true 150 135 15 75 150 15 285 75
Polygon -7500403 true true 15 75 15 225 150 285 150 135
Line -16777216 false 150 285 150 135
Line -16777216 false 150 135 15 75
Line -16777216 false 150 135 285 75

bug
true
0
Circle -7500403 true true 96 182 108
Circle -7500403 true true 110 127 80
Circle -7500403 true true 110 75 80
Line -7500403 true 150 100 80 30
Line -7500403 true 150 100 220 30

butterfly
true
0
Polygon -7500403 true true 150 165 209 199 225 225 225 255 195 270 165 255 150 240
Polygon -7500403 true true 150 165 89 198 75 225 75 255 105 270 135 255 150 240
Polygon -7500403 true true 139 148 100 105 55 90 25 90 10 105 10 135 25 180 40 195 85 194 139 163
Polygon -7500403 true true 162 150 200 105 245 90 275 90 290 105 290 135 275 180 260 195 215 195 162 165
Polygon -16777216 true false 150 255 135 225 120 150 135 120 150 105 165 120 180 150 165 225
Circle -16777216 true false 135 90 30
Line -16777216 false 150 105 195 60
Line -16777216 false 150 105 105 60

car
false
0
Polygon -7500403 true true 300 180 279 164 261 144 240 135 226 132 213 106 203 84 185 63 159 50 135 50 75 60 0 150 0 165 0 225 300 225 300 180
Circle -16777216 true false 180 180 90
Circle -16777216 true false 30 180 90
Polygon -16777216 true false 162 80 132 78 134 135 209 135 194 105 189 96 180 89
Circle -7500403 true true 47 195 58
Circle -7500403 true true 195 195 58

circle
false
0
Circle -7500403 true true 0 0 300

circle 2
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240

cow
false
0
Polygon -7500403 true true 200 193 197 249 179 249 177 196 166 187 140 189 93 191 78 179 72 211 49 209 48 181 37 149 25 120 25 89 45 72 103 84 179 75 198 76 252 64 272 81 293 103 285 121 255 121 242 118 224 167
Polygon -7500403 true true 73 210 86 251 62 249 48 208
Polygon -7500403 true true 25 114 16 195 9 204 23 213 25 200 39 123

cylinder
false
0
Circle -7500403 true true 0 0 300

dot
false
0
Circle -7500403 true true 90 90 120

face happy
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 255 90 239 62 213 47 191 67 179 90 203 109 218 150 225 192 218 210 203 227 181 251 194 236 217 212 240

face neutral
false
0
Circle -7500403 true true 8 7 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Rectangle -16777216 true false 60 195 240 225

face sad
false
0
Circle -7500403 true true 8 8 285
Circle -16777216 true false 60 75 60
Circle -16777216 true false 180 75 60
Polygon -16777216 true false 150 168 90 184 62 210 47 232 67 244 90 220 109 205 150 198 192 205 210 220 227 242 251 229 236 206 212 183

fish
false
0
Polygon -1 true false 44 131 21 87 15 86 0 120 15 150 0 180 13 214 20 212 45 166
Polygon -1 true false 135 195 119 235 95 218 76 210 46 204 60 165
Polygon -1 true false 75 45 83 77 71 103 86 114 166 78 135 60
Polygon -7500403 true true 30 136 151 77 226 81 280 119 292 146 292 160 287 170 270 195 195 210 151 212 30 166
Circle -16777216 true false 215 106 30

flag
false
0
Rectangle -7500403 true true 60 15 75 300
Polygon -7500403 true true 90 150 270 90 90 30
Line -7500403 true 75 135 90 135
Line -7500403 true 75 45 90 45

flower
false
0
Polygon -10899396 true false 135 120 165 165 180 210 180 240 150 300 165 300 195 240 195 195 165 135
Circle -7500403 true true 85 132 38
Circle -7500403 true true 130 147 38
Circle -7500403 true true 192 85 38
Circle -7500403 true true 85 40 38
Circle -7500403 true true 177 40 38
Circle -7500403 true true 177 132 38
Circle -7500403 true true 70 85 38
Circle -7500403 true true 130 25 38
Circle -7500403 true true 96 51 108
Circle -16777216 true false 113 68 74
Polygon -10899396 true false 189 233 219 188 249 173 279 188 234 218
Polygon -10899396 true false 180 255 150 210 105 210 75 240 135 240

house
false
0
Rectangle -7500403 true true 45 120 255 285
Rectangle -16777216 true false 120 210 180 285
Polygon -7500403 true true 15 120 150 15 285 120
Line -16777216 false 30 120 270 120

leaf
false
0
Polygon -7500403 true true 150 210 135 195 120 210 60 210 30 195 60 180 60 165 15 135 30 120 15 105 40 104 45 90 60 90 90 105 105 120 120 120 105 60 120 60 135 30 150 15 165 30 180 60 195 60 180 120 195 120 210 105 240 90 255 90 263 104 285 105 270 120 285 135 240 165 240 180 270 195 240 210 180 210 165 195
Polygon -7500403 true true 135 195 135 240 120 255 105 255 105 285 135 285 165 240 165 195

line
true
0
Line -7500403 true 150 0 150 300

line half
true
0
Line -7500403 true 150 0 150 150

pentagon
false
0
Polygon -7500403 true true 150 15 15 120 60 285 240 285 285 120

person
false
0
Circle -7500403 true true 110 5 80
Polygon -7500403 true true 105 90 120 195 90 285 105 300 135 300 150 225 165 300 195 300 210 285 180 195 195 90
Rectangle -7500403 true true 127 79 172 94
Polygon -7500403 true true 195 90 240 150 225 180 165 105
Polygon -7500403 true true 105 90 60 150 75 180 135 105

plant
false
0
Rectangle -7500403 true true 135 90 165 300
Polygon -7500403 true true 135 255 90 210 45 195 75 255 135 285
Polygon -7500403 true true 165 255 210 210 255 195 225 255 165 285
Polygon -7500403 true true 135 180 90 135 45 120 75 180 135 210
Polygon -7500403 true true 165 180 165 210 225 180 255 120 210 135
Polygon -7500403 true true 135 105 90 60 45 45 75 105 135 135
Polygon -7500403 true true 165 105 165 135 225 105 255 45 210 60
Polygon -7500403 true true 135 90 120 45 150 15 180 45 165 90

square
false
0
Rectangle -7500403 true true 30 30 270 270

square 2
false
0
Rectangle -7500403 true true 30 30 270 270
Rectangle -16777216 true false 60 60 240 240

star
false
0
Polygon -7500403 true true 151 1 185 108 298 108 207 175 242 282 151 216 59 282 94 175 3 108 116 108

target
false
0
Circle -7500403 true true 0 0 300
Circle -16777216 true false 30 30 240
Circle -7500403 true true 60 60 180
Circle -16777216 true false 90 90 120
Circle -7500403 true true 120 120 60

tree
false
0
Circle -7500403 true true 118 3 94
Rectangle -6459832 true false 120 195 180 300
Circle -7500403 true true 65 21 108
Circle -7500403 true true 116 41 127
Circle -7500403 true true 45 90 120
Circle -7500403 true true 104 74 152

triangle
false
0
Polygon -7500403 true true 150 30 15 255 285 255

triangle 2
false
0
Polygon -7500403 true true 150 30 15 255 285 255
Polygon -16777216 true false 151 99 225 223 75 224

truck
false
0
Rectangle -7500403 true true 4 45 195 187
Polygon -7500403 true true 296 193 296 150 259 134 244 104 208 104 207 194
Rectangle -1 true false 195 60 195 105
Polygon -16777216 true false 238 112 252 141 219 141 218 112
Circle -16777216 true false 234 174 42
Rectangle -7500403 true true 181 185 214 194
Circle -16777216 true false 144 174 42
Circle -16777216 true false 24 174 42
Circle -7500403 false true 24 174 42
Circle -7500403 false true 144 174 42
Circle -7500403 false true 234 174 42

turtle
true
0
Polygon -10899396 true false 215 204 240 233 246 254 228 266 215 252 193 210
Polygon -10899396 true false 195 90 225 75 245 75 260 89 269 108 261 124 240 105 225 105 210 105
Polygon -10899396 true false 105 90 75 75 55 75 40 89 31 108 39 124 60 105 75 105 90 105
Polygon -10899396 true false 132 85 134 64 107 51 108 17 150 2 192 18 192 52 169 65 172 87
Polygon -10899396 true false 85 204 60 233 54 254 72 266 85 252 107 210
Polygon -7500403 true true 119 75 179 75 209 101 224 135 220 225 175 261 128 261 81 224 74 135 88 99

wheel
false
0
Circle -7500403 true true 3 3 294
Circle -16777216 true false 30 30 240
Line -7500403 true 150 285 150 15
Line -7500403 true 15 150 285 150
Circle -7500403 true true 120 120 60
Line -7500403 true 216 40 79 269
Line -7500403 true 40 84 269 221
Line -7500403 true 40 216 269 79
Line -7500403 true 84 40 221 269

x
false
0
Polygon -7500403 true true 270 75 225 30 30 225 75 270
Polygon -7500403 true true 30 75 75 30 270 225 225 270

@#$#@#$#@
NetLogo 4.0.4
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180

@#$#@#$#@
