#version 330 core
layout(location = 0) in vec4 vertex_position_homogeneous;
layout(location = 1) in vec3 vertex_color;

out vec3 fragment_color;

uniform mat4 mvp_matrix;

void main() {
	gl_Position = mvp_matrix * vertex_position_homogeneous;
	fragment_color = vertex_color;
}
