// Vertex Shader – file "pointlight.vert"
 
#version 140

precision highp float; // needed only for version 1.30

in vec3 in_Position;
in vec2 in_UV;

out vec2 ex_UV;

void main()
{
	//float3 alignedPos = float3(input.Position.x - gHalfPixel.x, input.Position.y - gHalfPixel.y, input.Position.z);

	gl_Position = vec4(in_Position.xy, 0, 1.0);
	ex_UV = in_UV;
}