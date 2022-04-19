#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;

void main()
{

    ivec2 texture_size = textureSize(screenTexture, 0);
    vec2 step_size = 1.0/texture_size;

    vec2 offsets[9] = vec2[](
    vec2(-step_size.x, step_size.y), // top-left
    vec2(0.0f, step_size.y), // top-center
    vec2(step_size.x, step_size.y), // top-right
    vec2(-step_size.x, 0.0f), // center-left
    vec2(0.0f, 0.0f), // center-center
    vec2(step_size.x, 0.0f), // center-right
    vec2(-step_size.x, -step_size.y), // bottom-left
    vec2(0.0f, -step_size.y), // bottom-center
    vec2(step_size.x, -step_size.y)// bottom-right
    );

    float kernel[9] = float[](
    1, 1, 1,
    1, -8, 1,
    1, 1, 1
    );

    vec3 color = vec3(0.0);
    for (int i = 0; i < 9; i++)
    {
        color += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
    }
    FragColor = vec4(color, 1.0);
}