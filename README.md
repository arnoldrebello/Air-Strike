# Air-Strike
Modern OpenGL project in C++

The Project runs at 4K and 120Hz making complete use of GPU processing and Memory

Step 1: Installation and setup of IDE
- Install Visual Studio 2019 with all essential components 
- open VS installer -> Modify and check desktop development with C++ and game development with C++
- enable Windows 10 SDK and C++ profiling tools in the optional section

Step 2: Installation of Libraries
- Move the CGV Project folder to Documents->Visual Studio 2019->Projects 
- Install GLEW, GLFW, GLM, ASSIMP libraries in Documents->Visual Studio 2019->External Libs
- Open project properties, under C/C++->Additional include directories->add the include file paths
- under Linker->Additional Library Directories-> add the lib file paths
- under Linker->Input->Additional Dependencies type the following
opengl32.lib
glew32.lib
glfw3.lib
assimp.lib

Step 3: Running the project
- open the file CGV Project.sln 
- Build -> Clean Solution
- Build -> Build Solution
- Debug -> Start debugging(F5)
