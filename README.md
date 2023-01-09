# EquinoxEngine

A small Game Framework written in C++ using the Win32 api. Written for Learning purposes.

1.0 Release comming soon

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

## Usage
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

## Important TODOs
- Rewrite Sprite Class to contain Original and editable Buffer

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
