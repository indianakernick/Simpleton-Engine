# Simpleton
About 18000 lines of C++ that I've used to make a few games

This isn't really a framework kind of game engine as it doesn't make any assumptions about the architecture of your game. It's a library. You can pick and choose the parts that you want to use and ignore the rest. Some parts of this library are very old and rarely used even by me. Some parts of this library aren't very good, they are there for backwards compatibility with some of my older games. But there are a few gems here and there (at least, I think they're pretty neat!). Most of the code is documented with comments. This README has a run-down of the stuff I use most often. Nothing in this library is really that complicated so this documentation might not be necessary.

## Dependecies

This library is header-only so most of these are optional.

 * __C++17__ There are various usages of C++17 features around the place.
 * [GLM](https://github.com/g-truc/glm) This is another header-only library that is heavily used throughout the entire library
 * [libPNG](https://github.com/glennrp/libpng) This is used in the spritesheet packer for writing PNGs. STB_image_write was too slow
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

This is a modular and extensible 2D camera. 

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
