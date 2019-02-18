#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertexPosition_cameraspace;
in vec3 fragmentColor;
in vec3 vertexNormal_cameraspace;
in vec2 texCoord;
in vec2 blurVec;
//taking in shadow Coordinates
in vec4 shadowCoord;

// Ouput data
out vec4 color;

struct Light {
	int type;
	vec3 position_cameraspace;
	vec3 color;
	float power;
	float kC;
	float kL;
	float kQ;
	vec3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
};

struct Material {
	vec3 kAmbient;
	vec3 kDiffuse;
	vec3 kSpecular;
	float kShininess;
};

float getAttenuation(Light light, float distance) {
	if(light.type == 1)
		return 1;
	else
		return 1 / max(1, light.kC + light.kL * distance + light.kQ * distance * distance);
}

float getSpotlightEffect(Light light, vec3 lightDirection) {
	vec3 S = normalize(light.spotDirection);
	vec3 L = normalize(lightDirection);
	float cosDirection = dot(L, S);
	//return smoothstep(light.cosCutoff, light.cosInner, cosDirection);
	if(cosDirection < light.cosCutoff)
		return 0;
	else
		return 1; //pow(cosDirection, light.exponent);
}

// Constant values
const int MAX_LIGHTS = 1;
const int MAX_TEXTURES = 1;

// Values that stay constant for the whole mesh.
uniform bool lightEnabled;
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform int numLights;
uniform bool colorTextureEnabled[MAX_TEXTURES];
//uniform bool colorTextureEnabled1;
uniform sampler2D colorTexture[MAX_TEXTURES];
//uniform sampler2D colorTexture1;
uniform bool textEnabled;
uniform vec3 textColor;
uniform sampler2D shadowMap;

float getShadowEffect()
{
	float visibility = 1.0;
	float bias = 0.005;
	if(texture2D(shadowMap, shadowCoord.xy).x < shadowCoord.z - bias) {
		visibility = 0.25;
	}
	return visibility;
}

void main(){
	if(lightEnabled == true)
	{
		// Material properties
		vec4 materialColor = vec4(0, 0, 0, 1);
		
		int texCount = 0;
		
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if(colorTextureEnabled[i] == true)
			{
				materialColor += texture2D( colorTexture[i], texCoord );
				++texCount;

			}		
		}
		if (texCount > 0)
		{
			materialColor = materialColor/texCount;
		}
		else
			materialColor = vec4( fragmentColor, 1 );
	
		// Vectors
		vec3 eyeDirection_cameraspace = - vertexPosition_cameraspace;
		vec3 E = normalize(eyeDirection_cameraspace);
		vec3 N = normalize( vertexNormal_cameraspace );
		
		// Ambient : simulates indirect lighting
		color = materialColor * vec4(material.kAmbient, 1);
		
		for(int i = 0; i < numLights; ++i)
		{
			// Light direction
			float spotlightEffect = 1;
			vec3 lightDirection_cameraspace;
			if(lights[i].type == 1) {
				lightDirection_cameraspace = lights[i].position_cameraspace;
			}
			else if(lights[i].type == 2) {
				lightDirection_cameraspace = lights[i].position_cameraspace - vertexPosition_cameraspace;
				spotlightEffect = getSpotlightEffect(lights[i], lightDirection_cameraspace);
			}
			else {
				lightDirection_cameraspace = lights[i].position_cameraspace - vertexPosition_cameraspace;
			}
			// Distance to the light
			float distance = length( lightDirection_cameraspace );
			
			// Light attenuation
			float attenuationFactor = getAttenuation(lights[i], distance);

			vec3 L = normalize( lightDirection_cameraspace );
			float cosTheta = clamp( dot( N, L ), 0, 1 );
			
			vec3 R = reflect(-L, N);
			float cosAlpha = clamp( dot( E, R ), 0, 1 );
			
			float visibility =getShadowEffect();
			
			color +=
				// Diffuse : "color" of the object
				visibility * materialColor * vec4(material.kDiffuse, 1) * vec4(lights[i].color, 1) * lights[i].power * cosTheta * attenuationFactor * spotlightEffect +
				
				// Specular : reflective highlight, like a mirror
				visibility * vec4(material.kSpecular, 1) * vec4(lights[i].color, 1) * lights[i].power * pow(cosAlpha, material.kShininess) * attenuationFactor * spotlightEffect;

		}
	}
	else
	{
		if(colorTextureEnabled[0] == true && textEnabled == true)
		{
			color = texture2D( colorTexture[0], texCoord ) * vec4( textColor, 1 );
		}
		else
		{
			int texCount = 0;
		
			color = vec4(0, 0, 0, 0);
			for (int i = 0; i < MAX_TEXTURES; ++i)
			{
				if(colorTextureEnabled[i] == true)
				{
					color += texture2D( colorTexture[i], texCoord );
					++texCount;
				}
			}
			if (texCount > 0)
			{
				color = color/texCount;
			}
			else
				color = vec4( fragmentColor, 1 );
		}
		
	}
}