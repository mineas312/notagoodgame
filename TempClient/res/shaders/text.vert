#version 460 core

struct InternalMatrix {
	vec4 C1;
	vec4 C2;
	vec4 C3;
	vec4 C4;
};

struct AtlasLookup {
    vec2 PIOffset[4];
};

layout (location = 0) in vec3 vec2DPos;

layout (std140, binding = 0) uniform MVPMatrix {
    InternalMatrix matrix[1024];
};

layout (std140, binding = 1) uniform AtlasValues {
    AtlasLookup lookup[1024];
};

out vec2 texOffset;

void main(){
    mat4 MVP;
	MVP[0] = matrix[gl_InstanceID].C1;
	MVP[1] = matrix[gl_InstanceID].C2;
	MVP[2] = matrix[gl_InstanceID].C3;
	MVP[3] = matrix[gl_InstanceID].C4;

    gl_Position = MVP * vec4(vec2DPos, 1);

    texOffset = lookup[gl_InstanceID].PIOffset[gl_VertexID];

}