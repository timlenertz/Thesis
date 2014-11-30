#version 330 core
in vec3 fragment_color;
out vec3 color;

void main() {
	color = vec3(1.0, 1.0, 1.0);

	//gl_FragColor = vec4(fragment_color, 1.0);
}
