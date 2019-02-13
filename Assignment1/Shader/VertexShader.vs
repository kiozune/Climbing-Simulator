#version 330 core

layout(location = 0)in vec2 pos;

layout(location = 1)in vec2 uvCoord;

out vec2 uv;

void main()
{
  uv = uvCoord;

  gl_Position = vec4(pos.x,pos.y,0,1);
}