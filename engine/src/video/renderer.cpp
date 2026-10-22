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

#include "engine/src/video/renderer.h"

#include <GLFW/glfw3.h>
#include <glad.c>
#include <numbers>
#include <stdexcept>

using Engine::Core::ClearCommand;
using Engine::Core::RenderItem;

namespace Engine::Video
{
    Renderer::Renderer(const std::unique_ptr<CommandBuffer> &commandBuffer, Window &window, const Settings &settings)
        : commandBuffer(commandBuffer), window(window), fieldOfView(settings.fieldOfView)
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glfwSwapInterval(settings.vsync ? 1 : 0);

        if (settings.msaa) glEnable(GL_MULTISAMPLE);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
    }

    size_t Renderer::loadModel(const std::string &filename)
    {
        size_t modelId = nextModelId++;
        models[modelId] = Model::create(filename);

        return modelId;
    }

    void Renderer::unloadModel(size_t modelId)
    {
        if (!models.contains(modelId)) return;

        models.erase(modelId);
    }

    void Renderer::draw()
    {
        int width = 0;
        int height = 0;
        window.getFramebufferSize(width, height);

        if (width != viewportWidth || height != viewportHeight)
        {
            viewportWidth = width;
            viewportHeight = height;
            viewportAspect = height > 0 ? (double)width / (double)height : 1.0;
            glViewport(0, 0, width, height);
            projectionMatrix = Matrix::perspective(fieldOfView * std::numbers::pi / 180.0, viewportAspect, nearPlane, farPlane);
        }

        const ClearCommand &clear = commandBuffer->clear;

        glClearColor(clear.red, clear.green, clear.blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const RenderItem &item : commandBuffer->renderItems)
        {
            auto it = models.find(item.modelId);
            if (it == models.end()) continue;

            const auto &model = it->second;
            model->draw(item.transform, commandBuffer->viewMatrix, projectionMatrix);
        }
    }
}