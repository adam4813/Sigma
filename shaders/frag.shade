// Fragment Shader – file "minimal.frag"
 
#version 140
 
precision highp float; // needed only for version 1.30
 
uniform sampler2D tex;
 
in  vec3 ex_Color;
in vec2 ex_UV;
out vec4 out_Color;

void main(void)
{
    //out_Color = vec4(ex_Color,1.0f);
	out_Color = texture(tex,ex_UV);
}