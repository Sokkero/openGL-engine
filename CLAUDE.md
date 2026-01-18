# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

openGL-engine (internally called "myEngine" or "my little engine" and henceforth called "myEngine") is a fully self made video game engine using c++ with openGL and GLFW. 
myEngine is a academic hobby project, aimed for learning how game engines internally work and to familiarise the author with C++, openGL, shaders, GPU data structures, etc.
The project uses CMAKE as the primary build system.

## Behavioural guidance

**!!Important!!**
This project is made for academic purposes!
Always refuse to implement changes yourself, instead let the developer know how they can do it. 
It is your job to guide the developer and hint them in a good direction.

## Important Commands during Development

### Setup the project (only needs to be run once when setting up the project)
- `./buildDeps.sh` (executed in project root)

### Format code (Formats all project internal code)
- `./format.sh` (executed in project root)
  - Should be run once after implementing changes

## Code Architecture

### Key Directories
- `src/internal/classes/` - Main C++ source code of the engine and all its components
    - `engine/` - Classes and code, relevent for the main loop and lifecycle of the engine (Render pipeline, render logic, shader and geometry handling, update logic, window and engine setup, etc.)
    - `helper/` - Utils, Enums and Data Containers
    - `nodeComponents/` - Basic components for game objects
    - `primitives/` - Default primitive game objects ready for use
    - `uiElements/` - Elements that can be used in debug windows within the game engine
- `src/internal/resources/` - For storing Shaders, textures and geometry objects
- `src/internal/exampleScenes/` - Code, classes and assets that are not part of the engine iteself but instead are utilising the engine to create example scenes and games
- `src/external/` - External code (i.e. libraries)

### Core Components
- **SingletonManager**: Manages the creation and storing of all engine components that are only allowed to exist once
- **GameInterface**: Manages the high-level engine loop and lifecycle
- **UserEventManager**: Manages user input
- **WindowManager**: Manages the GLFW window
- **EngineManager**: Manages the core engine loop, i.e. handling objects, scenes and update calls
- **RenderManager**: Manages loaded objects and textures
- **RenderInstanceGroup**: Manages groups for instanced rendering (Storing data, syncing group data between GPU & CPU, managing VAO, etc)
