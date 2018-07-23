#version 460 core

in vec2 Texcoord;

out vec4 color;

uniform sampler2D sampler;

void main() {
	color = texture(sampler, Texcoord);
}