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
