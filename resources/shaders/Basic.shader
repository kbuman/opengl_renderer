#shader vertex
#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 aNormal;

out vec2 v_TexCoord;
out vec3 currentPosition;
out vec3 normal;
out vec4 pixelColor;

uniform mat4 camera;
uniform mat4 model;

void main()
{
   currentPosition = vec3(model * position);
   gl_Position = camera*vec4(currentPosition,1.0f);
   v_TexCoord = texCoord;
   normal = normalize(vec3(model*vec4(aNormal,1.0f)));
   pixelColor = vertexColor;
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 currentPosition;
in vec3 normal;
in vec4 pixelColor;

uniform vec4 u_color;
uniform sampler2D diffuseTexture;
uniform vec3 lightPosition;
uniform vec4 lightColor;
uniform vec3 cameraPosition;

void main()
{
   float ambient = 0.2f;
   vec3 Normal = normalize(normal);
   vec3 lightDirection = normalize(lightPosition - currentPosition);

   float diffuse = max(dot(Normal, lightDirection),0.0f);

   float specularLight = 1.0f;

   vec3 viewDirection = normalize(cameraPosition - currentPosition);
   vec3 reflectionDirection = reflect(-lightDirection,Normal);
   float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 50);
   float specular = specAmount * specularLight;

   vec4 texColor = pixelColor * lightColor * (diffuse+ambient+specular);
//    vec4 texColor = texture(diffuseTexture, v_TexCoord) * lightColor * (diffuse+ambient+specular);
   FragColor = texColor;
};