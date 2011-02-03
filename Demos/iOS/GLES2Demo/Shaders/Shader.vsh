//
//  Shader.vsh
//  OpenGLTest3
//
//  Created by David Petrie on 18/05/10.
//  Copyright n/a 2010. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;


varying vec4 colorVarying;

uniform mat4 camera;
uniform mat4 rotation;
uniform vec4 translation;

void main()
{
	colorVarying = color;
	gl_Position = (camera * position);
}
