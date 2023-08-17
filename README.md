# <u><b>OpenGl 3D engine</b></u>

## <u>Project Setup</u>
### Requirements
- Conan 1.X (1.60.2 recommended)
    - `brew install conan@1 && brew link conan@1`
- Clang-format (latest LTS recommended)
- <a href="https://ninja-build.org/">Ninja</a> (latest LTS recommended)
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
- Supports two shaders
  - Render vertices in custom colors
  - Render vertices with a texture
- Supporting customizable ambient & diffuse lighting
- Objects in the scene follow a scene graph hierarchy
- All engine settings and variables can be safely accessed from its entry point `Engine::GameInterface`
- Objects can be tinted
- Objects can freely be moved, scaled (wip) and rotated in runtime
- Scene Objects (`VirtualObjects`) can be created by picking components from `src/engine/classes/NodeComponents`