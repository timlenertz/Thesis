#version 330 core
layout(location = 0) in vec3 vertex_position;

out vec3 fragment_color;

uniform vec3 color;

uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

uniform bool fog;
uniform float fog_distance;
uniform vec3 fog_color;

void main() {
	vec4 vertex_position_homogeneous = vec4(vertex_position.xyz, 1.0);

	gl_Position = mvp_matrix * vertex_position_homogeneous;
	
	if(fog) {
		float d = length(mv_matrix * vertex_position_homogeneous);
		float min_d = fog_distance;
		float max_d = 6.0 * fog_distance;
	
		float fog_factor = (d - min_d) / (max_d - min_d);
		fog_factor = clamp(fog_factor, 0.0, 0.8);
		fragment_color = mix(color, fog_color, fog_factor);
	} else {
		fragment_color = color;
	} 
}
