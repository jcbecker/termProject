//Author: João C. Becker
#ifndef PTG_HPP
#define PTG_HPP


#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>
#include <camera.hpp>
#include <PerlinNoise.hpp>
#include <TerrainGen.hpp>

#include <vector>
//Global settings
//initial screen size
const unsigned int ISCR_W = 100 * 16;//WIDTH
const unsigned int ISCR_H = 100 * 9;//HEIGHT


static void errorCallback(int error, const char* description);//to show some glfw errors

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);//input callBack

void framebufferSizeCallback(GLFWwindow* window, int width, int height);//resize screen

void processInputHoldingKey(GLFWwindow *window, float deltaTime);//to manage keys holding down

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);//send mouse position to the camera

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);//callback to get mouse scroll events and change zoom matrix in the view of camera

#endif