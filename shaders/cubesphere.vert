// Vertex Shader – file "minimal.vert"
 
#version 140
 
in  vec3 in_Position;
in  vec3 in_Color;
in  vec3 in_Normal;
in  vec3 in_UVW;

uniform  mat4 in_Model;
uniform  mat4 in_View;
uniform  mat4 in_Proj;

out vec3 ex_Color;
out vec3 ex_Normal;
out vec3 ex_UVW;

int face(vec3 coord) {
    if((abs(coord.x) > abs(coord.y)) && (abs(coord.x) > abs(coord.z)))
    {
      if (coord.x > 0)
        return 0;
      else
        return 1;
    }
    if( abs(coord.y) > abs(coord.z))
    {
      if (coord.y > 0)
        return 2;
      else
        return 3;
    }
    if (coord.z > 0)
      return 4;
    return 5;         
}

vec3 cube(vec3 position,vec3 cubeOrigin)
{
    int CUBE_SIZE = 2;
    vec3 coord = normalize(position - cubeOrigin);
    int face = face(coord);
    vec3 faceNormal = vec3(0);
    switch (face)
    {
      case 0: faceNormal = vec3(1,0,0); break;
      case 1: faceNormal = vec3(-1,0,0); break;
      case 2: faceNormal = vec3(0,1,0); break;
      case 3: faceNormal = vec3(0,-1,0); break;
      case 4: faceNormal = vec3(0,0,1); break;
      case 5: faceNormal = vec3(0,0,-1); break;
    }  

    float gamma = acos (dot(faceNormal,coord));
    float alfa = 3.14 - 1.57 - gamma; //there is no PI or HALF_PI in glsl you have to define it on your own.
    //this is little hard stuff. You want to know length from origin to place on cube. It is simple triangle and can be solved by law of sines. 
    //It should be multiplied with sin(HALF_PI) but it is 1. 
    float posFactor =  (CUBE_SIZE/2.0)/sin(alfa) ; 

    return (cubeOrigin + coord * posFactor);      
}

 
void main(void)
{
	vec3 transformedPos = normalize(in_Position);
	gl_Position = in_Proj * (in_View * (in_Model * vec4(transformedPos,1)));
    ex_Color = in_Color;
	ex_Normal = (in_Model * vec4(transformedPos,0)).xyz;
	ex_UVW = transformedPos;
}