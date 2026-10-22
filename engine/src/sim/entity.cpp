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

#include "engine/src/sim/entity.h"

#include "engine/src/core/application.h"

#include <iostream>
#include <json.hpp>

using Engine::Core::Application;
using Engine::Core::RenderItem;
using Json = nlohmann::json;

namespace Engine::Sim
{
    bool Entity::load(const std::string &filename, Renderer &renderer)
    {
        auto file = Application::load(filename);
        if (!file) return false;

        Json config = Json::parse(*file, nullptr, false);

        if (config.is_discarded())
        {
            std::cout << "! Failed to parse entity " + filename << std::endl;
            return false;
        }

        if (!config.contains("models") || !config["models"].is_array())
        {
            std::cout << "! Failed to read entity " + filename + ": no models" << std::endl;
            return false;
        }

        for (const auto &entry : config["models"])
        {
            std::string model = {};
            Vector3 scale = {1.0, 1.0, 1.0};

            if (entry.is_string())
            {
                model = entry.get<std::string>();
            }
            else if (entry.is_object() && entry.contains("model") && entry["model"].is_string())
            {
                model = entry["model"].get<std::string>();

                if (entry.contains("scale") && entry["scale"].is_array() && entry["scale"].size() == 3)
                {
                    scale = {entry["scale"][0].get<double>(), entry["scale"][1].get<double>(), entry["scale"][2].get<double>()};
                }
            }

            if (model.empty()) continue;

            ModelInstance instance = {
                .modelId = renderer.loadModel(model),
                .transform = Matrix::scale(scale)
            };
            models.push_back(instance);
        }

        if (models.empty())
        {
            std::cout << "! Failed to read entity " + filename + ": no usable model" << std::endl;
            return false;
        }

        return true;
    }

    void Entity::unload(Renderer &renderer)
    {
        for (const std::unique_ptr<Script> &script : scripts)
        {
            script->unload(*this);
        }

        for (const ModelInstance &model : models)
        {
            renderer.unloadModel(model.modelId);
        }
    }

    void Entity::update(double dt, double t)
    {
        for (const std::unique_ptr<Script> &script : scripts)
        {
            script->update(*this, dt, t);
        }
    }

    void Entity::commit(CommandBuffer &commandBuffer) const
    {
        Matrix entityTransform = Matrix::translation(position) * rotation.toMatrix() * Matrix::scale(scale);

        for (const ModelInstance &model : models)
        {
            RenderItem item = {
                .modelId = model.modelId,
                .transform = entityTransform * model.transform
            };
            commandBuffer.renderItems.push_back(item);
        }
    }
}
