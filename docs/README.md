# CronosEngine
<p align="center">
  <b>Cronos Engine</b> is a 3D Game Engine based on OpenGL and made by 2 students <a href="https://github.com/lucho1">Lucho Suaya</a> and <a href="https://github.com/rleonborras">Roger Leon</a> of the Degree in Videogames Design and Development of Universitat Politecnica de Catalunya for the Videogames Engines subject of 3rd course.
</p>

![](res/Cronos_Engine_Logo.png?style=centerme)

**[TODO]** Add trailer??

We used [SDL](https://www.libsdl.org/), [SDL_Mixer](https://www.libsdl.org/projects/SDL_mixer/), STL, [OpenGL4](https://www.opengl.org/) with [GLAD](https://glad.dav1d.de/), [ImGui](https://github.com/ocornut/imgui), [Premake](https://premake.github.io/), [GLM](https://glm.g-truc.net/0.9.9/index.html) and [MathGeoLib](https://github.com/juj/MathGeoLib) (for some purposes), [GPUDetect](https://github.com/GameTechDev/gpudetect), [MMGR](https://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml), [ParShapes](https://prideout.net/shapes), [Assimp](http://www.assimp.org/), [Devil](http://openil.sourceforge.net/) and [JSON for Modern C++](https://nlohmann.github.io/json/).

## Screenshots & Video
**[TODO]** Add 1min video
**[TODO]** Add screenshots

## Innovating Features & Functionalities
* Shader System
* Lighting algorithm improved, better than Phong (Blinn-Phong) - Test the difference with Spotlights!
* Material System with shader rendering
* ZBuffer Real-Time Renderer (Check the Shaded dropdown in the editor, above scene window)

* Timing: Play/Pause/Stop & x2/x0.5 speed!
* Engine's camera speed control with mouse middle button (Unity-like!)

* GameObjects:
   * Copy/Paste with CTRL+C/CTRL+V & Deletion
   * Hierarchy system
   * Tranformations & AA/OO BBs
   
* Mesh & Textures importing
* Resource Manager: All data saved/loaded in binary own format and loaded into a Resource Manager to optimize resources loading and avoiding files duplication. All assets in the filesystem are loaded and saved to own file format from the beginning, and, if loaded, loaded from the own file format.
* Extensive Filesystem with good UI/UX Functionalities (asset finder, good navigation system, drag & drop...)
* Scene Serialization

* Many different types of primitives creation (check it out!)
* OpenGL Automatic Error Handler and logger (check out Errors.h!) and with possibility of calling GLCall() for every OGL Function to get possible errors with OGL functions/methods (through glGetError()) with extensive information
* Extensive System Info Read (on GPU, CPU, Memory and Software!)
* Cronos Engine will detect if you have a dedicated NVIDIA or AMD Graphics Card and will use it automatically!
* Usage of Precompiled Header to imporve build performance
* Engine's Project built with a Premake file and Project.bat generator to automatize build process

## Cronos Sub-Systems
**[TODO]**

### Shaders Sub-System
**[TODO]** Add 2 gifs

## Credits
Cronos Engine is developed by **LuchoSuaya** ([@lucho1](https://github.com/lucho1)) and **Roger Leon** ([@rleonborras](https://github.com/rleonborras)) for Videogames Engines subject of UPC's Degree in Videogames Design and Development, taught by **Marc Garrigo Garcia** ([@markitus18](https://github.com/markitus18)).

![](res/fotiyo.jpg?style=centerme)

Take a look to our Github Repository: https://www.github.com/lucho1/CronosEngine

### Individual Work
* Lucho Suaya: In charge of Engine's main systems (core functionality, game objects & components, camera, octrees optimization...) & Rendering workflow (lighting and material system, shaders sub-system...).

* Roger Leon: In charge of the whole UI, entire engine's filesystem and resource manager & serialization and engine's aspect as well as handeling several game objects' functionality as well as hierarchies (parenting), UX functionality like copy/paste, delete...

Both have been involved in the system to load geometry and textures in the engine and primitives' creation.

## License
**Copyright (c) 2019 Lucho Suaya Leiro and Roger Leon Borras**
Licensed under the **[Apache License](https://github.com/lucho1/CronosEngine/blob/master/docs/LICENSE)**.

Cronos Engine is Licensed under Apache License (http://www.apache.org/licenses/), you'll find the details in the License file in Github repository or in Engine's folder.