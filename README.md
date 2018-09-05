# Simpleton
About 18000 lines of C++ that I've used to make a few games

This isn't really a framework kind of game engine as it doesn't make any assumptions about the architecture of your game. It's a library. You can pick and choose the parts that you want to use and ignore the rest. Some parts of this library are very old and rarely used even by me. Some parts of this library aren't very good, they are there for backwards compatibility with some of my older games. But there are a few gems here and there (at least, I think they're pretty neat!). Most of the code is documented with comments. This README has a run-down of the stuff I use most often. Nothing in this library is really that complicated so this documentation might not be necessary.

## Dependecies

This library is header-only so most of these are optional.

 * __C++17__ There are various usages of C++17 features around the place.
 * [EnTT](https://github.com/skypjack/entt) The best ECS framework ever!
 * [GLM](https://github.com/g-truc/glm) This is another header-only library that is heavily used throughout the entire library
 * [libPNG](https://github.com/glennrp/libpng) This is used in the spritesheet packer for writing PNGs. STB_image_write was too slow
 * [zlib](https://github.com/madler/zlib) A dependency of __libPNG__
 * [nlohmann_json](https://github.com/nlohmann/json) This is only used in a couple of places. You won't need it if you aren't already using it
 * [STB](https://github.com/nothings/stb) A couple of the STB libraries are embedded in the source
 * [SDL2](https://www.libsdl.org/download-2.0.php) There's a paper-thin SDL2 wrapper in the [SDL module](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Simpleton/SDL).
 * __OpenGL__ There's also a paper-thin OpenGL wrapper in the [OpenGL module](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Simpleton/OpenGL) which is used by the [Graphics 2D module](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Simpleton/Graphics%202D)

## Documentation

This documentation doesn't cover everything. Go explore!

### [Grid](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Simpleton/Grid)

This module provides utilities for dealing with 2D grids. It is intended for use within tile based games.

#### [Grid container](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Grid/grid.hpp)

This is a 2D array container. Supports functions for dealing with positions and indicies. It exposes iterators to the underlying `std::vector` as well as some custom iterators for cool stuff! Here's a snippet from the map loader of __Classic Tower Defence__.

```C++
Map &map = reg.get<Map>();
for (const Grid::Coord y : map.vertRev()) {
  for (const Grid::Coord x : map.hori()) {
    map(x, y) = tileChar(file.get());
  }
  if (file.get() != '\n') {
    file.unget();
  }
}
```

`Grid::vertRev()` returns a range from `height - 1` to `0` for use in a range-based for loop. Use a range-based for loop is much less error prone than something like this:

```C++
for (Grid::Coord y = grid.height() - 1; y != -Grid::Coord{1}; --y) {}
// equivilent to...
for (const Grid::Coord y : grid.vertRev()) {}
```

#### [A*](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Grid/a%20star.hpp) and [One Path](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Grid/one%20path.hpp)

These are two maze solving algorithms. Both of them will take a grid and find a path between two points and then return a `std::vector<Pos>`. __A*__ will find the shortest path. __One Path__ will find the only path. If you know ahead-of-time that there is only one path between the two points, then this is much faster than A*. Both of these algorithms take a function as a parameter. This function should return true if a tile is not a path tile.

Here's a snippet from __Classic Tower Defence__ that finds the path for units to follow:

```C++
const auto notPath = [] (const TileType type) {
  return type == TileType::PLATFORM || type == TileType::TREE;
};
info.path = Grid::astar(map, notPath, info.entry, info.exit);
```

`Grid::astar` can easily be swapped out for `Grid::onePath` as they both have the same interface.

#### [Dir](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Grid/dir.hpp)

This an orthogonal direction enum that is unbelevibly useful in tile based games. The game logic in __The Machine__ heavily uses `Grid::Dir`. At its heart, `Grid::Dir` is really just this:

```C++
enum class Dir : uint8_t {
  UP,
  RIGHT,
  DOWN,
  LEFT
};
```

But its so much more! I won't mention everything it has to offer here. There's `Grid::Dir::NONE` for representing a "null" direction. There's `Grid::opposite` to get the opposite direction. This function (and many others) use bit twiddling under the hood for maximum speed. Converting a direction to a `glm::vec` is quite handy. You can configure the conversion using template parameters:

```
using MyToVec = Grid::ToVec<float, Grid::Dir::RIGHT, Grid::Dir::DOWN>;

// later...

const glm::vec2 vec = MyToVec::conv(dir);
```

The first template parameter is the type of the coordinates in the vector, the second template parameter is the direction of positive-x and the third template parameter is the direction of positive-y.

One more thing I will mention is `Grid::DIR_RANGE`. This is a range that allows you to iterate the four directions in a range-based for loop. This feature is heavily used in the systems of __The Machine__. Here's a snippet from the input system in __The Machine__

```C++
for (const Grid::Dir dir : Grid::DIR_RANGE) {
  if (isEnabled(states.dirs[static_cast<size_t>(dir)])) {
    return setDesiredDir(registry, dir);
  }
}
```

#### [Dir bits](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Grid/dir%20bits.hpp)

This is a bitset with one bit for each direction. This (as well as Dir) is heavily used in the game logic of __The Machine__. There's lots of bit twiddling under the hood for maximum speed! It does all that you would expect a bitset to do. Here's an example:

```C++
Grid::DirBits dirs = Grid::DirBits::RIGHT_LEFT;
Grid::test(dirs, Grid::Dir::UP); // false
Grid::test(dirs, Grid::Dir::LEFT); // true
Grid::set(dirs, Grid::Dir::DOWN);
Grid::reset(dirs, Grid::Dir::RIGHT);
Grid::test(dirs, Grid::Dir::DOWN); // true
Grid::test(dirs, Grid::Dir::RIGHT); // false
```

### [Camera 2D](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Camera%202D)

This is a modular and extensible 2D camera. This module depends on __GLM__ for matricies and vectors.

#### [Camera](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Camera%202D/camera.hpp)

Modules can be written to customize animation of the three properties: position, zoom and angle. If you don't want to animate them, you can forget they exist. This is the simplest usage of the camera:

```C++
// at startup
Cam2D::Camera cam;
cam.setPos({2.0f, 3.0f});

// before rendering
Cam2D::Params params
params.aspect = 16.0f / 9.0f; // the aspect ratio of the window
params.delta = 1000.0f / 60.0f; // number of seconds since the last update
// no need to set params.delta if you're not animating

// This does animation and recalculates the matricies
cam.update(params);

// This is the view_proj matrix that you can send off to OpenGL
// This matrix maps game coordinates to OpenGL's normalized coordinate system
const glm::mat3 viewProj = cam.transform.toPixels();
```

Of course, cameras are never this simple. Camera parameters (`Cam2D::Params`) were introduced in the previous snippet. These parameters influence the animation of properties and the calculation of matricies. Camera properties (`Cam2D::Props`) are things that can be set and animated. The camera currently supports three properties: position (pos), scale (zoom) and rotation (angle). For each property, there is a class that calculates the target value for that property (`Cam2D::Target`) and a class that calculates the current value of the given the target (`Cam2D::Animate`). A few implementations of these are provided in the module but writing one yourself is trivial.

#### [Animate](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Camera%202D/animate.hpp) and [Target](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Camera%202D/target.hpp)

Objects that implement `Cam2D::Target` and `Cam2D::Animate` must be passed to `Cam2D::Camera::update`. If a `Cam2D::Target` object is not provided for a particular property then that property will remain the same. If a `Cam2D::Animate` object is not provided for a property then that property will be set to the target (without animating). On fair useful implementation of `Cam2D::Target<PropID::ZOOM>` is `Cam2D::ZoomToFit`. This will set the target zoom such that a rectangle (of a given size) will always fit within the bounds of the camera. This is incredibly useful if you want to camera to automatically zoom if the player resizes the window. Here's an example:

```C++
Cam2D::Camera cam;
// make sure that a 10 by 10 grid will always fit perfectly on the screen
Cam2D::ZoomToFit zoomToFit{{10, 10}};
// make sure that the zoom property changes by half a unit every second
// without this, zoom will just be set instantly to whatever the ZoomToFit returns
Cam2D::ZoomConstantSpeed zoomConSpeed{0.5};

// setup parameters same as before
Cam2D::Params params;
// The aspect ratio of the window
// You'll probably want to query the size of the window
params.aspect = 16.0f / 9.0f;
// The number of seconds since the last update
// You'll probably want to use a timer
params.delta = 1000.0f / 60.0f;

// the order of the parameters following params doesn't matter
// through the power of templates, everything will just work
cam.update(params, zoomToFit, zoomConSpeed);
```

#### [Transform](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Camera%202D/transform.hpp)

The sole purpose of `Cam2D::Transform` is to take the camera properties and parameters and calculate the matricies. There is more than one way to calculate the matricies. The `toPixels` function returns a matrix that maps game coordinates to normalized screen coordinates (not actually pixels). The `toMeters` function returns a matrix that maps normalized screen coordinates to game coordinates (not necessarily meters). Both of these functions return cached matricies that are updated when `calculate` is called. The `calculate` function is called by `Camera::update` so you don't need to worry about it.

The `setOrigin` function sets the position on the screen where (0,0) should be. If you want the coordinates (0,0) to coorespond to the top-left corner of the screen then you would do this:

```C++
cam.transform.setOrigin(Cam2D::Origin::TOP_LEFT);
```

The `setInvertX` and `setInvertY` will invert coordinates. By default, +x is right and +y is up. `setInvertY(true)` will make +y point down.

`Cam2D::Transform::setSize` is a peculiar function. You see, `Cam2D::ZoomToFit` isn't the fully solution to making a rectangle fit within the viewport of the camera. `Cam2D::ZoomToFit` will set the right zoom but sometimes the position also needs to be changed. Shifting the position requires information about the view port that only `Cam2D::Transform` has such as the origin and invert states. `setSize` will set the box size to shift the position to fit a rectangle on the screen. Here's a snippet from __Classic Tower Defence__

```C++
camera.transform.setOrigin(Cam2D::Origin::BOTTOM_LEFT);
camera.setPos({-4.0f, 0.0f});
zoomToFit.setSize({20.0f, 11.25f});
camera.transform.setSize({20.0f, 11.25f});
```

Note that the size given to `Cam2D::ZoomToFit` is the same as the size given to `Cam2D::Transform`. This is important. Everything falls to pieces if this isn't done. I'm thinking about putting the functionality of `Cam2D::ZoomToFit` into `Cam2D::Transform` so that the size only needs to be set once. I'm not sure how well that will go though.

### [Spritesheet](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Simpleton/Sprite)

This module consists of a class for dealing with sprite sheets and a tool for packing them. The packing tool depends on __libPNG__ which depends on __zlib__.

#### [Pack](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Pack)

There is a CMake script for building the packing tool but it isn't cross platform. I plan on writing a script that downloads libPNG and zlib and builds them from source. The tool also uses `dirent.h` to search directories so it will only work on Mac and maybe Linux. I plan on utilizing `std::filesystem` when it's more widely supported. To build the packing tool on a mac:

```sh
cd Pack
mkdir build
cd build
brew install libpng
brew install zlib
cmake ..
make
make install
```

The tool will print a list of available options if you pass it invalid arguments like `pack help`. I think the options are pretty self-explainatory. The tool will generate two files, a png spritesheet and a json atlas. 

If there are two files in a directory: `first.png` and `second.png`. Both of them can be packing into a spritesheet with this command:

```sh
pack sep=0
```

If both images are 16x16 then this will produce a `sprites.png` file that is 32x32. The spritesheet is always square and always a power-of-two in length. The atlas produced looks like this:

```json
{"length":64,"names":{"first":0,"second":1},"rects":[[0,32,32,0],[32,32,64,0]]}
```

Here it is pretty printed:
```json
{
  "length": 64,
  "names": {
    "first": 0,
    "second": 1
  },
  "rects": [
    [0, 32, 32, 0],
    [32, 32, 64, 0]
  ]
}
```

I must warn you that you shouldn't modify the atlas. The packer and unpacker aren't actually dealing with JSON. They're dealing with a binary format that just happens to be valid JSON. This makes producing and parsing the atlas quite a bit faster but if you put a space somewhere in the atlas, the unpacker will fail to load the atlas.

The atlas contains a mapping from names to indicies and a mapping from indicies to rectangles. This is handy for animation because you can carry around an ID and a count and then use that to calculate the index of a rectangle. Source images that end in numbers are sorted by that number. This means that if you had these files:

```
standing.png
jumping.png
running 0.png
running 1.png
running 2.png
running 3.png
walking 0.png
walking 1.png
walking 2.png
walking 3.png
```

You can take the ID of `running 0` and add the frame number to that ID to get the ID of the current sprite. For example:

```C++
Sprite::Sheet sheet = Sprite::makeSheet("sprites.atlas");
Sprite::ID running = sheet.getIDfromName("running 0");
// the following two lines are equivilent
Sprite::Rect running2 = sheet.getSprite(running + 2); // fast array lookup
Sprite::Rect running2 = sheet.getSprite("running 2"); // slow hash table lookup
```

#### [Sheet](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Simpleton/Sprite/sheet.hpp)

An example use of `Sprite::Sheet` was shown in the previous section. There is a factory function `Sprite::makeSheet` that loads the atlas file generated by the packing tool.

Accessing the `"names"` table is done through the `getIDfromName` method. This method returns a `Sprite::ID` which is an index into the `"rects"` array.

Looking up a rectangle on the `"rects"` array is done through the `getSprite` method. Passing a string to `getSprite` will lookup the index in the table and then lookup the rectangle in the array.

The size of the texture is also accessable through the `getLength` method. Remember that the image is always square and the side lengths are always powers of two.

Notice that `Sprite::sheet` has no idea what png is. The image part of the sprite sheet should be loaded using the Surface library described in the __Graphics 2D/Surface__ section. But any image loading library will do. There's nothing special about it. It's just an image! 

#### [Animation](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Simpleton/Sprite/anim.hpp)

Since source images are sorted by their frame number, animation sprites is quite easy. This is perfectly fine:

```C++
Sprite::Sheet sheet = Sprite::makeSheet("sprites.atlas");
Sprite::ID running = sheet.getIDfromName("running 0");
Sprite::ID frame = 0;

// animation loop
while (true) {
  frame = (frame + 1) % 4;
  Sprite::Rect rect = sheet.getSprite(running + frame);
}
```

Problems arise when you need to do more complicated things. Maybe you need the animation the stop and start. Maybe you need the animation to slow down. Luckily, there's an animation class that abstracts way all of the details involved with handling animations manually. Here's an example:

```C++
Sprite::Sheet sheet = Sprite::makeSheet("sprites.atlas");
Sprite::Anim running{sheet.getIDfromName("running 0"), 4};

// animation loop
while (true) {
  anim.incrRepeat();
  Sprite::Rect rect = sheet.getSprite(running.sprite());
}
```

In that example, there was a call to `incrRepeat`. There are a couple of other `incr*` functions that have slightly different behaviours.

 * `incr` Advances without checking if the frame has passed the end
 * `incrRepeat` Sets the frame to 0 if the frame passes the end
 * `incrCheck` Sets the frame to the last valid frame and returns `true` if the frame passes the end
 * `incrStop` Calls `stop()` if the frame passes the end

The animation has a running state. The animation can either be running or stopped. This state can be set by the `start`, `stop`, `resume`, `pause`, `toggle` methods. Calling any of the `incr*` methods on a stopped animation has no effect.

The animation also has a delay count. The delay is the number of times you have to call `incr*` in order to advance the animation by one frame. If you set the delay to `4` then you will have to call `incr*` 4 times to advance the animation by 1 frame. The delay can only be used to slow down the animation, not speed it up. Here's an example:

```C++
Sprite::Anim anim{0, 4};
anim.delay(2);
// delay can also be set with a float
anim.speed(0.5f);

             anim.frame(); // 0
anim.incr(); anim.frame(); // 0
anim.incr(); anim.frame(); // 1
anim.incr(); anim.frame(); // 1
anim.incr(); anim.frame(); // 2
```

Simply getting the frame number can be done with the `frame` method but this is rarely what you actually want to do. Get the `Sprite::ID` with the `sprite` method is what is commonly done. There is an overload of `sprite` for handling animation groups. Animation groups are describe in the following example:

Let's say you have these images in a `Sprite::Sheet`:

```
walk 0 0.png
walk 0 1.png
walk 0 2.png
walk 0 3.png
walk 1 0.png
walk 1 1.png
walk 1 2.png
walk 1 3.png
walk 2 0.png
walk 2 1.png
walk 2 2.png
walk 2 3.png
walk 3 0.png
walk 3 1.png
walk 3 2.png
walk 3 3.png
```

They are in the order that the packing tool will sort them. The animation frames are in this format `walk <dir> <frame>.png`.

The first number is the direction. `0` is up, `1` is right, `2` is down and `3` is left. This is the same order as `Grid::Dir`.

If you have a walking animation which four frames, you can use animation groups to select the right direction.

```C++
Sprite::Sheet sheet = Sprite::makeSheet("walking.atlas");
// notice that the animation has 4 frames
Sprite::Anim walk{sheet.getIDfromName("walk 0 0"), 4};
// now we need to select the group. This is done by doing 
// sprite() + group * frames
// or...
Sprite::ID sprite = walk.sprite(Grid::Dir::DOWN);
// equivalent to...
Sprite::ID sprite = walk.sprite() + 2 * 4; 
```

This is used in __Classic Tower Defence__ to animate the units. This is the system that renders units taken from __Classic Tower Defence__:

```C++
// these components are defined elsewhere
struct Position {
  glm::vec2 pos;
};
struct UnitSprite {
  Sprite::Anim walk;
  Sprite::Anim death;
};
struct UnitDir {
  Grid::Dir dir;
};

void unitRenderingSystem(ECS::Registry &reg, G2D::Section &sec) {
  const auto view = reg.view<Position, UnitDir, UnitSprite>();
  for (const ECS::EntityID entity : view) {
    sec.quad();
    sec.depth(Depth::UNIT);
    sec.tilePos(view.get<Position>(entity).pos);
    sec.tileTex(view.get<UnitSprite>(entity).walk.sprite(
      view.get<UnitDir>(entity).dir
    ));
  }
}
```

The `Grid::Dir` is passed to `Sprite::Anim::sprite(group)` which returns a `Sprite::ID` which is passed to `G2D::Section::tileTex` which looks up the `Sprite::Rect` in the `Sprite::Sheet` and writes it to the `G2D::Quad`.

### [Graphics 2D](https://github.com/Kerndog73/Simpleton-Engine/tree/master/Simpleton/Graphics%202D)

A 2D rendering abstraction that supports OpenGL 3.3, OpenGL ES 3.0 and WebGL 2.0. Implementations in Vulkan, Metal and DirectX 12 might appear in future. All that would need to change is the 200 lines of OpenGL code in [renderer.inl](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/renderer.inl).

#### [Surface](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/surface.hpp)

A surface is an image stored in RAM as opposed to a texture which is stored in VRAM. Surfaces are used in the spritesheet packer for loading images and constructing the spritesheet. Surfaces are pretty simple and I think the code speaks for itself so I won't go into too much detail here.

Surfaces can be [loaded](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/load%20surface.hpp) from any image format supported by [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h).

Surfaces can be [written](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/write%20surface.hpp) to PNGs using [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h). I chose to write PNGs using __libPNG__ in the spritesheet packer because I was generating very large spritesheets and __stb_image_write__ was quite slow.

Surfaces can be [blitted](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/blit%20surface.hpp) to each other. This means copy one surface onto another surface. This is used in the spritesheet packer to copy the source images onto the spritesheet.

Finally, surfaces can be [filtered](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/filter%20surface.hpp). This involves applying a function to each pixel to calculate a new pixel. The filter function should have this signature `std::array<uint8_t, DST_BPP>(std::array<uint8_t, SRC_BPP>)`.

#### [Renderer](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/renderer.hpp)

This is a complete abstraction of the underlying rendering library. All it does it render textured quads. Supporting Metal or Vulkan would involve reimplementing this class and that is all.

This is how you would render a texture that covers the whole window using this class. If you've ever used a low level rendering library like OpenGL then this should be pretty easy to understand.

```C++
G2D::Renderer renderer;
renderer.init();
const G2D::TexParams texParams = {
  .wrap = G2D::TexWrap::REPEAT,
  .min = G2D::MinFilter::LINEAR,
  .mag = G2D::MagFilter::LINEAR
};
// texture ID is just an index onto an array
// tex is 0
const G2D::TextureID tex = renderer.addTexture("my texture.png", texParams);

// rendering loop
while (true) {
  const G2D::QuadRange range = {0, 1};
  G2D::Quad quad;
  // trying to get this right is the reason why there are abstractions over this interface
  quad[0].pos = {-1.0f, -1.0f, 0.0f};
  quad[0].texCoord = {0.0f, 0.0f};
  quad[1].pos = {1.0f, -1.0f, 0.0f};
  quad[1].texCoord = {1.0f, 0.0f};
  quad[2].pos = {1.0f, 1.0f, 0.0f};
  quad[2].texCoord = {1.0f, 1.0f};
  quad[3].pos = {-1.0f, 1.0f, 0.0f};
  quad[3].texCoord = {0.0f, 1.0f};
  // the format for the quads on the CPU and GPU is exactly the same
  // writeQuads is essentially a memcpy from RAM to VRAM
  // for an OpenGL implementation, this is just glBufferSubData
  renderer.writeQuads(range, &quad);
  
  // now that our huge array of one quad is in VRAM, we can issue a draw call!
  
  // this object stores the values of the uniforms in the shaders
  G2D::RenderParams params;
  params.viewProj = {};  // the view_proj matrix is identity by default
  params.tex = tex;      // ID of the texture to bind
  params.color = {1.0f}; // Color to multiply by the texture pixels
  // this is the call to glDrawElements
  renderer.render(range, params);
}
```

#### [Quad Writer](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/quad%20writer.hpp)

Using the renderer directly is a pain. Luckily, there are abstractions! This example is the same as the previous example, except that it uses `G2D::QuadWriter`.

```C++
G2D::Renderer renderer;
renderer.init();
const G2D::TexParams texParams = {
  .wrap = G2D::TexWrap::REPEAT,
  .min = G2D::MinFilter::LINEAR,
  .mag = G2D::MagFilter::LINEAR
};
// a Sprite::Sheet and a G2D::TextureID
G2D::SheetTex sheetTex {{}, renderer.addTexture("my texture.png", texParams)};
G2D::QuadWriter writer;

//rendering loop
while (true) {
  G2D::Section &sec = writer.section(glm::mat3{}, sheetTex, glm::vec4{1.0f});
  sec.quad();
  sec.depth(0.0f);
  sec.tilePos({-1.0f, -1.0f}, {2.0f, 2.0f});
  sec.tileTex({0.0f, 0.0f}, {1.0f, 1.0f});
  writer.render(renderer);
}
```

A `G2D::Section` represents a group of quads and the uniform values (camera matrix, texture and color). A section also carries an extra piece of information: a `Sprite::Sheet`. This means that you can pass a `Sprite::ID` to `G2D::Section::tileTex` and the texture coordinates will automatically be added to the quad. Pretty cool huh?

`G2D::Section` maintains an array of `G2D::Quad`s. `quad()` pushes a new quad onto the the array. The pushed quad becomes the "current" quad. Functions like `depth()` and `tilePos()` modify the current quad.

Getting the depth of each quad correct can be difficult. This quad needs to above that quad and below this other quad... It's a headache. You need to put the depth information in one place. The order of every layer in your game must be defined in one place. I think the best way to do this is with an enum. This is the depth enum from __Classic Tower Defence__:

```C++
enum class Depth {
  UI_TEXT,
  UI_ELEM_1,
  UI_ELEM,
  UI_BASE,

  TOWER_RANGE,
  UNIT_HEALTH,
  UNIT_DEATH,
  TOWER_AURA,
  TOWER_GUN,
  TOWER_PROJ,
  TOWER_BEAM,
  UNIT,
  TOWER_BASE,
  MAP,
  
  COUNT
};
```

From this enum, we can clearly see that the UI must be above the game and the unit's health bar must be above the unit. If the depth of anything in the game needs to change, this enum is all that needs to change. `G2D::Section::depth` can take an enum instead of a float and calculate the depth from that. `sec.depth(Depth::UNIT)` instead of `sec.depth(0.1234f)`. Note that if the enum doesn't have a `COUNT` member then it is assumed that there are `255` layers. It is highly recommended that you include a `COUNT` member because of the precision of depth values in the depth buffer is typically only 8 or 16 bits so you want your layers to be as far apart as possible.

Here is the map rendering system from __Classic Tower Defence__. This is not a trick - it really is this simple!

```C++
// Note that ECS::Registry is entt::DefaultRegistry
// and       Map           is Grid::Grid<TileType>
void mapRenderingSystem(ECS::Registry &reg, G2D::Section &sec) {
  const Map &map = reg.get<Map>();
  const MapSprites &sprites = reg.get<MapSprites>();
  for (size_t t = 0; t != map.area(); ++t) {
    const Sprite::ID id = sprites.sprite + static_cast<Sprite::ID>(map[t]);
    sec.quad();
    sec.depth(Depth::MAP);
    sec.tilePos(map.toPos(t));
    sec.tileTex(id);
  }
}
```

The map rendering system is a perfect example of how `G2D::Section` should be used. You would typically initialize a `G2D::Section` with the game spritesheet and pass it to all of your rendering systems. The systems would push a few quads and happy days!

#### [Z Sort](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/zsort.hpp)

There are times where the pixels on the texture have an alpha value that is between 0 and 1. So you have to sort. The zsort header contains a few predicates for sorting quads. You would typically just use `G2D::sort` without a second thought but there are others just-in-case! You can pass these sorting predicates to `G2D::Section::sort`.

```C++
sec.sort(G2D::sort);
```

It couldn't be simpler!

#### [Quad Writer (Lite)](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/quad%20writer%20lite.hpp)

`G2D::QuadWriterLite` is very similar to `G2D::QuadWriter` except that there are no `G2D::Section` objects. There is one array of quads and sections are ranges on this array. This makes `G2D::QuadWriterLite` slightly faster than `G2D::QuadWriter` at the cost of being a bit inflexible. `G2D::QuadWriterLite` also doesn't deal with `Sprite::Sheet` so you can't just pass in a `Sprite::ID`. This class is actually a previous version of the quad writer that I renamed to be the "lite" version.

Each instance of `G2D::Section` has it's own array of quads so you can write to many `G2D::Section`s at once in any order you like. With `G2D::QuadWriterLite`, you don't have this luxury. You can only write quads to the current section, much like you can only modify the current quad. You change the section by calling `G2D::QuadWriterLite::section` with a `G2D::RenderParams`. Sorting is also possible. Sorting is done on a per-section basis, much like `G2D::QuadWriter`.

The neat thing about `G2D::QuadWriterLite` is that they can be appended. This is possible because it stores one single array of quads. You can write quads to two separate writers and then append one to the other. In a way, this gives you the flexibility of `G2D::Section` but I think it's kind of messy.

#### [Text](https://github.com/Kerndog73/Simpleton-Engine/blob/master/Simpleton/Graphics%202D/text.hpp)

This class calculates the positions of glyphs of a monospaced font and writes them to a `G2D::Section`. For __Classic Tower Defence__, I created my own bitmap font and used this class to render it. I was really proud of the results! I love the look of old school pixel fonts!

This is the very first test of the text renderer. I nearly fell out of my chair when I saw this!

![First test](https://i.imgur.com/J11H0H6.png)

Isn't that a sight to behold!

`G2D::Text` holds a reference to a `G2D::Section`. The section should have a compatible spritesheet. A compatible sprite sheet should have all of the printable ASCII characters. That's `!` up to `~`. Sprite ID 0 should be `!`, Sprite ID 1 should be `"`, etc. I did this by naming the source images `33.png`, `34.png`, etc.

Writing text is fairly straight forward. It can be as simple as this:

```C++
text.writeLeft({4.0f, 5.0f}, "Hello World!");
// or do some fancy center alignment
text.writeCenter({4.0f, 5.0f}, "Hello World!");
```

All of the special whitespace characters that ASCII has to offer are supported so you can use tabs, newlines and carriage returns and everything will work as expected.
