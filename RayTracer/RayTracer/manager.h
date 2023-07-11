#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <stdbool.h>
#include <string.h>
#include <cstdio>
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

// Constantes de tela
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Constantes de FPS
#define TARGET_FPS 1000

// Constantes de leitura
#define SCREEN_DATA_SIZE 4

// Cores
#define COL_CHECKER "checker"
#define COL_SOLID "solid"
#define COL_TEXMAP "texmap"

// Structs
struct Color {
    int colorId;
    std::string colorType;
    std::string colorFile;
    float vec3[3];
    float vec4[4];
    float col1[3];
    float col2[3];
    float lenght;
};

struct Texture {
    int textureId;
    int textureType;
};

struct Body {
    int bodyId;
    // dados de cor
    // dados de posição
    // dados de textura
};

// M�todos de meitura
std::vector<glm::vec3> readScreenData(FILE*);
void readLightData(FILE*);
std::vector<Color> readColorData(FILE*);
std::vector<Texture> readTextureData(FILE*);
std::vector<Body> readBodyData(FILE*, std::vector<Color>, std::vector<Texture>);
glm::mat4 getView(std::vector<glm::vec3>);

void drawScene(std::vector<Body>);