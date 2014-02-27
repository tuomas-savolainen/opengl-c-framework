#version 130
uniform float programTime;
in vec3 normal;
in vec3 fromVertexToLight0;
in vec3 fromVertexToLight1;
in vec3 position;
void main(void)
{

	const vec4 ambientColor0 = vec4(0.1, 0.1, 0.1, 1.0);
	const vec4 specularColor0 = vec4(0.2, 0.2, 0.4, 1.0);
	const vec4 diffuseColor0 = vec4(0.05, 0.05, 0.1, 1.0);
	float attenuation0 = 1 /  (0.01 + 0.2 * length(fromVertexToLight0) + 0.3 * length(fromVertexToLight0) * length(fromVertexToLight0) );

	vec3 unitNormal = normalize(normal);
	vec3 unitFromVertexToLight0 = normalize(fromVertexToLight0);

	float diffuseTerm0 = clamp(dot(unitNormal, unitFromVertexToLight0), 0.0, 1.0);

	vec4 specularReflection0;
	if (dot(unitNormal, unitFromVertexToLight0) < 0.0) specularReflection0 = vec4(0.0, 0.0, 0.0, 1.0);
	else specularReflection0 =  specularColor0 * attenuation0 * pow(max(0.0, dot(reflect(-unitFromVertexToLight0, unitNormal), position)), 3.0);

	const vec4 specularColor1 = vec4(0.2, 0.4, 0.2, 1.0);
	const vec4 diffuseColor1 = vec4(0.05, 0.1, 0.05, 1.0);
	float attenuation1 = 1 /  (0.01 + 0.2 * length(fromVertexToLight1) + 0.3 * length(fromVertexToLight1) * length(fromVertexToLight1) );

	vec3 unitFromVertexToLight1 = normalize(fromVertexToLight1);

	float diffuseTerm1 = clamp(dot(unitNormal, unitFromVertexToLight1), 0.0, 1.0);

	vec4 specularReflection1;
	if (dot(unitNormal, unitFromVertexToLight1) < 0.0) specularReflection1 = vec4(0.0, 0.0, 0.0, 1.0);
	else specularReflection1 =  specularColor1 * attenuation1 * pow(max(0.0, dot(reflect(-unitFromVertexToLight1, unitNormal), position)), 3.0);


	gl_FragColor = ambientColor0*0.3 + (specularReflection0 + diffuseColor0 * diffuseTerm0)*0.3 + (specularReflection1 + diffuseColor1 * diffuseTerm1)*0.3;
}