#include "rect.h"
#include "../util/color.h"

Rect::Rect(Shader & shader, vec2 pos, vec2 size, struct color color)
        : Shape(shader, pos, size, color) {
    initVectors();
    initVAO();
    initVBO();
    initEBO();
    isOn = true;
}

Rect::Rect(Rect const& other) : Shape(other) {
    initVectors();
    initVAO();
    initVBO();
    initEBO();
}

Rect::~Rect() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Rect::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Rect::initVectors() {
    this->vertices.insert(vertices.end(), {
            0.5f, -0.5f,  // x, y of bottom right corner
            0.5f, 0.5f,  // top right corner
            -0.5f, -0.5f, // bottom left corner
            -0.5f, 0.5f, // top left corner
            -0.5f, -0.5f, // bottom left corner
            0.5f, 0.5f
    });

    this->indices.insert(indices.end(), {
            0, 1, 2, // First triangle
            1, 2, 3  // Second triangle
    });
}

bool Rect::isOverlapping(const vec2 &point) const {
    // A shape is overlapping a point if the point is within the shape's bounding box.
    if (getLeft() <= point.x && getRight() >=point.x) {
        if (getTop() > point.y && getBottom() < point.y) {
            return true;
        }
    }
    return false;
}

bool Rect::toggle() {
    //cout << isOn;
    if (isOn) {
        setColor(LIGHT_GRAY);
        isOn = false;
    }
    else {
        setColor(YELLOW);
        this->isOn = true;
    }
}

// Overridden Getters from Shape
float Rect::getLeft() const        { return pos.x - (size.x / 2); }
float Rect::getRight() const       { return pos.x + (size.x / 2); }
float Rect::getTop() const         { return pos.y + (size.y / 2); }
float Rect::getBottom() const      { return pos.y - (size.y / 2); }