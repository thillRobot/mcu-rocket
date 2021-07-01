# This page describes the design and construction of the launch vehicle for Mission A: Nano-Rocket

Note: This page is specific to `Mission A: Nano-Rocket`. For information about a different vehicle or mission, select the appropriate branch.

## Overview 

The primary goal is to design and construct a safe and low-cost model rocket capable of carrying a flight computer and sensor package as the payload in the nosecone. The rocket design shoud be suitable for construction with a typical FDM 3D printer and available building materials. 

## Design Requirements

## Design History of Nano-Rocket

### Nosecone Design

I made these designs with Autodesk Fusion 360 using the hobbyist license. This is not my favorite CAD program, but it is nice for small projects. The part construction seems straightforward, but assembly seems to be missing traditional mates such as in Inventor or Solidworks. The developers have claimed that they buiult it around joints instead of mates, but I have not quite figured out how they work yet. 

Nosecone V1 
  - first draft in CAD
  - first printed protoype showed that the split cone design was problematic

<img src="png-images/science_nosecone3.png" width=300 align=center> <img src="png-images/science_nosecone4.png" width=300 align=center> 
<img src="png-images/nosecone_construction1.png" width=300 align=center> <img src="png-images/nosecone_construction2.png" width=300 align=center>

Nosecone V2 
  - one piece nosecone with split coupler cylinder for access
  - cylinder/coupler diameter reduced for streamline  
  - closing two piece clyinder complicates problem as in V1
  - discs or rings to hold the electronics... almost worked

<img src="png-images/nosecone_assembly2.png" width=300 align=center> <img src="png-images/nosecone_assembly1.png" width=300 align=center>
<img src="png-images/nosecone_construction3.png" width=300 align=center> <img src="png-images/nosecone_construction4.png" width=300 align=center>

Nosecone V3 
  - one piece nosecone with one piece coupler cylinder for simplicity
  - pcb board added to replace MCU discs and reduce wiring
  - floor of clyinder shaped to hold pcb carrier and batteries
  - cleiling of cyinder shaped with holes to support SD card board, hold sensor boards, and allow wiring access
  - parabolic nose profile added for reduced drag in sub-sonic flight

<img src="png-images/nosecone_assembly5.png" width=300 align=center> <img src="png-images/nosecone_assembly4.png" width=300 align=center>
<img src="png-images/nosecone_construction5.png" width=300 align=center> <img src="png-images/nosecone_construction6.png" width=300 align=center> <img src="png-images/nosecone_construction7.PNG" width=300 align=center>

### Tail Design

Tail V1 

  - the purpose of the tail piece is to hold the motor and the fins securely to the body tube
  - the 'boat tail' is intended to reduce drag, the angle is set at 10 degrees
  - the first print was very heavy, which can lead to a slow and unstable rocket 
  - extrusion in motor tube for motor clip like typical estes rocket 

<img src="png-images/tail_assembly2.png" width=300 align=center> <img src="png-images/tail_assembly1.png" width=300 align=center>


Tail V2 

  - all wall thickness reduced for weight
  - body tube coupler length reduced for weight
  - boat tail length reduced for weight
  - fin reciever (notch) depth increased
  - motor clip extrusion removed, friction mount has worked for two launches

<img src="png-images/tail_assembly4.png" width=300 align=center> <img src="png-images/tail_assembly3.png" width=300 align=center>

### Fin Design

Fin V1
  - rectangular fin for subsonic flight
  - airfoil cross section for reduced profile drag and increased lift (induced drag), not sure about this part -> need to do my homework
  - rectangle base for attachment to tail piece
  
  <img src="png-images/fin1.png" width=300 align=center> <img src="png-images/fin2.png" width=300 align=center>
