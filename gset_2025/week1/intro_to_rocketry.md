# introduction to mcu-rocket

Project introduction and basics of rocketry dicussion

## Abstract (GSET 2025)
Space travel is an exciting and important cross discipline engineering application essential for communication, defense, and exploration. Rockets are primarily used for deploying vehicles from earth, and the prediction and measurement of a rocket apogee, or maximum distance from earth, is critical for mission success. Participants will engage in an exciting engineering project including the design, fabrication, and launch of a custom model rocket with an on-board sensor suite and data acquisition payload for monitoring flight performance. The primary goals of the project are designing a rocket capable of successful launch with recovery and predicting and measuring the rocket apogee during flight. Engineering activities include 3D computer aided design (CAD), additive manufacturing and 3D printing, analytical and numerical analysis, computer programming with embedded systems, and empirical testing. Students will be exposed to and learn the basics of Solidworks for CAD, FDM 3D printing, Arduino MCUs and C++ programming, and rocket modeling and analysis.


## Introduction to Rocketry 

[parts of a Rocket](https://www1.grc.nasa.gov/beginners-guide-to-aeronautics/model-rockets/#parts-of-a-single-stage-model-rocket)
  
[Guide to Rockets - Glenn Research Center](https://www1.grc.nasa.gov/beginners-guide-to-aeronautics/guide-to-rockets/)

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

When a fluid flows steadily through a conduit that has a flow cross-sectional area that varies with the axial distance, the conservation of mass (continuity) equation can be used to relate flow rates at different sections.

$$\dot{m}=\rho AV=constant$$

... Newton's 2nd law was applied to the inviscid (frictionless) and steady flow of a fluid particle. For the streamwise direction, the result for either compressible or incompressible flows is

$$dp +\frac{1}{2}\rho d\left(V^2\right) +\gamma dz=0$$ 

... For ideal gas flow, the potential engergy difference term, $\gamma dz$, can be dropped ... an appropriate equation of motion in the streamwize direction for the steady, one-dimensional, and isentropic (adiabatic and frictionless) flow of and ideal gas is obtained ...

$$\frac{dp}{\rho V^2} = -\frac{dV}{V}$$

If we form the logarith of both sides of the continuity equation, the result is

$$ln \rho +ln A +lnV = constant$$

differentiate (take the derivative)

$$\frac{d\rho}{\rho} +\frac{dA}{A} +\frac{dV}{V}= 0$$

rearange

$$-\frac{dV}{V}=\frac{d\rho}{\rho}+\frac{dA}{A}$$

now combine the equations

$$\frac{dp}{\rho V^2}\left(1-\frac{V^2}{dp/d\rho} \right)=\frac{dA}{A}$$


