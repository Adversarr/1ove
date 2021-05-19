#version 330

in vec4 vertexColor;
in vec4 ourColor;

out vec4 FragColor;

void main()
{
    FragColor = vertexColor;
}