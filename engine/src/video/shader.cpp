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

#include "engine/src/video/shader.h"

#include "engine/src/core/application.h"

#include <glad/glad.h>
#include <iostream>

using Engine::Core::Application;
using Engine::Math::Matrixf;

namespace Engine::Video
{
    std::shared_ptr<Shader> Shader::create(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
    {
        auto shader = std::shared_ptr<Shader>(new Shader());
        ShaderData vertexShaderData = { GL_VERTEX_SHADER, vertexShaderFile };
        ShaderData fragmentShaderData = { GL_FRAGMENT_SHADER, fragmentShaderFile };

        bool success = readFile(vertexShaderData) && readFile(fragmentShaderData);
        if (!success) return shader;

        success = compileShader(vertexShaderData) && compileShader(fragmentShaderData);
        if (!success) return shader;

        linkProgram(*shader, vertexShaderData, fragmentShaderData);
        return shader;
    }

    bool Shader::readFile(ShaderData &data)
    {
        auto file = Application::load(data.filename);
        if (!file) return false;

        std::string line;
        while (getline(*file, line))
        {
            data.source += (line + "\n");
        }

        return true;
    }

    bool Shader::compileShader(ShaderData &data)
    {
        char *shaderCodeChar = data.source.data();

        int success = 0;
        char infoLog[512];

        data.shaderId = glCreateShader(data.type);
        glShaderSource(data.shaderId, 1, &shaderCodeChar, nullptr);
        glCompileShader(data.shaderId);
        glGetShaderiv(data.shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(data.shaderId, 512, nullptr, infoLog);
            std::cout << "! Failed to compile shader " + data.filename + ":\n" + infoLog << std::endl;
            return false;
        }

        return true;
    }

    void Shader::linkProgram(Shader &shader, ShaderData &vertexShaderData, ShaderData &fragmentShaderData)
    {
        int success = 0;
        char infoLog[512];

        uint32_t programId = glCreateProgram();
        glAttachShader(programId, vertexShaderData.shaderId);
        glAttachShader(programId, fragmentShaderData.shaderId);
        glLinkProgram(programId);
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId, 512, nullptr, infoLog);
            std::cout << "! Failed to link shader program " + vertexShaderData.filename + " + " + fragmentShaderData.filename + ":\n" + infoLog << std::endl;
            programId = 0;
        }

        glDeleteShader(vertexShaderData.shaderId);
        glDeleteShader(fragmentShaderData.shaderId);

        shader.programId = programId;
    }

    int Shader::getLocation(const std::string &name)
    {
        return glGetUniformLocation(programId, name.c_str());
    }

    bool Shader::hasUniform(const std::string &name)
    {
        return getLocation(name) >= 0;
    }

    void Shader::setInt(const std::string &name, int value)
    {
        int location = getLocation(name);
        if (location < 0) return;

        glUniform1i(location, value);
    }

    void Shader::setMatrix(const std::string &name, const Matrix &value)
    {
        int location = getLocation(name);
        if (location < 0) return;

        Matrixf matrix = value;
        glUniformMatrix4fv(location, 1, GL_FALSE, (float *)&matrix);
    }

    Shader::~Shader()
    {
        glDeleteProgram(programId);
    }
}