# POMP
PHONI on Mobile Platform

The MinION is a piece of genomic sequencing hardware, introduced in 2014, as the first commercial sequencer using nanopore technology. The task of genomic sequencing is highly computationally expensive, and was prohibitive in seeing such functionality on mobile devices in 2014. 
However, since the MinION’s release, significant hardware advances in the processing power and onboard RAM of mobile devices has made porting this functionality feasible. Our goal is to do precisely that.
Under the direction of Christina Boucher, we aim to port this onto the Android OS. The current code base can be found at https://github.com/koeppl/phoni. Our objectives currently include: 
1.Port the sdsl data structures library. This will serve as a basis for the PHONI framework.
2.Port the actualy PHONI framework itself, the core functionality of this project.
3.Create a frontend for users to utilize PHONI on their devices.
