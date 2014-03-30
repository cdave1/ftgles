//
//  Shader.fsh
//  OpenGLTest3
//
//  Created by David Petrie on 18/05/10.
//  Copyright n/a 2010. All rights reserved.
//

varying lowp vec4 colorVarying;
precision mediump float;
//attribute vec4 color;
varying vec2 texture_coordinate;
uniform sampler2D color_sampler;


void main() {
    vec4 color = texture2D(color_sampler, texture_coordinate);
    gl_FragColor = vec4(colorVarying.r, colorVarying.g, colorVarying.b, color.a);
}