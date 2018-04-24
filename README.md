# termProject
Repository to organize my term project, is a terrain generator
![3-dimensional perspective height map](images/screenShot1.png?raw=true "Preview")

### Abstract
The larger the scenario of a game, more time a player will spend to exploit it.
Traditionally, computer games scenarios were built using large graphic design teams. This work
details a technique able to procedurally generate a pseudo-infinite terrain, with 5 visually
distinct biomes, spread them in regions and smooth their borders. The area, frequency, adjacency
and smoothness of the biomes and the terrain can be adjusted through parameters. The resulting
work is a terrain generator suited for computer games development.

## How to build

```shell
git clone https://github.com/jcbecker/termProject.git
cd termProject
git submodule init
git submodule update
cd ptg/cpp/
mkdir build
cd build
cmake ..
make
./ptg
```
