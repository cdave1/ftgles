precision highp float;
varying vec4 colorVarying;
varying vec2 texture_coordinate;

uniform sampler2D color_sampler;

void main() {
    vec4 color = texture2D(color_sampler, texture_coordinate);
    gl_FragColor = vec4(colorVarying.r, colorVarying.g, colorVarying.b, color.a);
}