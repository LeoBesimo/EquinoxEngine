# eq::Renderer

This module handles singlethreadded and multithreadded Rendering.

[![GitHub Streak](http://github-readme-streak-stats.herokuapp.com?user=leobesimo&theme=dark&background=000000)](https://git.io/streak-stats)

## [Readme](../README.md)

## Sub Modules
- [Drawable](rendering/drawable.md)
- [Camera](rendering/camera.md)
- [Shapes](rendering/shapes.md)
- [Text](rendering/text.md)
- [Sprite](rendering/sprite.md)
- [Bitmap Textures](rendering/bitmap.md)

### Color Struct

- ```cpp
  struct eq::Color
  {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t alpha = 255;
  }
  // The Fields of the Color Struct
  ```

- ```cpp
  eq::Color();
  // Creates a Color Struct with the default Color Black
  ```

- ```cpp
  eq::Color(uint8_t red, uint8_t green, uint8_t blue);
  // Creates a Color Struct with set Red, Green and Blue Values
  ```

- ```cpp
  eq::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
  // Creates a Color Struct with set Red, Green, Blue and Alpha values
  ```

### Rect Struct

- ```cpp
  struct eq::Rect
  {
    int x;
    int y;
    int width;
    int height;
  }
  // The Fields of the Rect Struct
  ```

- ```cpp
  eq::Rect(float x, float y, float width, float height);
  // Creates a Rect Struct to Draw an axis aligned Rectangle
  ```

#### Draw Functions

- ```cpp
  void eq::Renderer::SetClearColor(const eq::Color& color);
  // Sets the Clear color of the Renderer
  ```

- ```cpp
  void eq::Renderer::SetPixel(int x, int y, const eq::Color& color);
  // Sets the Pixel at (x,y) to the Color
  ```
- ```cpp
  void eq::Renderer::SetPixel(int x, int y, const uint32_t color);
  // Sets the Pixel at (x,y) to the Color
  ```

- ```cpp
  void eq::Renderer::FillRectangle(const eq::Rect& rect, const eq::Color& color);
  // Draws a filled axis aligned Rectangle based on the Rect reference and Color
  ```

- ```cpp
  void eq::Renderer::DrawRectangle(const eq::Rect& rect, const eq::Color& color);
  // Draws a axis aligned Rectangle Outline based on the Rect reference and Color
  ```

- ```cpp
  void eq::Renderer::DrawLine(int x0, int y0, int x1, int y1, const eq::Color& color);
  // Draws a Line from (x0, y0) to (x1, y1) with the Color
  ```

- ```cpp
  void eq::Renderer::DrawLine(eq::Math::Vector2 a, eq::Math::Vector2 b, const eq::Color& color);
  // Draws a Line from A to B with the Color
  ```

- ```cpp
  void eq::Renderer::FillCircle(int originX, int originY, int radius, const eq::Color& color);
  // Draws a Filled Circle with the Center at (OriginX, OriginY) with the Radius and Color
  ```

- ```cpp
  void eq::Renderer::FillEllipse(int originX, int originY, int radiusX, int radiusY, const eq::Color& color);
  // Draws a Filled Ellipse with the Center at (OriginX, OriginY) with the RadiusX, RadiusY and color
  ```

- ```cpp
  void eq::Renderer::DrawCircle(int originX, int originY, int radius, const eq::Color& color);
  // Draws a Circle Outline with the Center at (OriginX, OriginY) with the Radius and Color
  ```

- ```cpp
  void eq::Renderer::DrawCircle(eq::Math::Vector2 position, int radius, const eq::Color& color);
  // Draws a Circle Outline with the Center at the Position Vector with the Radius and Color
  ```

- ```cpp
  void eq::Renderer::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const eq::Color& color);
  // Draws a Triangle Outline with the Vertecies (x0, y0), (x1, y1), (x2, y2) and the Color
  ```
- ```cpp
  void eq::Renderer::DrawSprite(eq::Sprite& sprite);
  // Draws the Sprite with its Transforms applied
  ```

- ```cpp
  void eq::Renderer::Draw(Ellipse ellipse);
  // Adds the Ellipse Shape to the Render Queue
  ```

- ```cpp
  void eq::Renderer::Draw(Line line);
  // Adds the Line Shape to the Render Queue
  ```

- ```cpp
  void  eq::Renderer::Draw(Rectangle rectangle);
  // Adds the Rectangle Shape to the Render Queue
  ```

- ```cpp
  void eq::Renderer::Draw(Text text);
  // Adds the Text Shape to the Render Queue
  ```

- ```cpp
  void eq::Renderer::Draw(Sprite sprite);
  // Adds the Sprite Object to the Render Queue
  ```

- ```cpp
  void eq::Renderer::Draw(eq::Physics::PhysicsWorld& world);
  // Adds all the Physics Bodies contained in the World to the Render Queue
  ```

#### Utility

- ```cpp
  void eq::Renderer::SetCamera(std::shared_ptr<eq::Camera> camera);
  // Binds a Camera Instance to the Renderer
  ```

- ```cpp
  eq::Math::Vector2 eq::Renderer::WorldToScreenspace(eq::Math::Vector2 p);
  // Transforms the Vector p with the Bound Camera from Worldspace to Screenspace
  ```

- ```cpp
  eq::Math::Vector2 eq::Renderer::ScreenToWorldspace(eq::Math::Vector2 p);
  // Transforms the Vector p with the Bound Camera from Screenspace to Worldspace
  ```

- ```cpp
  eq::Math::Vector2 eq::Renderer::ApplyCameraPosition(eq::Math::Vector2 p);
  // Applies the Bound Camera Position to the Vector p
  ```

- ```cpp
  eq::Math::Vector2 eq::Renderer::ApplyCameraTransform(eq::Math::Vector2 p);
  // Applies the Bound Camera Transform to the Vector p
  ```