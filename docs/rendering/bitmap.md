# eq::BitmapTexture

The Bitmap Texture class is used to create, load and save bitmap textures.

## [Renderer](../renderer.md)

#### Constructors
- ```cpp
    eq::BitmapTexture();
    // Creates an empty Bitmap Texture with Width 0 and Height 0
  ```

- ```cpp
    eq::BitmapTexture(unsigned int width, unsigned int height);
    // Creates an empty Bitmap Texture with a set Width and Height
  ```

#### Getters
- ```cpp
    uint32_t getPixel(unsigned int x, unsigned int y);
    // Returns the Pixel Color at the point (x,y)
    // If (x,y) is outside the Texture it returns 0
  ```

#### Setters
- ```cpp
    void setPixel(unsigned int x, unsigned int y, uint32_t color);
    // Sets the Pixel at (x,y) to the passed color if (x,y) is contained on the Texture
  ```

#### Utility
- ```cpp
    void invertX();
    // Flips the Texture along the X axis
  ```

- ```cpp
    void invertY();
    // Flips the Texture along the Y axis
  ```
  
- ```cpp
    bool read(const char* filePath);
    // Loads the Texture from a bmp File from Disk.
    // Returns True if load was successfull
  ```
  
- ```cpp
    bool save(const char* filePath);
    // Saves the Texture to a bmp File on the Disk
    // Returns True if save was successfull
  ```