#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "manager.h"

FILE* inputFile;

int main(int argc, char* argv[]) {

    if (argc < 2)
        throw std::runtime_error("ERROR: Argumentos inv�lidos.\n");

    // Abre arquivo
    const char* input = argv[1];
     fopen_s(&inputFile, input, "r");


    // Inicializa a biblioteca
    if (!glfwInit())
        throw std::runtime_error("ERROR: N�o foi poss�vel inicializar o OpenGL.\n");

    // Cria uma janela e seu contexto OpenGL
    GLFWwindow* window;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RayTracer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    std::vector<glm::vec3> pos = readScreenData(inputFile);
    readLightData(inputFile);
    std::vector<Color> colorVec = readColorData(inputFile);
    std::vector<Texture> textureVec = readTextureData(inputFile);
    std::vector<Body> bodyVec = readBodyData(inputFile, colorVec, textureVec);

    // Define a janela como contexto atual
    glm::mat4 view = getView(pos);
    glfwMakeContextCurrent(window);


    while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawScene(bodyVec);

		glfwSwapBuffers(window);
		glfwPollEvents();
    }

    
	glfwDestroyWindow(window);
    glfwTerminate();
    return 13;
}