#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;
out vec4 pos;
out vec3 norm;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	vec4 pos = u_View *  u_Model * vec4(vPos, 1.0f);
	vec3 norm = vec3(transpose(inverse(u_View * u_Model)) * vec4(vNor, 1.0f));
	gl_Position = u_Projection * u_View * u_Model * vec4(vPos, 1.0f);
};


