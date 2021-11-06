#version 330 core

in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D texture2;
void main()
{
    FragColor = vec4(texture(texture2, TexCoords));
}