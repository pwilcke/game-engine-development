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

#include <cstdint>
#include <memory>
#include <string>

namespace Engine::Video
{
    struct TextureData
    {
        std::string filename;
        int width = 0;
        int height = 0;
        int channels = 0;
        unsigned char *content = nullptr;
    };

    class Texture
    {
      public:
        static std::shared_ptr<Texture> create(const std::string &filename);
        ~Texture();

        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;

        static bool readFile(TextureData &info);
        static void genTexture(Texture &texture, const TextureData &info);
        uint32_t getId() const { return textureId; };
        
      private:
        Texture() = default;
        uint32_t textureId = 0;
    };
}