#version 330 core

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

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TextureCoordinate;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec3 lightDirection = normalize(vec3(-0.4, 0.6, -1.0));

out VertexShaderOutput {
    vec2 TextureCoordinate;
    vec3 SunPosition;
} vertex;

void main()
{
    vec4 sun = ProjectionMatrix * ViewMatrix * vec4(-lightDirection * 100.0, 1.0);
    vertex.SunPosition = vec3(sun.xy / sun.w, sun.w);

    vertex.TextureCoordinate = TextureCoordinate;

    gl_Position = vec4(VertexPosition, 1.0);
}
