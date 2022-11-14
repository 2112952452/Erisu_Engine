//
// Created by Adarion on 2022/11/4.
//


#include <string>
#include <mutex>
#include "UIShader.h"


namespace
{
    constexpr std::string_view UIDefaultShader = R"(#version 330 core

layout (location = 0) in vec4 aPos; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_Model * vec4(aPos.xy, 0.0, 1.0);
    TexCoords = aPos.zw;
}
)";

    constexpr std::string_view UIDefaultFragmentShader = R"(#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
void main()
{
    FragColor = texture(u_Texture, TexCoords) * u_Color;
}
)";

    constexpr std::string_view defaultFontVertexShader = R"(
#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

    constexpr std::string_view defaultFontFragmentShader = R"(
#version 330 core

out vec4 color;
in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
    float alpha = texture(u_Texture, TexCoords).r;
    color = vec4(u_Color.rgb, alpha * u_Color.a);
}
)";

    constexpr std::string_view defaultOutlineTextVertexShader = R"(
#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_Proj;

void main()
{
    gl_Position = u_Proj * u_Model * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

    constexpr std::string_view defaultOutlineTextFragmentShader = R"(
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform vec4 u_OutlineColor;
uniform float u_OutlineRadius;

void main()
{
    float alpha = texture(u_Texture, TexCoords).r;
    vec2 texelSize = vec2(1.0) / textureSize(u_Texture, 0);
    float offsetRadius = u_OutlineRadius;
    const vec2 offset[8] = vec2[](
            vec2(1.0, 0.0),
            vec2(0.707106, 0.707106),
            vec2(0.0, 1.0),
            vec2(-0.707106, 0.707106),
            vec2(-1.0, 0.0),
            vec2(-0.707106, -0.707106),
            vec2(0.0, -1.0),
            vec2(0.707106, -0.707106)
    );

    float alphaDiff = 0.0;
    for (int i = 0; i < 8; i++)
    {
        float sample = texture(u_Texture, TexCoords + offset[i] * texelSize * offsetRadius).r;
        alphaDiff += sample - alpha;
    }

    alphaDiff = smoothstep(0, 0.1, alphaDiff * 0.125);

    FragColor = mix(vec4(u_Color.rgb, alpha), u_OutlineColor, alphaDiff);
}
)";

}

namespace Erisu::Function
{
    std::shared_ptr<GLShader> DefaultUIShader()
    {
        return std::make_shared<GLShader>(UIDefaultShader.data(), UIDefaultFragmentShader.data(), true);
    }

    std::shared_ptr<GLShader> DefaultFontShader()
    {
        return std::make_shared<GLShader>(defaultFontVertexShader.data(), defaultFontFragmentShader.data(), true);
    }

    std::pair<unsigned, unsigned> DefaultVAOVBO()
    {
        static constexpr float vertex[] =
                {
                        // pos      // tex
                        -0.5f, -0.5f, 0.0f, 0.0f,
                        0.5f, -0.5f, 1.0f, 0.0f,
                        0.5f, 0.5f, 1.0f, 1.0f,

                        0.5f, 0.5f, 1.0f, 1.0f,
                        -0.5f, 0.5f, 0.0f, 1.0f,
                        -0.5f, -0.5f, 0.0f, 0.0f
                };

        unsigned int VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return std::make_pair(VAO, VBO);
    }

    std::shared_ptr<GLShader> DefaultUIOutlineTextShader()
    {
        return std::make_shared<GLShader>(defaultOutlineTextVertexShader.data(), defaultOutlineTextFragmentShader.data(), true);
    }

}
