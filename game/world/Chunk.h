#pragma once

#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include "glm/gtc/random.hpp"

#include "Engine.h"

#include "render/Renderer.h"
#include "render/containers/Vertex.h"
#include "render/containers/Mesh.h"

#include "Block.h"

struct Chunk
{
    Chunk(glm::ivec3 id)
        : m_Chunk(id),
          m_ChunkSize(16),
          m_ChunkHeight(16)
    {
        std::cout << "Chunk::Chunk()" << std::endl;
        m_Blocks.reserve(m_ChunkSize * m_ChunkSize * m_ChunkHeight);
        FillChunk(Block(1));
        GenerateMesh();
        // p_World->p_Engine->GetRenderer()->AddToMeshQueue(&m_Mesh);
    }

    ~Chunk()
    {
        std::cout << "Chunk::~Chunk()" << std::endl;
    }

    void SetBlock(glm::ivec3 coordinate, Block &block)
    {
        m_Blocks.insert_or_assign(coordinate, block);
    }

    void FillChunk(Block &block)
    {
        for (int i = 0; i < m_ChunkSize; i++)
        {
            for (int j = 0; j < m_ChunkHeight; j++)
            {
                for (int k = 0; k < m_ChunkSize; k++)
                {
                    SetBlock(glm::ivec3(i, j, k), block);
                }
            }
        }
    }

    void GenerateMesh()
    {
        std::vector<Vertex> m_Vertices;
        m_Vertices.reserve(8 * m_ChunkSize * m_ChunkSize * m_ChunkHeight);
        std::vector<unsigned int> m_Indices;
        m_Indices.reserve(36 * m_ChunkSize * m_ChunkSize * m_ChunkHeight);

        unsigned int offset = 0;

        for (int xRelPos = 0; xRelPos < m_ChunkSize; xRelPos++)
        {
            for (int yRelPos = 0; yRelPos < m_ChunkHeight; yRelPos++)
            {
                for (int zRelPos = 0; zRelPos < m_ChunkSize; zRelPos++)
                {
                    float xOffset = m_Chunk.x;
                    // float yOffset = m_Chunk.y;
                    float zOffset = m_Chunk.z;

                    float xPos = static_cast<float>(xRelPos) + xOffset;
                    float yPos = static_cast<float>(yRelPos);
                    float zPos = static_cast<float>(zRelPos) + zOffset;

                    float red = glm::linearRand(0.0f, 1.0f);
                    float green = glm::linearRand(0.0f, 1.0f);
                    float blue = glm::linearRand(0.0f, 1.0f);

                    m_Vertices.insert(m_Vertices.end(), {Vertex(xPos - 0.5f, yPos - 0.5f, zPos + 0.5f, red, green, blue),
                                                         Vertex(xPos + 0.5f, yPos - 0.5f, zPos + 0.5f, red, green, blue),
                                                         Vertex(xPos + 0.5f, yPos + 0.5f, zPos + 0.5f, red, green, blue),
                                                         Vertex(xPos - 0.5f, yPos + 0.5f, zPos + 0.5f, red, green, blue),
                                                         Vertex(xPos - 0.5f, yPos - 0.5f, zPos - 0.5f, red, green, blue),
                                                         Vertex(xPos + 0.5f, yPos - 0.5f, zPos - 0.5f, red, green, blue),
                                                         Vertex(xPos + 0.5f, yPos + 0.5f, zPos - 0.5f, red, green, blue),
                                                         Vertex(xPos - 0.5f, yPos + 0.5f, zPos - 0.5f, red, green, blue)});

                    m_Indices.insert(m_Indices.end(), {0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset,   // front
                                                       1 + offset, 5 + offset, 6 + offset, 6 + offset, 2 + offset, 1 + offset,   // right
                                                       5 + offset, 4 + offset, 7 + offset, 7 + offset, 6 + offset, 5 + offset,   // back
                                                       4 + offset, 0 + offset, 3 + offset, 3 + offset, 7 + offset, 4 + offset,   // left
                                                       0 + offset, 4 + offset, 5 + offset, 5 + offset, 1 + offset, 0 + offset,   // bottom
                                                       3 + offset, 2 + offset, 6 + offset, 6 + offset, 7 + offset, 3 + offset}); // top
                    offset += 8;
                }
            }
        }
        m_Mesh.AddGemoetry(m_Vertices, m_Indices);
    }

    Mesh *GetMesh() { return &m_Mesh; }

private:
    std::unordered_map<glm::ivec3, Block> m_Blocks;
    glm::ivec3 m_Chunk;
    unsigned int m_ChunkSize;
    unsigned int m_ChunkHeight;

    Mesh m_Mesh;
};