#include "engine.h"
#include <iostream>

const color borderHover(1, 0, 0);
const color hoverTileColor(0, 0, 0);


Engine::Engine() : keys() {
    this->initWindow();
    this->initShaders();
    this->initShapes();
}

Engine::~Engine() {}

unsigned int Engine::initWindow(bool debug) {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(width, height, "engine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // OpenGL configuration
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    return 0;
}

void Engine::initShaders() {
    // load shader manager
    shaderManager = make_unique<ShaderManager>();

    // Load shader into shader manager and retrieve it
    shapeShader = this->shaderManager->loadShader("../res/shaders/shape.vert", "../res/shaders/shape.frag",  nullptr, "shape");

    // Set uniforms that never change
    shapeShader.use();
    shapeShader.setMatrix4("projection", this->PROJECTION);
}

void Engine::initShapes() {
    // Size of the tiles
    const int tileSize = 75;
    // Variable for 5x5 grid
    const int numTilesInGrid = 5;
    // Spacing in between tiles
    const int spacing = 10;
    // Size of the underlying tile
    const int hoverTile = 80;
    // Total width and height that the tiles in grid will take up in the window
    int totalWidth = numTilesInGrid * tileSize + ((numTilesInGrid - 1) * spacing);
    int totalHeight = numTilesInGrid * tileSize + ((numTilesInGrid - 1) * spacing);

    // Starting x position for grid
    float startX = (width - totalWidth) / 2.0f;
    // Starting y position for grid
    float startY = (height - totalHeight) / 2.0f;

    // Visible lightsOn squares
    for (int row = 0; row < numTilesInGrid; row++) {
        std::vector<Rect> rects;
        for (int col = 0; col < numTilesInGrid; col++) {
            float xPos = (startX + (tileSize / 2)) + col * (tileSize + spacing);
            float yPos = (startY + (tileSize / 2)) + row * (tileSize + spacing);
            rects.push_back(Rect(shapeShader, vec2(xPos, yPos), vec2(tileSize), YELLOW));
        }
        tiles.push_back(rects);
    }
    // Pushes to a vector of the underlying squares that are invisible unless hovered over
    for (int hRow = 0; hRow < numTilesInGrid; hRow++) {
        for (int hCol = 0; hCol < numTilesInGrid; hCol++) {
            float xPos = (startX + (tileSize / 2)) + hCol * (tileSize + spacing);
            float yPos = (startY + (tileSize / 2)) + hRow * (tileSize + spacing);
            hoverSquare.push_back(make_unique<Rect>(shapeShader, vec2(xPos, yPos), vec2(hoverTile), hoverTileColor));
        }
    }
}

void Engine::processInput() {
    glfwPollEvents();

    // Set keys to true if pressed, false if released
    for (int key = 0; key < 1024; ++key) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            keys[key] = true;
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
            keys[key] = false;
    }

    // Close window if escape key is pressed
    if (keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, true);

    // Cursor input processing
    glfwGetCursorPos(window, &MouseX, &MouseY);
    MouseY = height - MouseY;
    // Changes the color of border if mouse is hovering over tile
    for (const unique_ptr<Shape> &hoverTile : hoverSquare) {
        bool hoverOverlapsMouse = hoverTile->isOverlapping(vec2(MouseX, MouseY));
        if (hoverOverlapsMouse) {
            hoverTile->setColor(borderHover);
        }
        else {
            hoverTile->setColor(hoverTileColor);
            }
    }


    // Turn lights off
    bool mousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    if (!mousePressed && mousePressedLastFrame) {
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                bool tileOverlapsMouse = tiles[row][col].isOverlapping(vec2(MouseX, MouseY));
                if (tileOverlapsMouse && mousePressedLastFrame) {
                    toggleTouchingTiles(row, col);
                }
            }
        }
    }

    mousePressedLastFrame = mousePressed;

}

void Engine::checkExist(int row, int col) {
    if (row >= 0 && row < 5 && col >= 0 && col < 5) {
        tiles[row][col].toggle();
    }
}

void Engine::toggleTouchingTiles(int row, int col) {
    tiles[row][col].toggle();
    checkExist(row - 1, col);
    checkExist(row + 1, col);
    checkExist(row , col - 1);
    checkExist(row, col + 1);
}

void Engine::update() {
    // Calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Engine::render() {
    glfwSwapBuffers(window);
    for (const unique_ptr<Shape> &h : hoverSquare) {
        h->setUniforms();
        h->draw();
    }
    // Render and draw the tiles on grid
    for (auto &row: tiles) {
        for (auto &t: row) {
            t.setUniforms();
            t.draw();
        }
    }

}

bool Engine::shouldClose() {
    return glfwWindowShouldClose(window);
}