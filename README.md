# termProject
This is the repository of my term project: a terrain generator.
![3-dimensional perspective height map](images/screenShot1.png?raw=true "Preview")

### Abstract
The larger the scenario of a game, more time a player will spend to exploit it.
Traditionally, computer games scenarios were built using large graphic design teams. This work
details a technique able to procedurally generate a pseudo-infinite terrain, with 5 visually
distinct biomes, spread them in regions and smooth their borders. The area, frequency, adjacency
and smoothness of the biomes and the terrain can be adjusted through parameters. The resulting
work is a terrain generator suited for computer games development.

### Biomes
* The terrain may have 5 biomes, each one with different ways of calculating the relief;
* It's possible to create more biomes with few modifications;


![5 Terrain in distinct biomes](images/biomes.png?raw=true "Biomes")

* Biomes have square areas, the probability of the adjacent square being the same biome is controlled;

![6 Images with different frequencies of distribution of biomes](images/biomefrequency.png?raw=true "BiomesFrequency")

* The boundaries are continuous by interpolation;
* The distance from the interpolation is controlled.

![Discontinuous terrain and another continuous](images/interpolation1.png?raw=true "Border comparison1")

![4 images to see the influence of interpolation](images/interpolation2.png?raw=true "Border comparison2")



## How to use

### About

* The project was developed in arch linux with gnome and wayland, with no intention of being cross-platform;
* The x file was written to run on my pc, but it could probably run on other systems with some modifications;

* The [Perlin noise](<https://github.com/Reputeless/PerlinNoise>) implementation was writed by [Ryo Suzuki](<https://github.com/Reputeless>).

### Dependencies

* Graphic card whith driver who support modern OpenGL (3.3v and 4.5v);
* C++: compiler;
* GLFW: provided in git submodule;
* GLAD: provided in git submodule;
* GLM: provided in git submodule.

* Mail me if I forgot something, beckerjc95@gmail.com.
### How to build

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
```
* Run with...

```shell
./ptg
```

## License

termProject is licensed under the terms of the [MIT](https://choosealicense.com/licenses/mit/) Open Source license and is available for free.
