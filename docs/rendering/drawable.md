# eq::Drawable

Drawable is a super class for Render shapes

## [Renderer](../renderer.md)

### Drawable
#### Getters

- ```cpp
    eq::Math::Vector2 getPosition();
    // Returns the Position of the Shape
  ```

- ```cpp
    eq::Color getColor();
    // Returns the Color of the Shape
  ```

- ```cpp
    bool isCameraDependent();
    // Returns true if the Shape gets Transformed with the Camera transform
  ```

#### Setters

- ```cpp
    void setPosition(eq::Math::Vector2 position);
    // Sets the Position of the Shape
  ```

- ```cpp
    void setColor(eq::Color color);
    // Sets the Color of the Shape
  ```

- ```cpp
    void setCameraDependent(bool dependent);
    // Sets the flag for Camera dependency
  ```