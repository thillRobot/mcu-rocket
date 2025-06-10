# introduction to mcu-rocket

Project introduction and basics of rocketry dicussion

## Abstract (GSET 2025)
Space travel is an exciting and important cross discipline engineering application essential for communication, defense, and exploration. Rockets are primarily used for deploying vehicles from earth, and the prediction and measurement of a rocket apogee, or maximum distance from earth, is critical for mission success. Participants will engage in an exciting engineering project including the design, fabrication, and launch of a custom model rocket with an on-board sensor suite and data acquisition payload for monitoring flight performance. The primary goals of the project are designing a rocket capable of successful launch with recovery and predicting and measuring the rocket apogee during flight. Engineering activities include 3D computer aided design (CAD), additive manufacturing and 3D printing, analytical and numerical analysis, computer programming with embedded systems, and empirical testing. Students will be exposed to and learn the basics of Solidworks for CAD, FDM 3D printing, Arduino MCUs and C++ programming, and rocket modeling and analysis.


## Introduction to Rocketry 

[parts of a Rocket](https://www1.grc.nasa.gov/beginners-guide-to-aeronautics/model-rockets/#parts-of-a-single-stage-model-rocket)
  
### Forces on a Rocket

  - Thrust

  - Weight - force from gravity, applied at center of gravity (CG) towards earth

Force from gravity:

$$F_w=mass \times gravity = mg$$

  - Drag - force from air in rocket direction    

  - Lift - force from air orthogonal to rocket direction, similar to airplane lift 

- Rocket Trust

Rocket Thrust Equation:

$$F=\dot{m}V_e+(p_e-p_o)A_e$$

- Rocket Weight


### Rocket Nozzle

What is the purpose of the nozzle on the model rocket? How does the nozzle work?

- the nozzle increases the exit velocity of the gas which is directly proportional to the thrust

Consider flow through a converging nozzle (a pipe with reducing cross sectional area). Is the output flow expected the be faster or slower than the input flow? How can we find the answser?  

When a fluid flows steadily through a conduit that has a flow cross-sectional area that varies with the axial distance, the conservation of mass (continuity) equation

$$\dot{m}=\rho AV=constant$$



[Guide to Rockets - Glenn Research Center](https://www1.grc.nasa.gov/beginners-guide-to-aeronautics/guide-to-rockets/)


## Introduction to 3D CAD 

- Discuss Backgrounds

- Introduce Solidworks

- Topics:
  
  - [] 2D Sketching
    - [x] basic drawing
    - [x] sketch constraints  
      - [x] coincident 
      - [] orthonogal
      - [] parallel 
      - [] colinear
  - [] 3D Features  
    - [x] Extrude 
    - [] Revolve 
    - [] Extrude Cut
    - [] Hole
  - [x] Saving a Part
    - [x] save as Solidworks part file .sldprt (DSS proprietary)
    - [x] save as stereolithography file .stl (standard - [wikipedia](https://en.wikipedia.org/wiki/STL_(file_format)))
