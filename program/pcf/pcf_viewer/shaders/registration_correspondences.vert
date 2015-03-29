#version 330 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in float vertex_weight;

out vec4 fragment_color;

uniform mat4 mvp_matrix;
uniform float min_weight;
uniform float max_weight;
uniform vec3 min_edge_color;
uniform vec3 max_edge_color;
uniform float min_edge_alpha;
uniform float max_edge_alpha;


void main() {
	vec4 vertex_position_homogeneous = vec4(vertex_position.xyz, 1.0);

	gl_Position = mvp_matrix * vertex_position_homogeneous;
	
	float a;
	float diff = max_weight - min_weight;
	if(diff != 0.0) a = (vertex_weight - min_weight) / diff;
	else a = 0.0;

	vec4 mincol = vec4(min_edge_color, min_edge_alpha);
	vec4 maxcol = vec4(max_edge_color, max_edge_alpha);

	fragment_color = (1.0 - a)*mincol + a*maxcol;
}
