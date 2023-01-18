# eq::Application

This module is responsible for handling the Window creation and update loop. It is packed into a static class.   
Everything in this library is contained within the eq Namespace.

## [Readme](../README.md)

```cpp

#include "Equinox.hpp"

equinoxAppEntryPoint
{
    eq::Application::SetWindowProperties(L"Window Title", Window Width, Window Height); 
    eq::Application::SetApplicationUpdate([&](float delta){});
    eq::Application::Start();
}

```

## Functions
### Main Functions
- ```cpp
    void eq::Application::SetWindowProperties(const std::wstring% windowTitle, const int& width, const int& height);
    // Sets the Properties for the application window;
    // Default Application Parameters are L"Equinox Application", 1920, 1080
    ```
- ```cpp 
    void eq::Application::SetApplicationUpdate(const std::function<void(float delta)>& update);
    // Sets the Application update method;
    ```
- ```cpp
    void eq::Application::Start();
    // Starts the Application and creates the Window;
    ```
### Optional Functions
- ```cpp
    void eq::Application::SetFrameRate(float frameRate);
    // Sets the Maximum Framerate
    ```
- ```cpp
    bool eq::Application::IsRunning();
    // Returns True if the Application is Running
    ```
- ```cpp
    std::wstring eq::Application::GetWindowTitle();
    // Returns the window title
    ```
- ```cpp
    int eq::Application::GetWindowWidth();
    // Returns the window width in Pixels
    ```
- ```cpp
    int eq::Application::GetWindowHeight();
    // Returns the window height in Pixels
    ```
- ```cpp
    HWND eq::Application::GetWindowHandle();
    // Returns the Handle of the Application window
    ```
- ```cpp
    float eq::Application::GetFrameTime();
    // Returns the Time the last Frame took to Render in Seconds
    ```