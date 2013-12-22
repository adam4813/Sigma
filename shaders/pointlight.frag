#version 140

precision highp float; // needed only for version 1.30

uniform mat4 viewProjInverse;
uniform vec3 lightPosW;
uniform float lightRadius;
uniform vec4 lightColor;

uniform sampler2D normalBuffer;
uniform sampler2D depthBuffer;

in vec2 ex_UV;
out vec4 out_Color;

vec3 decode(vec3 normal) {
	return normal * 2.0 - 1.0;
}

void main(void) {
	//out_Color = vec4(0.0, 0.0, 1.0, 0.5);
	
	// GET NORMAL DATA
	vec4 normalData = texture(normalBuffer,ex_UV);

	// Transform normal back to [-1, 1] range
	vec3 normal = decode(normalData.rgb);
	
	// RECREATE POSITION
	// Retrieve screen-space depth value
	float depthValue = texture(depthBuffer, ex_UV).r;

	// screen space position
	vec4 position;

	position.x = ex_UV.s * 2.0 - 1.0;
	position.y = ex_UV.t * 2.0 - 1.0;
	position.z = depthValue;
	position.w = 1.0;

	// transform to world space
	position = viewProjInverse * position;
	position /= position.w;

	// CALCULATE LIGHTING //
	// surface-to-light vector
	vec3 lightVector = lightPosW - position.xyz;

	// ATTENUATION ////////
	float d = length(lightVector); 
	float A = clamp(1.0 - (d/3)*d / lightRadius, 0.0, 1.0);
	
	//float A = 1.0f - sqrt(dot(lightVector, lightVector)) / lightRadius;
	
	//float A = 1.0 - dot(lightPosW, lightPosW) / (lightRadius*lightRadius);

	lightVector = normalize(lightVector);

	// DIFFUSE ////////////
	float	NdL				= dot(normal, lightVector);
	vec3	diffuseLight	= NdL * lightColor.rgb;

	out_Color = vec4(diffuseLight*A, 1.0f);
	//out_Color = vec4(normal, 1.0);
	//out_Color = vec4(depthValue, depthValue, depthValue, 1.0);
}