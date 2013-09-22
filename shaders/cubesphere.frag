// Fragment Shader – file "minimal.frag"
 
#version 140
 
precision highp float; // needed only for version 1.30

uniform vec3 light = normalize(vec3(-1.0, 1.0, -1.0));
uniform samplerCube cubeMap;
uniform samplerCube cubeNormMap;

in  vec3 ex_NormalW;
in  vec3 ex_TangentW;
in  vec3 ex_BiNormalW;
in  vec3 ex_Light;
in  vec3 ex_UVW;
out vec4 out_Color;
 
void main(void) {
		vec3 normalMap = (2.0 * texture(cubeNormMap, ex_UVW.stp).rgb - 1.0);
		normalMap.z = sqrt(1.0 - dot(normalMap.xy, normalMap.xy));
		vec3 normal = normalize((normalize(ex_TangentW)*normalMap.x) + (normalize(ex_BiNormalW) * normalMap.y) + (normalize(ex_NormalW)*normalMap.z));

		float cosTheta = max(dot(normal, light), 0.0);
		out_Color = vec4(texture(cubeMap, ex_UVW.stp).rgb*cosTheta, 1.0f);
}