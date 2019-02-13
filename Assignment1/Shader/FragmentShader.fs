#version 330 core

in vec2 uv;

out vec3 color;

uniform sampler2D renderedTexture;
uniform float time;

void main(){
    color = texture( renderedTexture, uv ).xyz;
}