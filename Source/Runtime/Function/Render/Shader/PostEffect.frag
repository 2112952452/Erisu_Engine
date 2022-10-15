#version 330

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ScreenTexture;


void main() {
    vec4 color = texture(ScreenTexture, TexCoord);
    vec2 texelSize = 1.0 / textureSize(ScreenTexture, 0);

    // Calculate the gradient using Sobel operator
    float gx = texture(ScreenTexture, TexCoord + vec2(texelSize.x, 0.0)).r - texture(ScreenTexture, TexCoord - vec2(texelSize.x, 0.0)).r;
    float gy = texture(ScreenTexture, TexCoord + vec2(0.0, texelSize.y)).r - texture(ScreenTexture, TexCoord - vec2(0.0, texelSize.y)).r;
    float gradient = sqrt(gx * gx + gy * gy);

    // Calculate the edge threshold
    float threshold = 0.1;
    float edge = step(threshold, gradient);

    vec3 col = color.rgb * (1 - edge);
    // Calculate the final color
    //FragColor = vec4(col, 1.0);

    FragColor = color;
}