//
//  Shader.vsh
//  OpenGLTest3
//
//  Created by David Petrie on 18/05/10.
//  Copyright n/a 2010. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;
attribute vec2 texCoord;
varying vec2 texture_coordinate;

uniform mat4 camera;

varying lowp vec4 colorVarying;

void main()
{
	gl_Position = (camera * position);
    texture_coordinate = texCoord;
    colorVarying = color;
}
