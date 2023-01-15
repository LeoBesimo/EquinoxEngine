# eq::Camera

Camera object with a transformation which gets applied on Drawables on the render thread

## [Renderer](../renderer.md)

#### Constructors
- ```cpp
    eq::Camera();
    // Creates a Camera object at Position (0,0) with a default Transform
  ```

#### Getters
- ```cpp
    eq::Math::Vector2 getPosition();
    // Returns the current position of the Camera
  ```

- ```cpp
    eq::Math::Vector2 getDimension();
    // Returns the current dimension of the Camera
    // Dimension does not yet have a usage
  ```

- ```cpp
    eq::Math::Matrix2x2 getTransform();
    // Returns the current Transform of the Camera
  ```

#### Setters
- ```cpp
    void setPosition(eq::Math::Vector2 position);
    // Sets the position of the Camera
  ```

- ```cpp
    void setDimension(eq::Math::Vector2 dimension);
    // Sets the dimension of the Camera
    // Dimension does not yet have a usage
  ```

- ```cpp
    void setTransform(eq::Math::Matrix2x2 transform);
    // Sets the transform of the Camera
  ```

#### Utilities
- ```cpp
    void move(eq::Math::Vector2 direction);
    // Moves the camera position along the direction for the magnitude of the direction
  ```
