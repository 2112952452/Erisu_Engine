//
// Created by Adarion on 2022/10/24.
//

#include "GalTextRenderer.h"
#include "Global/Global.h"
#include <utility>

namespace
{
    constexpr std::string_view galFontVertexShader = R"(#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

    constexpr std::string_view galFontFragmentShader = R"(#version 330 core

out vec4 color;
in vec2 TexCoords;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_ShowProgress;

void main()
{
    float alpha = texture(u_Texture, TexCoords).r;
    float progress = smoothstep(1.001, u_ShowProgress, TexCoords.x);
    color = vec4(u_Color.rgb, alpha * progress);
}
)";


}


namespace Erisu::Function
{

    GalTextRenderer::GalTextRenderer(std::string name, std::string fontPath, std::string text, int fontSize,
                                     Eigen::Vector4f color, int priority) : TextRenderer(std::move(name), std::move(text),
                                                                                         galFontVertexShader.data(),
                                                                                         galFontFragmentShader.data(),
                                                                                         std::move(fontPath),
                                                                                         fontSize, priority)
    {
        color_ = std::move(color);

        onUpdateUniform_ = [this](const std::shared_ptr<GLShader>& shader, Character* character)
        {
            float delta = 1.f / wText_.size(), sp = 0;
            if (delta * character->index_ < showProgress_)
                sp = std::clamp(showProgress_ - delta * character->index_, 0.f, delta) / delta;

            shader->SetFloat("u_ShowProgress", sp);
        };
    }

}