precision highp float;

uniform sampler2D texture0;

varying vec2 fragmentTexCoord;
varying vec4 fragmentColor;
varying vec4 colorVarying;

void main() {
    vec4 textureColor = texture2D(texture0, fragmentTexCoord);
    //gl_FragColor = color * vec4(fragmentColor.r, fragmentColor.g, fragmentColor.b, textureColor.a);
    gl_FragColor = colorVarying; //vec4(textureColor.rgba);//, 1.0);// vec4(colorVarying.r, colorVarying.g, colorVarying.b, textureColor.a);//textureColor; //vec4(vec3(textureColor.a), textureColor.a); //color; //vec4(1., 1., 1., 1.);// * textureColor;
}