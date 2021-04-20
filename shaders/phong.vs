#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNor;

out vec4 v_Light;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec4 u_Diffuse;
uniform vec4 u_Ambient;
uniform vec4 u_LightCol;
uniform vec4 u_Specular;
uniform float u_SpecInt;

vec4 ComputeLight(vec3 lightDir, vec4 lighCol, vec3 normal, vec3 halfAngle,
	vec4 diffuse, vec4 spec, float spec_size)
{
	float light_int = max(dot(normalize(normal), -normalize(lightDir)), 0.0f);
	vec4 lambert = diffuse * lighCol * light_int;

	float spec_int = pow(max(dot(normal, halfAngle), 0.0f), spec_size);
	vec4 phong = spec * lighCol * spec_int;
	
	return lambert + phong;
};

void main()
{
	vec3 u_LightDir = vec3(-1, 1, -1);
	vec4 pos = u_Model * u_View * vec4(vPos, 1.0f);
	vec3 norm = vec3(transpose(inverse(u_Model * u_View)) * vec4(vNor, 1.0f));
	vec3 hpos = pos.xyz / pos.w;
	vec3 hlf = -normalize(normalize(u_LightDir) + normalize(hpos));
	vec4 light = ComputeLight(u_LightDir, u_LightCol, norm, hlf, u_Diffuse, u_Specular, u_SpecInt);

	v_Light = u_Ambient + light;

	gl_Position = u_Projection * u_Model * u_View * vec4(vPos, 1.0f);
};


