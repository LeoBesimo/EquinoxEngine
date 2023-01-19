# Equinox Engine

A small Game Framework written in C++ using the Win32 api. Written for Learning purposes.

1.0 Release comming soon

## [Modules](docs/Modules.md)
- [Application](docs/application.md)
- [Renderer](docs/renderer.md)
- [Input](docs/input.md)
- [Math](docs/math.md)
- [Physics](docs/physics.md)

## Features
- [x] Creating a Window
- [x] Drawing simple Shapes to the Screen
- [x] Option to use Multithreaded Rendering
- [x] Supports Camera objects with Scaling Matricies and Position offsets
- [x] Rigid Body Physics Engine supporting
    - Circles
    - Boxes
    - Polygons
- [x] Support for 32 Bit Bitmap Textures
    - Loading from Disk
    - Saving to Disk
- [X] Sprites
    - Creating Sprite Objects from Textures
    - Scale and Translate Sprite Objects
    - Option to precompute Sprite Transformations
- [X] Small Math Library
    - 2D Vectors
    - 3D Vectors
    - 4D Vectors
    - 2x2 Matrix
    - Some Utility functions

## main.cpp
**Link the Libraries and Include the Header files**
```cpp
#include <Equinox.hpp>

equinoxAppEntryPoint
{
    eq::Application::SetWindowProperties(L"Window Title", Window Width, Window Height);
    
    eq::Application::SetApplicationUpdate([&](float delta){
        // Your update Code goes here
    });

    eq::Application::Start();

    return 0;
}
```

## Important Usage Information
 - The Bitmap Class was tested with Aseprite Bitmaps, chance for wrong loading with bitmaps from other programms
 - There is currently no option to draw ellipse outlines
 - Precomputed sprites don't get Scaled with the bound Camera matrix
 - There is no default Camera bound to the Renderer, a Camera object has to be assigned to the Renderer before the first draw call
 - The Renderer begins to flicker at high Framerates due to async update and render threads
 - Renderer flickers at large Camera scales due to async update and render threads
 - Can Crash due to Async update and render threads

## Important TODOs
 - [ ] Fix penetration distance calculation for Polygon and box shapes

## Definitve Features to Implement
- [ ] Multithreaded Rendering for Filled Shapes
- [ ] Midpoint Ellipse Algorithm
- [ ] Color for Ridgid Bodies
- [ ] Filled Triangles
- [ ] Sound Support
- [ ] Sound Manager
- [ ] Animation System
- [ ] Line Colliders
- [ ] Trigger Colliders
- [ ] Sync update and render threads
- [ ] Default bound Camera object

## Maybe Features to Implement
- [ ] Cameras with their own Viewport
- [ ] Render directly to Textures
- [x] Scale sprite objects on render using bound Camera matrix

## Resources and References

- [Physics Collision Resolution](http://www.chrishecker.com/images/e/e7/Gdmphys3.pdf)
- [Physics Engine](https://www.youtube.com/playlist?list=PLSlpr6o9vURwq3oxVZSimY8iC-cdd3kIs)
- [Win32 Api Renderer](https://www.youtube.com/playlist?list=PL5Lk2LPoiyAKDzUg1KKJkkqcvoK6VI6Jp)


## Feedback
Feel free to Fork the repository and open Pull Requests if you find any bugs or if you otherwise want to contribute to this project.

- - -
Licensed under Creative Commons Zero v1.0 Universal
