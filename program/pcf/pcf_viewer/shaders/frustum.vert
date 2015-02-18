#version 330 core
layout(location = 0) in vec3 vertex_position;

out vec3 fragment_color;

uniform mat4 mvp_matrix;
uniform vec3 color;

void main() {
	vec4 vertex_position_homogeneous = vec4(vertex_position.xyz, 1.0);

	gl_Position = mvp_matrix * vertex_position_homogeneous;
	fragment_color = color;
}
