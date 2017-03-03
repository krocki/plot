#version 410

uniform mat4 modelViewProj;
in vec3 vertex_in_position;
in vec3 vertex_in_color;
out vec4 fragment_in_color;

void main() {
	fragment_in_color = vec4 ( vertex_in_color, 0.5 );
	gl_Position = modelViewProj * vec4 ( vertex_in_position, 1.0 );
}