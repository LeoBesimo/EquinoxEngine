# eq::Sprite

The Sprite class turns Bitmap Textures into Sprites which can be Rendered. 

## [Renderer](../renderer.md)

#### Constructors
- ```cpp
  eq::sprite();
  // Creates an empty Sprite object with Width 0 and Height 0
  ```

- ```cpp
  eq::Sprite(unsigned int width, unsigned int height);
  // Creates an empty Sprite object with set Width and Height
  ```

- ```cpp
  eq::Sprite(eq::BitmapTexture& texture);
  // Creates a Sprite from the complete Bitmap Texture
  ```

- ```cpp
  eq::Sprite(eq::BitmapTexture& texture, unsigned int width, unsigned int height);
  // Creates a Sprite from the Bitmap Texture with set Width and Height
  // If Width and Height are smaller than the Texture dimensions only part of the Texture gets loaded into the Sprite
  ```

- ```cpp
  eq::Sprite(eq::BitmapTexture& texture, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height);
  // Creates a Sprite from the Bitmap Texture with set Width and Height starting from the xOffset and yOffset on the Texture
  ```

#### Getters
- ```cpp
  Math::Vector2 getPosition();
  // Returns the Position of the Sprite in World Space
  ```

- ```cpp
  uint32_t getPixel(unsigned int x, unsigned int y);
  // Returns the Pixel Color at the point (x,y)
  // If (x,y) is outside the Texture it returns 0
  ```

- ```cpp
  bool isCameraDependent();
  // Returns true if the Shape gets Transformed with the Camera transform
  ```  

#### Setters

- ```cpp
  void setPosition(eq::Math::Vector2 position);
  // Sets the Position of the Sprite
  ```

- ```cpp
  void setCameraDependent(bool dependent);
  // Sets the flag for Camera dependency
  ```

#### Utility

- ```cpp
  void move(eq::Math::Vector2 direction);
  // Moves the Sprite position along the direction for the magnitude of the direction
  ```

- ```cpp
  void rotate(float angle);
  // Sets the Rotation of the Sprite to the passed Angle
  ```

- ```cpp
  void scale(float scaleX, float scaleY);
  // Sets the Scale of the Sprite to the passed ScaleX and ScaleY
  ```

- ```cpp
  void preprocessSprite();
  // Precalculates the Rotation and Scaling for static sprites that do not get dynamically Transformed
  ```