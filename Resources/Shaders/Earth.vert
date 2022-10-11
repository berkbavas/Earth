#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

uniform mat3 N; // Normal matrix
uniform mat4 M; // Model matrix
uniform mat4 VP; // View projection matrix

out vec4 fsPosition;
out vec2 fsTextureCoords;
out vec3 fsNormal;

void main()
{
    fsPosition = M * position;
    fsNormal = N * normal;
    fsTextureCoords = textureCoords;

    gl_Position = VP * fsPosition;
}
