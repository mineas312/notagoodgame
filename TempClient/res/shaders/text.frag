#version 460 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
layout(location = 3) uniform vec3 textColor;

void main()
{
    color = vec4(textColor, texture(text, TexCoords).r);
}  