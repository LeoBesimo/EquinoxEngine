# Shapes

Shape objects are used for multithreadded Rendering. Shapes are derived of a Drawable class

## [Renderer](../renderer.md)

## Shape Types
- Ellipse
- Line
- Rectangle
  
## Usage
Create a shape shape object and pass it to the renderer

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
    void setPosition(Math::Vector2 position);
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

### Ellipse Shape
#### Constructors

- ```cpp
    eq::Ellipse(eq::Math::Vector2 position, float radius1, float radius2);
    // Creates an Ellipse shape with 2 different Radii
  ```

- ```cpp
    eq::Ellipse(eq::Math::Vector2 position, float radius1, float radius2, eq::Color color);
    // Creates an Ellipse shape with 2 different Radii and a Color
  ```

- ```cpp
    eq::Ellipse(eq::Math::Vector2 position, float radius1);
    // Creates an Circle shape
  ```

- ```cpp
    eq::Ellipse(eq::Math::Vector2 position, float radius1, eq::Color color);
    // Creates an Circle shape with a Color
  ```

#### Getters
- ```cpp
    void getRadius(float& radius1, float& radius2);
    // Writes the Radii to the 2 passed parameters
  ```

- ```cpp
    void getRadius(float& radius);
    // Writes the first Radius to the passed paramater 
    //Circles only
  ```

#### Setters

- ```cpp
    void getRadius(float& radius1, float& radius2);
    // Writes the Radii to the 2 passed parameters
  ```

- ```cpp
    void getRadius(float& radius);
    // Writes the first Radius to the passed paramater
    // Circles only
  ```

### Line Shape
#### Constructors
- ```cpp
  
  //
  ```

- ```cpp
  
  //
  ```

#### Getters
#### Setters


### Rectangle Shape
#### Constructors
- ```cpp
  
  //
  ```

- ```cpp
  
  //
  ```
  
#### Getters
#### Setters