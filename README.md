Exam Autumn2020

Run Instructions (Visual Studio)

Open the folder
Allow CMake to build everything
Set main.cpp as the startup item
Press run

In case of the program not compiling/running, please try resaving the CMakeLists.txt file

Functionality of the program

The program reads the provided greyscale images and renders the geometry of the landscape based on this.
The player controls a first person camera that moves using WASD, with the opportunity to snap in to a perspective view with rotation
on one of the ground models by pressing F on the keyboard. There is also the possibility to zoom by scrolling on the mouse.
There are trees (instanced), deers (with collision in regards to trees), a plane and lots of textures.


Architecture / Framework

The code is structured in three different folders:


The CoreGL folder

Contains core code for handling boilerplate OpenGL code.

The main focus of this code is being reusable in many different areas.


The Engine folder

Contains a little more specific code, like cameras (first and third person) and object loaders


The Environment folder

Contains the code for rendering the landscape and models, which the environment consists of.
