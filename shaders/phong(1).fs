#version 330 core

in vec4 v_Position;
in vec3 v_Normal;
layout(location = 0) out vec4 fragcolor;


uniform vec4 u_Diffuse;
uniform vec4 u_Ambient;
uniform vec4 u_Specular;
uniform float u_SpecInt;
uniform vec4 u_LightCol;

vec4 ComputeLight(vec3 light_dir, vec4 light_col, vec3 normal, vec3 half_angle,
	vec4 diffuse, vec4 spec, float spec_size, float light_int)
{
	float surface_int = max(dot(normal, -light_dir), 0.0f);
	vec4 lambert = diffuse * light_col * surface_int * light_int;

	float spec_int = pow(max(dot(normal, half_angle), 0.0f), spec_size);
	vec4 phong = spec * light_col * spec_int * surface_int;

	return lambert + phong;
};

void main() 
{
	vec3 lightdir = vec3(-1, -1, -1);
	vec3 pos = v_Position.xyz / v_Position.w;
	vec3 hlf = -normalize(normalize(lightdir) + normalize(pos));
	vec4 light = ComputeLight(lightdir, u_LightCol, normalize(v_Normal), hlf, 
		u_Diffuse, u_Specular, u_SpecInt, 1);

	fragcolor = light + u_Ambient;
};