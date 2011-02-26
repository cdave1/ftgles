//
//  Shader.fsh
//  OpenGLTest3
//
//  Created by David Petrie on 18/05/10.
//  Copyright n/a 2010. All rights reserved.
//

varying lowp vec4 colorVarying;
precision mediump float;
varying vec2 texture_coordinate;
uniform sampler2D color_sampler;

void main()
{
   gl_FragColor = texture2D(color_sampler, texture_coordinate);
}