#version 400

out vec4 FragColor;
in vec4 pos;
in vec3 norm;

uniform vec4  u_Color1;
uniform vec4  u_Color2;
uniform vec4  u_Spec;
uniform float u_SpecSize;

vec4 ComputeLight(vec4 col1, vec3 lightDir, vec3 normal, vec3 halfAngle,
	vec4 col2, vec4 spec, float spec_size)
{
	// float spec_int = max(dot(normal, halfAngle), 0.0f);
	// if (spec_int < u_SpecSize) return spec;

	float light_int = max(dot(normalize(normal), vec3(0, 0, -1)), 0.0f);
	vec4 lambert;
	if (light_int > .5) return col1;
	else return col2;

};

void main()
{
	vec3 u_LightDir = vec3(0, 0, -1);
	vec3 hpos = pos.xyz / pos.w;
	vec3 hlf = -normalize(normalize(u_LightDir) + normalize(hpos));
	vec4 light = ComputeLight(u_Color1, u_LightDir, norm, hlf, u_Color2, u_Spec, u_SpecSize);

	FragColor = light;
}

