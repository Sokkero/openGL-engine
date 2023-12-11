# <u><b>OpenGl 3D engine</b></u>

<img src="https://github.com/Sokkero/openGL-engine/blob/master/src/resources/misc/engine.gif"  alt="Engine showcase"/>

## <u>Project Setup</u>
### Requirements
- Conan 1.X (1.60.2 recommended)
- Clang-format (latest LTS recommended)
- <a href="https://ninja-build.org/">Ninja</a> (latest LTS recommended)<br><br>
- `brew install conan@1 Clang-format Ninja && brew link conan@1`
### Setup
1) Run the buildDeps.sh script
2) Execute the following
   - `cd cmake-build-debug`
   - `cmake .. -G Ninja`

### Building the project
1) Run `cmake --build ./cmake-build-debug --target openGLEngine` from the projects root dir
2) Navigate to the folder `cmake-build-debug/bin/`
3) Execute the generated build named `openGLEngine`

### Code style
The code format rules are customized and declared in `.clang-format`.<br>
To enforce the defined code style, simply execute the script `format.sh` in the project root.

## <u>Engine features</u>
### Supported file types for models and textures
- `.obj`
- `.bmp`
- `.DDS`

### Capabilities
- Supports loading of custom shaders with custom data structures
- Supports customizable ambient & diffuse lighting
  - Can be bound to any shader
- Objects in the scene follow a scene graph hierarchy
- All engine settings and variables can be safely accessed from its entry point `Engine::GameInterface`
- Objects can be freely be moved, scaled and rotated at runtime
- Scene Objects (`VirtualObjects`) can be created by picking components from `src/engine/classes/NodeComponents`