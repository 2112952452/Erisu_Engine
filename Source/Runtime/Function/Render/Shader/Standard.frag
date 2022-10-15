#version 330

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    float shininess;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

uniform Material material;

uniform sampler2D texture_diffuse0;

uniform vec3 mainLightColor;
uniform vec3 mainLightDir;
uniform vec3 viewPos;

void main()
{
    vec4 texColor = texture(texture_diffuse0, TexCoord);
    vec3 normal = normalize(Normal);

    // Lambert
    float lambert = dot(normal, mainLightDir) * 0.5 + 0.5;
    vec3 diffuse = mainLightColor * texColor.rgb * lambert * material.diffuse;
    // gamma correction
    diffuse = pow(diffuse, vec3(1.0f / 2.2f));

    // Specular half Vector
    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 halfDir = normalize(mainLightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);
    vec3 specular = material.shininess <= 0.0 ? vec3(0) : mainLightColor * spec * material.specular;

    FragColor = vec4(diffuse + specular + material.emissive, texColor.a);
}