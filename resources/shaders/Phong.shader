#shader vertex
#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aVertexColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

uniform mat4 uProjection, uModelView, uNormalMat;

out vec3 vNormalInterp;
out vec3 vertPos;
out vec2 vTexCoord;
out vec4 vDiffuseColor;

void main() {
    vec4 vertPos4 = uModelView * vec4(aPosition, 1.0);
    vertPos = vec3(vertPos4) / vertPos4.w;
    vNormalInterp = vec3(uNormalMat * vec4(aNormal, 0.0));
    vTexCoord = aTexCoord;
    vDiffuseColor = aVertexColor;
    gl_Position = uProjection * vertPos4;
}

#shader fragment
#version 410 core

precision mediump float;
in vec3 vNormalInterp;
in vec3 vertPos;
in vec2 vTexCoord;
in vec4 vDiffuseColor;

uniform vec3 uLightPos;// Light position
uniform sampler2D uTexture;

out vec4 fragColor;

void main() {
    vec4 ambientColor = vec4(0.0, 0.0, 0.0, 1);
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 0.25);

    float Ka = 1.0;// Ambient reflection coefficient
    float Kd = 1.0;// Diffuse reflection coefficient
    float Ks = 0.5;// Specular reflection coefficient

    float shininessVal = 5;
    vec3 N = normalize(vNormalInterp);
    vec3 L = normalize(uLightPos - vertPos);

    // Lambert's cosine law
    float lambertian = max(dot(N, L), 0.0);
    float specular = 0.0;
    if (lambertian > 0.0) {
        vec3 R = reflect(-L, N);// Reflected light vector
        vec3 V = normalize(-vertPos);// Vector to viewer
        // Compute the specular term
        float specAngle = max(dot(R, V), 0.0);
        specular = pow(specAngle, shininessVal);
    }

        fragColor = Ka * ambientColor +
        Kd * lambertian * vDiffuseColor +
        Ks * specular * specularColor;

//        fragColor = Ka * ambientColor +
//        Kd * lambertian * texture(uTexture, vTexCoord) +
//        Ks * specular * specularColor;
}