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
 *
 * The lens flare is derived from a Shadertoy project by mu6k, available at
 * https://www.shadertoy.com/view/4sX3Rs and released into the public domain:
 * https://unlicense.org/
 */

out vec4 FragColor;

in VertexShaderOutput {
    vec2 TextureCoordinate;
    vec3 SunPosition;
};

uniform sampler2D Noise;

uniform float SunBrightness;
uniform float ViewportAspect;

uniform vec3 lightColor = vec3(1.0);

float noise(float t)
{
    return texture(Noise, vec2(t / 256, 0)).x;
}

float noise(vec2 t)
{
    return texture(Noise, vec2(t.x / 256, t.y / 256)).x;
}

vec3 lensflare(vec2 uv, vec2 pos)
{
    vec2 main = uv - pos;
    vec2 uvd = uv * (length(uv));

    float ang = atan(main.x, main.y);
    float dist = length(main);
    dist = pow(dist, .1);

    float f0 = 1.0 / (length(uv - pos) * 16.0 + 1.0);

    f0 = f0 + f0 * (sin(noise(sin(ang * 2. + pos.x) * 4.0 - cos(ang * 3. + pos.y)) * 16.) * .1 + dist * .1 + .8);

    float f2 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.8 * pos), 2.0)), .0) * 00.25;
    float f22 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.85 * pos), 2.0)), .0) * 00.23;
    float f23 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.9 * pos), 2.0)), .0) * 00.21;

    vec2 uvx = mix(uv, uvd, -0.5);

    float f4 = max(0.01 - pow(length(uvx + 0.4 * pos), 2.4), .0) * 6.0;
    float f42 = max(0.01 - pow(length(uvx + 0.45 * pos), 2.4), .0) * 5.0;
    float f43 = max(0.01 - pow(length(uvx + 0.5 * pos), 2.4), .0) * 3.0;

    uvx = mix(uv, uvd, -.4);

    float f5 = max(0.01 - pow(length(uvx + 0.2 * pos), 5.5), .0) * 2.0;
    float f52 = max(0.01 - pow(length(uvx + 0.4 * pos), 5.5), .0) * 2.0;
    float f53 = max(0.01 - pow(length(uvx + 0.6 * pos), 5.5), .0) * 2.0;

    uvx = mix(uv, uvd, -0.5);

    float f6 = max(0.01 - pow(length(uvx - 0.3 * pos), 1.6), .0) * 6.0;
    float f62 = max(0.01 - pow(length(uvx - 0.325 * pos), 1.6), .0) * 3.0;
    float f63 = max(0.01 - pow(length(uvx - 0.35 * pos), 1.6), .0) * 5.0;

    vec3 c = vec3(.0);

    c.r += f2 + f4 + f5 + f6;
    c.g += f22 + f42 + f52 + f62;
    c.b += f23 + f43 + f53 + f63;
    c = c * 1.3 - vec3(length(uvd) * .05);
    c += vec3(f0);

    return c;
}

vec3 cc(vec3 color, float factor, float factor2)
{
    float w = color.x + color.y + color.z;
    return mix(color, vec3(w) * factor, w * factor2);
}

void main()
{
    if (SunPosition.z <= 0.0)
    {
        FragColor = vec4(0.0);
        return;
    }

    vec2 uv = TextureCoordinate - 0.5;
    uv.x *= ViewportAspect;

    vec2 sun = SunPosition.xy * 0.5;
    sun.x *= ViewportAspect;

    vec3 color = normalize(lightColor) * lensflare(uv, sun);
    color -= noise(TextureCoordinate / 0.01) * 0.015;
    color = cc(color, 0.5, 0.1);
    color *= SunBrightness;

    FragColor = vec4(pow(clamp(color, 0.0, 1.0), vec3(1.0 / 2.2)), 1.0);
}
