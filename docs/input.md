# eq::Input

This module is responsible for handling Keyboard and Mouse Inputs. It is contained within a static class.   
Keys and Mouse Buttons are defined outside of the eq Namespace

## [Readme](../README.md)

- [eq::Input](#eqinput)
  - [Readme](#readme)
    - [Key State](#key-state)
    - [Utility](#utility)
    - [Keys](#keys)
    - [Mouse Buttons](#mouse-buttons)

### Key State
- ```cpp
   struct eq::KeyState
   {
     bool wasDown; // Was Key down previous Frame
     bool isDown; // Is Key down this Frame
   }
  // Depicts the State of a Single Key
  ```

### Position
- ```cpp
   struct eq::Position
   {
     int x;
     int y;
   } 
  // Used for Saving the Mouse Screenspace Position
  ```

### Utility
- ```cpp
  eq::KeyState eq::Input::GetKeyState(uint32_t keyCode);
  // Returns the Key State of the Key
  ```

- ```cpp
  bool eq::Input::IsKeyPressed(uint32_t keyCode);
  // Returns True if the Key is Pressed this Frame
  ```

- ```cpp
  bool eq::Input::WasKeyPressed(uint32_t keyCode);
  // Returns True if the Key was Pressed last Frame
  ```

- ```cpp
  bool eq::Input::WasKeyHit(uint32_t keyCode);
  // Returns True if the Key is Pressed this Frame and not Pressed in the Previous Frame
  ```


- ```cpp
  eq::Position eq::Input::GetMousePosition(Math::Vector2* position = nullptr);
  // Returns the Mouse Position and writes the Position into the passed Position Vector
  ```

- ```cpp
  bool eq::Input::IsMouseButtonPressed(unsigned int buttonCode);
  // Returns True if the Mouse Button is Pressed this Frame
  ```

- ```cpp
  bool eq::Input::WasMouseButtonPressed(unsigned int buttonCode);
  // Returns True if the Mouse Button was Pressed last Frame
  ```

- ```cpp
  bool eq::Input::WasMouseButtonHit(unsigned int buttonCode);
  // Returns True if the Mouse Button is Pressed this Frame and not Pressed last Frame
  ```

### Keys
| Key | Key Code |
|---|---|
|    `EQ_A`	|		0
|    `EQ_B`	|		1
|    `EQ_C`	|		2
|    `EQ_D`	|		3
|    `EQ_E`	|		4
|    `EQ_F`	|		5
|    `EQ_G`	|		6
|    `EQ_H`	|		7
|    `EQ_I`	|		8
|    `EQ_J`	|		9
|    `EQ_K`	|		10
|    `EQ_L`	|		11
|    `EQ_M`	|		12
|    `EQ_N`	|		13
|    `EQ_O`	|		14
|    `EQ_P`	|		15
|    `EQ_Q`	|		16
|    `EQ_R`	|		17
|    `EQ_S`	|		18
|    `EQ_T`	|		19
|    `EQ_U`	|		20
|    `EQ_V`	|		21
|    `EQ_W`	|		22
|    `EQ_X`	|		23
|    `EQ_Y`	|		24
|    `EQ_Z`	|		25 
|Arrow Keys| |
|    `EQ_UP`	|		26
|    `EQ_DOWN`|			27
|    `EQ_LEFT`|			28
|    `EQ_RIGHT`|		29
|Number Keys| |
|    `EQ_0`	|		30
|    `EQ_1`	|		31
|    `EQ_2`	|		32
|    `EQ_3`	|		33
|    `EQ_4`	|		34
|    `EQ_5`	|		35
|    `EQ_6`	|		36
|    `EQ_7`	|		37
|    `EQ_8`	|		38
|    `EQ_9`	|		39
|Special Keys | |
|    `EQ_MINUS`	|	40
|    `EQ_PLUS`	|		41
|    `EQ_SHIFT`	|	42
|    `EQ_CONTROL`	|	43
|    `EQ_ALT`		|	44
|    `EQ_SPACE`	|	45
|    `EQ_ESCAPE`	|	46
|    `EQ_CAPSLOCK`|		47
|    `EQ_TAB`		|	48
|    `EQ_ENTER`	|	49
|    `EQ_BACKSPACE`|	50
|    `EQ_TILDE`	|	51

### Mouse Buttons

| Button | Code |
|---|---|
| `EQ_MOUSE_LEFT` | 0
| `EQ_MOUSE_RIGHT` | 1
| `EQ_MOUSE_MIDDLE` | 2
| `EQ_MOUSE_X1` | 3
| `EQ_MOUSE_X2` | 4
