#version 330 core

uniform float earthAmbient;
uniform float earthDiffuse;
uniform float earthSpecular;
uniform float earthShininess;
uniform sampler2D earthTexture;

uniform vec4 sunColor;
uniform vec3 sunDirection;
uniform float sunAmbient;
uniform float sunDiffuse;
uniform float sunSpecular;

uniform vec3 cameraPosition;

in vec4 fsPosition;
in vec2 fsTextureCoords;
in vec3 fsNormal;

out vec4 outColor;

void main()
{
    vec3 sunDir = normalize(-sunDirection);
    vec3 viewDir = normalize(cameraPosition - fsPosition.xyz);

    // Ambient
    float ambient = sunAmbient * earthAmbient;

    // Diffuse
    float diffuse = max(dot(fsNormal, sunDir), 0.0) * sunDiffuse * earthDiffuse;

    // Specular
    vec3 reflectDir = reflect(-sunDir, fsNormal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), earthShininess) * sunSpecular * earthSpecular;

    outColor = (ambient + diffuse + specular) * sunColor * texture(earthTexture, fsTextureCoords);
}
