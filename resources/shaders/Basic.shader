#shader vertex
#version 410 core

layout(location = 0) in vec4 position;
// layout(location = 1) in vec2 texCoord;
layout(location = 1) in vec3 vertexColor;

out vec2 v_TexCoord;
out vec4 pixelColor;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP*position;
   // v_TexCoord = texCoord;
   pixelColor = vec4(vertexColor, 1.0);
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 pixelColor;

uniform vec4 u_color;
// uniform sampler2D u_Texture;

void main()
{
   // vec4 texColor = texture(u_Texture, v_TexCoord);
   color = pixelColor;
};