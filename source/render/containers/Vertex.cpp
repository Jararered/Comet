#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(float xPos, float yPos, float zPos)
    : position(xPos, yPos, zPos),
      color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, float r, float g, float b)
    : position(xPos, yPos, zPos),
      color(r, g, b, 1.0f)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, float r, float g, float b, float a)
    : position(xPos, yPos, zPos),
      color(r, g, b, a)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, glm::vec3 rgb)
    : position(xPos, yPos, zPos),
      color(rgb, 1.0f)
{
}

Vertex::Vertex(float xPos, float yPos, float zPos, glm::vec4 rgba)
    : position(xPos, yPos, zPos),
      color(rgba)
{
}
