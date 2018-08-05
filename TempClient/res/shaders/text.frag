#version 460 core

out vec4 color;

in vec2 texOffset;
uniform sampler2D texBitmap;

uniform vec3 textColor;

void main(){
    color = texture(texBitmap, texOffset);

}