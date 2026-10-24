/**
 * Game Engine Development
 * Technische Hochschule Mittelhessen
 * Copyright © 2021-2026 Tobias Reimann
 * Copyright © 2026 Paul Wilcke
 * Copyright © 2026 Berkay Özgür
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

#include "app/src/mygame.h"

#include "app/src/shipscript.h"
#include "engine/src/core/inputhandler.h"
#include "engine/src/math/vector3.h"

#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>

using Engine::Core::InputHandler;
using Engine::Math::Vector3;

namespace App
{
    MyGame::MyGame(int argc, char *argv[])
        : Application(argc, argv)
    {
        settings.title = "My Game";
    }

    bool MyGame::init()
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        bool ok = Application::init();
        if (!ok) return false;

        ship = simulation->loadEntity("entities/ship.entity");
        if (!ship) return false;

        ship->setPosition(Vector3(0.0, 0.0, 0.0));
        ship->addScript(std::make_unique<ShipScript>());

        // Add key mappings
        InputHandler::registerKeyAction("reset", GLFW_KEY_SPACE);
        InputHandler::onAction("reset", [this]()
        {
            ship->setRotation({});
        });

        // Register keys for for yaw pitch and roll
        InputHandler::registerKeyAxis("yaw", GLFW_KEY_E, GLFW_KEY_Q);
        InputHandler::registerKeyAxis("pitch", GLFW_KEY_S, GLFW_KEY_W);
        InputHandler::registerKeyAxis("pitch", GLFW_KEY_DOWN, GLFW_KEY_UP);
        InputHandler::registerKeyAxis("roll", GLFW_KEY_A, GLFW_KEY_D);
        InputHandler::registerKeyAxis("roll", GLFW_KEY_LEFT, GLFW_KEY_RIGHT);

        // Register gamepad keys
        InputHandler::registerButtonAction("reset", GLFW_GAMEPAD_BUTTON_A);

        InputHandler::registerStickAxis("yaw", GLFW_GAMEPAD_AXIS_RIGHT_X, true);
        InputHandler::registerStickAxis("pitch", GLFW_GAMEPAD_AXIS_LEFT_Y, true);
        InputHandler::registerStickAxis("roll", GLFW_GAMEPAD_AXIS_LEFT_X);

        for (const Vector3 &position : {Vector3::forward * 25.0, Vector3::forward * -25.0, Vector3::left * 25.0, Vector3::left * -25.0, Vector3::up * 25.0, Vector3::up * -25.0})
        {
            if (auto cube = simulation->loadEntity("entities/cuberef.entity"))
            {
                cube->setPosition(position);
            }
        }

        std::chrono::duration<double> startupTime = std::chrono::steady_clock::now() - start;
        std::cout << "+ Startup time: " << startupTime.count() << " s" << std::endl;

        return true;
    }
}