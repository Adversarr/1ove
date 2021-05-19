#version 330

in vec4 vertexColor;
in vec3 normal;
in vec3 fragPosition;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 ambient = 0.25f * lightColor;
    vec3 objColor = vec3(vertexColor.x, vertexColor.y, vertexColor.z);
    vec3 lightDirection = normalize(lightPos - fragPosition);
    vec3 norm = normalize(normal);
    vec3 reflectDir = reflect(-lightDirection, norm);
    vec3 viewDir = normalize(viewPos - fragPosition);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = .35f * spec * lightColor;
    
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (ambient + diffuse + specular)* objColor;
    
    FragColor = vec4(result, 1.0f);
}