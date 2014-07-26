attribute vec4 position;
attribute vec4 color;
attribute vec2 texCoord;

uniform mat4 camera;

varying vec2 texture_coordinate;
varying vec4 colorVarying;

void main() {
	gl_Position = (camera * position);
    texture_coordinate = texCoord;
    colorVarying = color;
}
