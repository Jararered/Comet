#include "Mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex> *vertices, std::vector<unsigned int> *indices, ShaderProgram *shader)
    : m_Vertices(vertices), m_Indices(indices), m_Shader(shader), m_Count(static_cast<unsigned int>(indices->size())),
      m_PushedToGPU(false), m_ModelMatrix(1.0f), m_TimeCreated(glfwGetTime())
{
}

Mesh::~Mesh() {}

void Mesh::Bind()
{
    glUseProgram(m_Shader->GetID());
    glBindVertexArray(m_VAO);
}

void Mesh::Unbind()
{
    glUseProgram(0);
    glBindVertexArray(0);
}

void Mesh::Initialize()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_IBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * m_Vertices->size() * sizeof(Vertex), (void *)0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices->size() * sizeof(Vertex), m_Vertices->data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * m_Indices->size() * sizeof(unsigned int), (void *)0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Indices->size() * sizeof(unsigned int), m_Indices->data());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, m_Position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, m_TextureCoordinate)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, m_Normal)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_PushedToGPU = true;

    // m_Vertices->clear();
    // m_Vertices->shrink_to_fit();
    // m_Indices->clear();
    // m_Indices->shrink_to_fit();
}

void Mesh::Update()
{
    m_TimeDelta = glfwGetTime() - m_TimeCreated;

    if (m_TimeDelta < 0.25)
    {
        m_ModelMatrix = glm::translate(
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)),
            glm::vec3(0.0f, static_cast<float>(10.0 * glm::sin(2 * 3.141592653589 * m_TimeDelta)), 0.0f));
        m_Transparency = m_TimeDelta * 4.0;
    }
    else
    {
        m_ModelMatrix = glm::mat4(1.0f);
        m_Transparency = 1.0;
    }
}

void Mesh::UpdateGeometry()
{
    m_Count = static_cast<unsigned int>(m_Indices->size());

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices->size() * sizeof(Vertex), m_Vertices->data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_Indices->size() * sizeof(unsigned int), m_Indices->data());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Finalize()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);

    m_PushedToGPU = false;
}
