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

#include "engine/src/video/model.h"

#include "engine/src/core/application.h"

#include <glad/glad.h>
#include <iostream>
#include <json.hpp>

using Engine::Core::Application;
using Json = nlohmann::json;

namespace Engine::Video
{
    std::shared_ptr<Model> Model::create(const std::string &filename)
    {
        auto model = std::shared_ptr<Model>(new Model());
        load(*model, filename);
        return model;
    }

    void Model::load(Model &model, const std::string &filename)
    {
        auto file = Application::load(filename);
        if (!file) return;

        Json config = Json::parse(*file, nullptr, false);

        if (config.is_discarded())
        {
            std::cout << "! Failed to parse model " + filename << std::endl;
            return;
        }

        if (config.contains("meshes") && config["meshes"].is_array())
        {
            for (const auto &mesh : config["meshes"])
            {
                if (mesh.is_string()) model.meshes.push_back(Mesh::create(mesh.get<std::string>()));
            }
        }
        else if (config.contains("mesh") && config["mesh"].is_string())
        {
            model.meshes.push_back(Mesh::create(config["mesh"].get<std::string>()));
        }

        if (model.meshes.empty())
        {
            std::cout << "! Failed to read model " + filename + ": no mesh" << std::endl;
            return;
        }

        if (!config.contains("shaders") || !config["shaders"].is_object() || !config["shaders"].contains("vertex") || !config["shaders"].contains("fragment"))
        {
            std::cout << "! Failed to read model " + filename + ": no shader" << std::endl;
            return;
        }
        model.shader = Shader::create(config["shaders"]["vertex"].get<std::string>(), config["shaders"]["fragment"].get<std::string>());

        if (config.contains("textures") && config["textures"].is_object())
        {
            for (const auto &[uniformName, texture] : config["textures"].items())
            {
                if (texture.is_string()) model.textures[uniformName] = Texture::create(texture.get<std::string>());
            }
        }
    }

    void Model::draw(const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projectionMatrix) const
    {
        if (!shader) return;

        glUseProgram(shader->getId());

        shader->setMatrix("WorldMatrix", worldMatrix);
        shader->setMatrix("ViewMatrix", viewMatrix);
        shader->setMatrix("ProjectionMatrix", projectionMatrix);

        int unit = 0;
        for (const auto &[uniformName, texture] : textures)
        {
            if (!shader->hasUniform(uniformName)) continue;

            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, texture->getId());
            shader->setInt(uniformName, unit);
            unit++;
        }

        for (const auto &mesh : meshes)
        {
            mesh->draw();
        }
    }
}
