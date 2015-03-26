#version 330 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in float vertex_weight;

out vec4 fragment_color;

uniform mat4 mvp_matrix;
uniform float min_weight;
uniform float max_weight;
uniform vec3 min_edge_color;
uniform vec3 max_edge_color;
uniform float edge_alpha;


void main() {
	vec4 vertex_position_homogeneous = vec4(vertex_position.xyz, 1.0);

	gl_Position = mvp_matrix * vertex_position_homogeneous;
	
	float a;
	float diff = max_weight - min_weight;
	if(diff != 0.0) a = (vertex_weight - min_weight) / diff;
	else a = 0.0;

	vec3 col = (1.0 - a)*min_edge_color + a*max_edge_color;
	fragment_color = vec4(col, edge_alpha);
}
