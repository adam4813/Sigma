#version 140

precision highp float; // needed only for version 1.30

uniform mat4 viewProjInverse;
uniform vec3 viewPosW;
uniform vec3 lightPosW;
uniform vec3 lightDirW;
uniform vec4 lightColor;

uniform float lightCosInnerAngle;
uniform float lightCosOuterAngle;

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
	vec3 normal = normalize(decode(normalData.rgb));
	float specularHardness = normalData.a*1000.0f;
	
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
	
	vec3 lightVector = lightPosW - position.xyz;
	float distance = length(lightVector);
	
	// ATTENUATION ///////////////
	
	// distance based
	float distAttenuation = 1.0 / (1.0 + 0.01*distance + 0.001*(distance*distance));
	
	lightVector = normalize(lightVector);
	
	// SPOTLIGHT VALUE ///////////		
	float   spotLight = dot(-lightVector, normalize(lightDirW));
	
	// angle based
	float coneAttenuation = 0.0;
	if(spotLight >= lightCosOuterAngle)
		coneAttenuation = 0.5*smoothstep(lightCosOuterAngle, lightCosInnerAngle, spotLight);
	if(spotLight >= lightCosInnerAngle)
		coneAttenuation += 0.5*smoothstep(lightCosInnerAngle, 1.0, spotLight);
	
	coneAttenuation = clamp(coneAttenuation, 0.0, 1.0);
	
	
	// DIFFUSE ////////////
	float NdL = max(dot(normal, lightVector), 0.0);
	
	// SPECULAR ////////////
	float specularLight = 0.0;
	
	// Only calculate specular if diffuse light is visible
	if(NdL > 0.0) {
		vec3 viewVector = normalize(viewPosW - position.xyz);
		vec3 halfVector = normalize(lightVector + viewVector);
		float NdH = dot(normal, halfVector);
		specularLight = pow(clamp(NdH, 0.0, 1.0), specularHardness);
	}
	
	//out_Color = vec4(clamp(spotLight*diffuse.rgb*clamp(NdL + specularLight, 0.0, 1.0), 0.0, 1.0), 1.0);
	out_Color = clamp(vec4(distAttenuation*coneAttenuation*diffuse.rgb*clamp(NdL + specularLight, 0.0, 1.0), 1.0), 0.0, 1.0);
}