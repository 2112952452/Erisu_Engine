#version 330 core

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 mainLightColor;
uniform vec3 mainLightDir;

uniform sampler2D texture_diffuse0;


void main()
{
    vec4 texColor = texture(texture_diffuse0, TexCoord);
    float diffToon = (smoothstep(0.48, 0.5, dot(Normal, mainLightDir) * 0.5 + 0.5) * 0.5 + 0.5);
    vec3 diffuseColor = texColor.rgb * mainLightColor * diffToon;
    // gamma correction
    diffuseColor = pow(diffuseColor, vec3(1.f / 2.2f));

    FragColor = vec4(diffuseColor, 1.0);
}