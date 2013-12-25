#version 140

precision highp float; // needed only for version 1.30

uniform mat4 viewProjInverse;
uniform vec3 lightPosW;
uniform vec3 lightDirW;
uniform vec4 lightColor;

uniform float lightAngle;
uniform float lightCosCutoff;
uniform float lightExponent;

uniform sampler2D diffuseBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D depthBuffer;

in vec2 ex_UV;
out vec4 out_Color;

vec3 decode(vec3 normal) {
	return normal * 2.0 - 1.0;
}

void main(void) {
	// GET DIFFUSE DATA
	vec4 diffuse = texture(diffuseBuffer,ex_UV);
	
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

	// DIFFUSE ////////////
	vec3 lightVector = lightPosW - position.xyz;
	
	float	NdL	= max(dot(normal, normalize(lightVector)), 0.0);
	float   spotLight = 0.0;
	
	float cos_outer_cone_angle = lightCosCutoff;
	float cos_inner_cone_angle = lightCosCutoff + 0.15;
	float cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;
	
	if (NdL > 0.0) {
		float spotEffect = dot(normalize(lightDirW), normalize(-lightVector));
		
		//if(spotEffect > lightCosCutoff) {
		//	spotLight = pow(spotEffect, lightExponent);
		//}
		spotLight = clamp((spotEffect - cos_outer_cone_angle) / cos_inner_minus_outer_angle, 0.0, 1.0);
	}

	out_Color = vec4(diffuse.rgb*spotLight, 1.0);
}