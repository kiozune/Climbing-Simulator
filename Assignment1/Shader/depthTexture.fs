#version 330 core
 
// Ouput data
in vec2 texCoord;

uniform bool colorTextureEnabled[3];
uniform sampler2D colorTexture[3];

layout(location = 0) out float fragmentdepth;
 
void main()
{
	
	float alpha = 1.0;
	if(colorTextureEnabled[0] == true)
		alpha = texture2D(colorTexture[0], texCoord).a;
	if(alpha < 0.3)
		discard;
	
    fragmentdepth = gl_FragCoord.z;
}