#version 450

uniform float u_time;

in vec3 FragPos;

out vec4 frag_colour;

void main() {

	vec2 u_resolution = vec2(0.5, 0.5);

	vec2 coord = vec2(FragPos.x, FragPos.y);

	vec3 color = vec3(0.0);

	color += abs(cos(coord.x*200.0) + sin(coord.y*200.0) - cos(u_time));

	frag_colour = vec4(color, 1.0);
}