# eq::Text

Text objects are used for rendering text on the render Thread. The Text class is derived from the [Drawable](drawable.md) class.

## [Renderer](../renderer.md)

#### Constructors

- ```cpp
    eq::Text();
    // Default constructor creates an empty text object
  ```


- ```cpp
    eq::Text(eq::Math::Vector2 position, std::wstring text);
    // Creates a Text object with set Text and Position
  ```


- ```cpp
    eq::Text(eq::Math::Vector2 position, std::wstring text, eq::Color);
    // Creates a Text object with set Text, Position and Color
  ```

#### Getters

- ```cpp
    std::wstring getText();
    // Returns the Text of the Text object
  ```

#### Setters

- ```cpp
    void setText(std::wstring& text);
    // Sets the Text of the Text object
  ```