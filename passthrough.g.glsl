#version 410

layout ( points ) in;
layout ( points ) out;
layout ( max_vertices = 1 ) out;

in vec4 v_color[];
out vec4 frag_color;

void main() {

	frag_color = v_color[0];
	gl_Position = gl_in[0].gl_Position;
	
	EmitVertex();
	
	EndPrimitive();
}