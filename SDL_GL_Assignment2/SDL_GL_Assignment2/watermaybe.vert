#version 450

in vec3 VertexPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float u_time;

out vec3 FragPos;

void main()
{
	FragPos = vec3(model * vec4(VertexPosition, 1.0));

	gl_Position = projection * view * model * vec4(VertexPosition * sin(u_time), 1.0);
}