#version 460 core

struct InternalMatrix {
	vec4 C1;
	vec4 C2;
	vec4 C3;
	vec4 C4;
};

layout(location = 0) in vec3 vec2Dpos;
layout(location = 1) in vec2 texcoord;

layout (std140, binding = 0) uniform MVPMatrixInst{
	InternalMatrix matrix[1025];
};

out vec2 Texcoord;

void main() {
	mat4 MVP;
	MVP[0] = matrix[gl_InstanceID].C1;
	MVP[1] = matrix[gl_InstanceID].C2;
	MVP[2] = matrix[gl_InstanceID].C3;
	MVP[3] = matrix[gl_InstanceID].C4;
	
	gl_Position = MVP*vec4( vec2Dpos, 1 );
	Texcoord = texcoord;
}