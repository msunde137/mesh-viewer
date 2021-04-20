#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out vec4 v_Position;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	mat4 u_ModelView = u_View * u_Model;
	v_Normal = vec3(transpose(inverse(u_ModelView)) * vec4(normal, 1.0f));
	v_Position = u_ModelView * vec4(position, 1.0f);

	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
};
