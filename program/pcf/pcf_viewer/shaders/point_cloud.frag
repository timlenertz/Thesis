#version 120
varying vec3 fragment_color;

void main() {
	gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);
	//gl_FragColor = vec4(fragment_color, 1.0);
}
