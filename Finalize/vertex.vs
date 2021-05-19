#version 330
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;
out vec3 normal;
out vec3 fragPosition;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vertexColor = vec4(1.f, 0.1f, 0.1f, 1.0f);
    normal = aNormal;
    fragPosition = vec3(model * vec4(aPos, 1.0f));
}