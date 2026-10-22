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

#pragma once

#include "engine/src/math/matrix.h"

#include <cstdint>
#include <memory>
#include <string>

using Engine::Math::Matrix;

namespace Engine::Video
{
    struct ShaderData
    {
        uint32_t type;
        std::string filename;
        std::string source;
        uint32_t shaderId;
    };

    class Shader
    {
      public:
        static std::shared_ptr<Shader> create(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
        ~Shader();

        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;

        static bool readFile(ShaderData &data);
        static bool compileShader(ShaderData &data);
        static void linkProgram(Shader &shader, ShaderData &vertexShaderData, ShaderData &fragmentShaderData);
        uint32_t getId() const { return programId; };

        bool hasUniform(const std::string &name);

        void setInt(const std::string &name, int value);
        void setMatrix(const std::string &name, const Matrix &value);

      private:
        Shader() = default;

        int getLocation(const std::string &name);

        uint32_t programId = 0;
    };
}