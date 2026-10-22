/**
 * Game Engine Development
 * Technische Hochschule Mittelhessen
 * Copyright © 2021-2026 Tobias Reimann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "engine/src/video/mesh.h"

#include "engine/src/core/application.h"
#include "engine/src/math/vector3.h"

#include <glad/glad.h>
#include <iostream>
#include <sstream>

using Engine::Core::Application;
using Engine::Math::Vector3;

namespace Engine::Video
{
    std::shared_ptr<Mesh> Mesh::create(const std::string &filename)
    {
        auto mesh = std::shared_ptr<Mesh>(new Mesh());
        MeshData data = { filename };

        bool success = readFile(data);
        if (!success) return mesh;

        createMesh(*mesh, data);

        return mesh;
    }

    bool Mesh::readFile(MeshData &data)
    {
        auto file = Application::load(data.filename);
        if (!file) return false;

        std::string line;
        while (getline(*file, line))
        {
            std::istringstream lineStream(line);
            std::string type;
            lineStream >> type;

            if (type == "v")
            {
                std::array<float, 3> position = {};
                lineStream >> position[0] >> position[1] >> position[2];
                data.positions.push_back(position);
            }
            else if (type == "vt")
            {
                std::array<float, 2> texCoord = {};
                lineStream >> texCoord[0] >> texCoord[1];
                data.texCoords.push_back(texCoord);
            }
            else if (type == "vn")
            {
                std::array<float, 3> normal = {};
                lineStream >> normal[0] >> normal[1] >> normal[2];
                data.normals.push_back(normal);
            }
            else if (type == "f")
            {
                std::vector<Vertex> face;
                std::string token;

                while (lineStream >> token)
                {
                    Vertex vertex = {};
                    std::istringstream tokenStream(token);
                    std::string index;

                    for (int component = 0; component < 3 && getline(tokenStream, index, '/'); component++)
                    {
                        if (index.find_first_not_of("0123456789") != std::string::npos) continue;

                        size_t i = stoul(index) - 1;
                        if (component == 0 && i < data.positions.size())
                        {
                            vertex.position = data.positions[i];
                        }
                        else if (component == 1 && i < data.texCoords.size())
                        {
                            vertex.texCoord = data.texCoords[i];
                        }
                        else if (component == 2 && i < data.normals.size())
                        {
                            vertex.normal = data.normals[i];
                        }
                    }

                    face.push_back(vertex);
                }

                for (size_t i = 2; i < face.size(); i++)
                {
                    data.vertices.push_back(face[0]);
                    data.vertices.push_back(face[i - 1]);
                    data.vertices.push_back(face[i]);
                }
            }
        }

        if (data.vertices.empty())
        {
            std::cout << "! Failed to read mesh " + data.filename + ": no faces" << std::endl;
            return false;
        }

        for (size_t i = 0; i < data.vertices.size(); i += 3)
        {
            Vertex &a = data.vertices[i];
            Vertex &b = data.vertices[i + 1];
            Vertex &c = data.vertices[i + 2];

            Vector3 edge1 = {b.position[0] - a.position[0], b.position[1] - a.position[1], b.position[2] - a.position[2]};
            Vector3 edge2 = {c.position[0] - a.position[0], c.position[1] - a.position[1], c.position[2] - a.position[2]};

            double du1 = b.texCoord[0] - a.texCoord[0];
            double dv1 = b.texCoord[1] - a.texCoord[1];
            double du2 = c.texCoord[0] - a.texCoord[0];
            double dv2 = c.texCoord[1] - a.texCoord[1];

            double determinant = du1 * dv2 - du2 * dv1;
            if (determinant == 0.0) continue;

            Vector3 tangent = Vector3::normalize((edge1 * dv2 - edge2 * dv1) / determinant);
            Vector3 bitangent = Vector3::normalize((edge2 * du1 - edge1 * du2) / determinant);

            a.tangent = {(float)tangent.x, (float)tangent.y, (float)tangent.z};
            a.bitangent = {(float)bitangent.x, (float)bitangent.y, (float)bitangent.z};
            b.tangent = a.tangent;
            b.bitangent = a.bitangent;
            c.tangent = a.tangent;
            c.bitangent = a.bitangent;
        }

        return true;
    }

    void Mesh::createMesh(Mesh &mesh, MeshData &data)
    {
        while (glGetError() != GL_NO_ERROR) { }

        glGenVertexArrays(1, &mesh.vertexArrayId);
        glBindVertexArray(mesh.vertexArrayId);

        glGenBuffers(1, &mesh.vertexBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), data.vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));
        glEnableVertexAttribArray(4);

        glBindVertexArray(0);

        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "! Failed to create mesh " + data.filename << std::endl;
            return;
        }

        mesh.vertexCount = data.vertices.size();
    }

    void Mesh::draw() const
    {
        glBindVertexArray(vertexArrayId);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertexCount));
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &vertexArrayId);
        glDeleteBuffers(1, &vertexBufferId);
    }
}
