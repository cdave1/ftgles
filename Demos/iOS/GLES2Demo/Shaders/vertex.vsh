attribute vec4 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec4 vertexColor;

uniform mat4 camera;
uniform vec4 color;

varying vec2 fragmentTexCoord;
varying vec4 fragmentColor;
varying vec4 colorVarying;

void main() {
	gl_Position = (camera * vertexPosition);
    fragmentTexCoord = vertexTexCoord;
    fragmentColor = vertexColor;
    colorVarying = color;
}
