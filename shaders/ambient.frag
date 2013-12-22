#version 140

precision highp float; // needed only for version 1.30

uniform vec4 ambientColor;

uniform sampler2D colorBuffer;

in vec2 ex_UV;
out vec4 out_Color;

void main(void) {	
	// GET COLOR DATA
	vec4 colorData = texture(colorBuffer,ex_UV);
	
	// Apply ambient lighting
	colorData *= ambientColor;
	out_Color = colorData;
}