#version 330 core

in vec3 pos;
in vec2 tex;

out vec2 UVs;

uniform mat4 MVP;

void main(){
	gl_Position = MVP * vec4(pos, 1);
	UVs = tex;
}