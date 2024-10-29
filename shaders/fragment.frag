#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

in mat4 aModel;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(aModel)));
    vec3 fragNormal = normalize(normalMatrix * Normal);

    float specularStrength = 0.5;
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    FragColor = vec4((ambient + diffuse + specular), 1.0) * texture(ourTexture, TexCoord);
}