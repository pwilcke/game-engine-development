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

#include "engine/src/core/application.h"

#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <utility>

namespace Engine::Core
{
    Application::Application(int argc, char *argv[])
    {
        settings = {
            .title = "Game Engine Development",
            .width = 1280,
            .height = 720,
            .fullscreen = false,
            .vsync = true,
            .msaa = true,
            .fieldOfView = 46.35
        };

        if (argc > 1) settings.assetRoot = argv[1];
    }

    std::unique_ptr<std::istream> Application::load(const std::string &filename, std::ios::openmode mode)
    {
        std::ios::openmode flags = std::ios::in;
        if (mode == std::ios::binary) flags |= std::ios::binary;

        auto file = std::make_unique<std::ifstream>(assetRoot + "/" + filename, flags);
        if (!file->is_open())
        {
            std::cout << "! Failed to open " + filename << std::endl;
            return nullptr;
        }

        return file;
    }

    bool Application::init()
    {
        assetRoot = settings.assetRoot;

        try
        {
            frontCommandBuffer = std::make_unique<CommandBuffer>();
            backCommandBuffer = std::make_unique<CommandBuffer>();

            window.emplace(settings);

            renderer.emplace(frontCommandBuffer, *window, settings);
            simulation.emplace(backCommandBuffer, *renderer);
        }
        catch (const std::exception &e)
        {
            std::cerr << "! " << e.what() << std::endl;
            return false;
        }

        return true;
    }

    bool Application::run()
    {
        try
        {
            std::chrono::steady_clock::time_point lastFrame = std::chrono::steady_clock::now();
            double fpsTime = 0.0;
            int fpsFrames = 0;

            while (window->processEvents())
            {
                std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
                std::chrono::duration<double> frameTime = now - lastFrame;
                lastFrame = now;

                fpsTime += frameTime.count();
                fpsFrames++;
                if (fpsTime >= 1.0)
                {
                    std::cout << "  FPS: " << fpsFrames << std::endl;
                    fpsTime = 0.0;
                    fpsFrames = 0;
                }

                std::chrono::duration<double> utc = std::chrono::system_clock::now().time_since_epoch();
                double t = utc.count();

                double dt = frameTime.count();

                simulation->update(dt, t);
                std::swap(frontCommandBuffer, backCommandBuffer);
                renderer->draw();
                window->present();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "! " << e.what() << std::endl;
            return false;
        }

        return true;
    }
}
