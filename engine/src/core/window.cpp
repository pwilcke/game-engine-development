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

#include "engine/src/core/window.h"

#include "engine/src/core/inputhandler.h"

#include <iostream>
#include <stdexcept>

namespace Engine::Core
{
    Window::Window(const Settings &settings)
    {
        glfwSetErrorCallback([](int error, const char *description)
        {
            std::cerr << "! GLFW Error: " << description << std::endl;
        });

        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (settings.msaa) glfwWindowHint(GLFW_SAMPLES, 4);

        if (settings.fullscreen)
        {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            window = glfwCreateWindow(mode->width, mode->height, settings.title.c_str(), monitor, nullptr);
        }
        else
        {
            window = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
        }

        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to open window");
        }

        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, this);

        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_REPEAT) return;
            InputHandler::keyCallback(key, scancode, mods, action == GLFW_PRESS);
        });

        InputHandler::onKeyPress(GLFW_KEY_ESCAPE, [this]()
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        });
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool Window::processEvents()
    {
        glfwPollEvents();
        return !glfwWindowShouldClose(window);
    }

    void Window::present()
    {
        glfwSwapBuffers(window);
    }

    void Window::getFramebufferSize(int &width, int &height) const
    {
        glfwGetFramebufferSize(window, &width, &height);
    }
}