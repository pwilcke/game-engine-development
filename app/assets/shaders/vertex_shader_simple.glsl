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
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out VertexShaderOutput {
    vec3 FragmentPosition;
    vec2 TextureCoordinate;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;
} vertex;

void main()
{
    vertex.FragmentPosition = vec3(WorldMatrix * vec4(VertexPosition, 1.0));

    vertex.TextureCoordinate = vec2(TextureCoordinate.x, 1.0 - TextureCoordinate.y);

    vertex.Normal = mat3(WorldMatrix) * Normal;
    vertex.Tangent = mat3(WorldMatrix) * Tangent;
    vertex.Bitangent = mat3(WorldMatrix) * Bitangent;

    vertex.CameraPosition = inverse(ViewMatrix)[3].xyz;

    gl_Position = ProjectionMatrix * ViewMatrix * WorldMatrix * vec4(VertexPosition, 1.0);
}
