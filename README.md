
UNREAL ENGINE 5 VR PROTOTYPE
===========================================================

Platform:      PCVR (Quest 3 via Link)
Engine:        Unreal Engine 5.5
Languages:     C++ and Blueprints
Duration:      Short experimental prototype


-----------------------------------------------------------
OVERVIEW
-----------------------------------------------------------
This prototype was created to get familiar with the UE workflow and to explore combining C++ gameplay logic
with Blueprint based event handling for a VR setup.
The goal was to understand how engine level gameplay code and Blueprint
scripting interact, while experimenting with gesture driven mechanics inspired by lightsaber and telekinetic abilities from my final year
project which was created in Unity. Link in Portfolio.


-----------------------------------------------------------
GAMEPLAY SUMMARY
-----------------------------------------------------------
- The player stands in a small test environment facing a
  cube that continuously fires projectiles.

- Using a lightsaber, the player can deflect incoming projectiles.

- Holding out the hand palm-forward and fingers extended freezes
  bullets mid-air.

- Releasing the gesture fires the frozen bullets back toward the cube.

All gesture inputs were manually recorded using Blueprints through a
Pose Recognizer setup (see included screenshots).


-----------------------------------------------------------
TECHNICAL DETAILS
-----------------------------------------------------------
- Core gameplay logic such as projectile spawning and heavy updates
  were handled in C++.

- Blueprint handled event responses and player interactions such as
  gesture detection, pose recording, and visual effects.

- Gesture recognition delegates were connected to gameplay actions such
  as enabling/disabling movement or triggering projectile release.

- Real-time lighting and particle systems (sparks, trails, smoke)
  provided visual feedback for collisions and deflections.

- Gesture recording was implemented using a Flip Flop system in Blueprint
  to toggle between "Recording Started" and "Recording Stopped" states.


-----------------------------------------------------------
LEARNING GOALS
-----------------------------------------------------------
- Understand Unreal’s Blueprint/C++ communication pipeline.
- Implement gesture driven input logic for VR using pose recognition.
- Experiment with physics based projectiles, particle feedback, and
  real-time lighting.
- Get Comfortable with UE terminology and Practices


-----------------------------------------------------------
INCLUDED FILES
-----------------------------------------------------------
- Some code snippets showing the bullet freeze and fire back ability setup and player locomotion
  [📂 Snippets](https://docs.google.com/document/d/1wJwJBecX-XP5wDAtEAXf6Qegq6YEMOFSg4b5GeJbJlo/edit?usp=drive_link)

- Short Demo showing gameplay from the perspective of the VR headset
  [📂 Demo Video](https://drive.google.com/file/d/1eb_31Tv_4Bji31pZP8BRC04V0UBrRsWx/view?usp=drive_link)


-----------------------------------------------------------
NOTES
-----------------------------------------------------------
This prototype is not a full game but an experimental gameplay sandbox.
It served as a learning project to better understand Unreal Engine’s
architecture, particularly how to combine C++ and Blueprint for
responsive gameplay in VR.

 
