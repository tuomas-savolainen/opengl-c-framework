#version 130
uniform float programTime;
out vec3 normal;
out vec3 fromVertexToLight0;
out vec3 fromVertexToLight1;
out vec3 position;
void main(void) 
{

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex  ; 
	position = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
	normal = gl_NormalMatrix * gl_Normal;
	vec4 vertexInModelViewSpace = gl_ModelViewMatrix * gl_Vertex; 
	vec4 light0 = vec4(15.0*cos(programTime/2.0),15.0*sin(programTime/2.0) , -10.0, 1.0);
	vec4 light1 = vec4(15.0*cos((programTime/2.0+3.14159265)),15.0*sin((programTime/2.0+3.14159265)) , -10.0, 1.0);
	fromVertexToLight0 = vec3(light0 - gl_Vertex);
	fromVertexToLight1 = vec3(light1 - gl_Vertex);
}