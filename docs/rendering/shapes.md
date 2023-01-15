# Shapes

Shape objects are used for multithreadded Rendering. Shapes are derived of a [Drawable](drawable.md) class

## [Renderer](../renderer.md)

## Shape Types
- [Ellipse](#ellipse-shape)
- [Line](#line-shape)
- [Rectangle](#rectangle-shape)
  
## Usage
Create a shape object and pass it to the renderer for multithreadded Rendering

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
    // Circles only
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
    eq::Line(eq::Math::Vector2 position1, eq::Math::Vector2 position2);
    // Creates a Line shape
  ```

- ```cpp
    eq::Line(eq::Math::Vector2 position1, eq::Math::Vector2 position2, eq::Color color);
    // Creates a Line shape with a Color
  ```

#### Getters
- ```cpp
    eq::Math::Vector2 getStartPos();
    // Returns the starting position of the Line
  ```

- ```cpp
    eq::Math::Vector2 getEndPos();
    // Returns the end position of the Line
  ```

#### Setters
- ```cpp
    void setStartPos(eq::Math::Vector2 startPos);
    // Sets the start position of the Line
  ```

- ```cpp
    void setEndPos(eq::Math::Vector2 endPos);
    // Sets the end position of the Line
  ```

### Rectangle Shape
#### Constructors
- ```cpp
    eq::Rectangle(eq::Math::Vector2 position, eq::Math::Vector2 dimension);
    // Creates an axis aligned Rectangle shape
  ```

- ```cpp
    eq::Rectangle(eq::Math::Vector2 position, eq::Math::Vector2 dimension, eq::Color color);
    // Creates an axis aligned Rectangle shape with a Color
  ```
  
#### Getters
- ```cpp
    eq::Math::Vector2 getDimension();
    // Returns the Dimensions of the Rectangle
  ```

#### Setters
- ```cpp
    void setDimension(eq::Math::Vector2 dimension);
    // Sets the Dimensions of the Rectangle
  ```