uniform mat4 viewProjInverse;
uniform vec3 lightPosW;
uniform float lightRadius;
uniform vec4 lightColor;

uniform sampler2D normalBuffer;
uniform sampler2D depthBuffer;

in vec2 ex_UV;

vec3 decode(vec3 normal) {
	return normal * 2.0f - 1.0f;
}

void main(void) {
	// GET NORMAL DATA
	vec4 normalData = texture(normalBuffer,ex_UV);

	// Transform normal back to [-1, 1] range
	float3 normal = decode(normalData.rgb);
	
	// RECREATE POSITION
	// Retrieve screen-space depth value
	float depthValue = texture(depthBuffer, ex_UV).r;

	// screen space position
	float4 position;

	position.x = ex_UV.u * 2.0f - 1.0f;
	position.y = -(ex_UV.v * 2.0f - 1.0f);
	position.z = depthValue;
	position.w = 1.0f;

	// transform to world space
	position = mul(position, viewProjInverse);
	position /= position.w;

	// CALCULATE LIGHTING //
	// surface-to-light vector
	float3 lightVector = lightPosW - position.xyz;

	// ATTENUATION ////////
	//float d = length(lightVector); 
	//float A = saturate( 1.0f - d / lightRadius );
	
	float A = 1.0f - dot(lightDirW, lightDirW) / (lightRadius*lightRadius);

	lightVector = normalize(lightVector);

	// DIFFUSE ////////////
	float	NdL				= dot(normal, lightVector);
	float3	diffuseLight	= NdL * lightColor.rgb;

	return float4(diffuseLight*A, 1.0f);
}