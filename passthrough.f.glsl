#version 410

in vec4 fragment_in_color;
out vec4 fragment_out_color;

void main() {
	fragment_out_color = fragment_in_color;
}