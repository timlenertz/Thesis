#version 120
attribute vec4 vertex_position_homogeneous;
attribute vec3 vertex_color;

varying vec3 fragment_color;

uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

uniform bool fog;
uniform float fog_distance;
uniform vec3 fog_color;

void main() {
	gl_Position = mvp_matrix * vertex_position_homogeneous;
	
	if(fog) {
		float d = length(mv_matrix * vertex_position_homogeneous);
		float min_d = fog_distance;
		float max_d = 6.0 * fog_distance;
	
		float fog_factor = (d - min_d) / (max_d - min_d);
		fog_factor = clamp(fog_factor, 0.0, 0.8);
		fragment_color = mix(vertex_color, fog_color, fog_factor);
	} else {
		fragment_color = vertex_color;
	} 
}
