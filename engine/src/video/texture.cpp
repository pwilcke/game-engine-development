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

#define STB_IMAGE_IMPLEMENTATION

#include "engine/src/video/texture.h"

#include "engine/src/core/application.h"

#include <glad/glad.h>
#include <iostream>
#include <iterator>
#include <stb_image.h>
#include <vector>

using Engine::Core::Application;

namespace Engine::Video
{
    std::shared_ptr<Texture> Texture::create(const std::string &filename)
    {
        auto texture = std::shared_ptr<Texture>(new Texture());
        TextureData textureData = { filename };

        bool success = readFile(textureData);
        if (!success) return texture;

        genTexture(*texture, textureData);
        return texture;
    }

    bool Texture::readFile(TextureData &data)
    {
        auto file = Application::load(data.filename, std::ios::binary);
        if (!file) return false;

        std::vector<uint8_t> bytes{std::istreambuf_iterator<char>(*file), std::istreambuf_iterator<char>()};

        data.content = stbi_load_from_memory(bytes.data(), static_cast<int>(bytes.size()), &data.width, &data.height, &data.channels, 0);
        if (!data.content || data.channels == 0 || data.width == 0 || data.height == 0)
        {
            std::cout << "! Failed to decode texture " + data.filename << std::endl;
            stbi_image_free(data.content);
            return false;
        }

        return true;
    }

    void Texture::genTexture(Texture &texture, const TextureData &data)
    {
        while (glGetError() != GL_NO_ERROR) { }

        uint32_t textureId = 0;

        glGenTextures(1, &textureId);
        glActiveTexture(GL_TEXTURE15);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data.channels == 1)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, data.width, data.height, 0, GL_RED, GL_UNSIGNED_BYTE, data.content);
        }
        else if (data.channels == 2)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, data.width, data.height, 0, GL_RG, GL_UNSIGNED_BYTE, data.content);
        }
        else if (data.channels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.width, data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.content);
        }
        else if (data.channels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.content);
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data.content);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (glGetError() != GL_NO_ERROR)
        {
            std::cout << "! Failed to generate texture " + data.filename << std::endl;
            glDeleteTextures(1, &textureId);
            textureId = 0;
        }

        texture.textureId = textureId;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &textureId);
    }
}