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

out vec4 FragColor;

in VertexShaderOutput {
    vec3 FragmentPosition;
    vec2 TextureCoordinate;
    vec3 Normal;
    vec3 CameraPosition;
};

uniform sampler2D AAA;
uniform sampler2D R;

uniform vec3 lightDirection = normalize(vec3(-0.4, 0.6, -1.0));
uniform vec3 lightColor = vec3(1.0);
uniform float ambient = 0.25;

uniform float opacity = 1.0;

const float pi = 3.14159265;

void main()
{
    vec3 albedo = pow(texture(AAA, TextureCoordinate).rgb, vec3(2.2));

    float roughness = max(texture(R, TextureCoordinate).r, 0.05);

    vec3 N = normalize(Normal);

    vec3 V = normalize(CameraPosition - FragmentPosition);
    vec3 L = -lightDirection;
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = clamp(dot(V, H), 0.0, 1.0);

    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float denominator = NdotH * NdotH * (alpha2 - 1.0) + 1.0;
    float D = alpha2 / (pi * denominator * denominator);
    float k = alpha / 2.0;
    float G = (NdotV / (NdotV * (1.0 - k) + k)) * (NdotL / (NdotL * (1.0 - k) + k));
    float F = 0.04 + 0.96 * pow(1.0 - VdotH, 5.0);

    float specular = D * G * F / max(4.0 * NdotV * NdotL, 0.000001);
    vec3 diffuse = (1.0 - F) * albedo / pi;

    vec3 direct = (diffuse + specular) * pi * NdotL;
    vec3 indirect = albedo * ambient;
    vec3 color = (direct + indirect) * lightColor;

    FragColor = vec4(pow(color, vec3(1.0 / 2.2)), opacity);
}
