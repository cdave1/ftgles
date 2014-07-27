attribute vec4 position;
attribute vec4 color;
attribute vec2 texCoord;

uniform mat4 camera;
uniform vec4 altColor;

varying vec4 colorVarying;
varying vec2 texCoordVarying;
varying vec4 altColorVarying;

void main() {
	gl_Position = (camera * position);

    colorVarying = color;
    texCoordVarying = texCoord;
    altColorVarying = altColor;
}
