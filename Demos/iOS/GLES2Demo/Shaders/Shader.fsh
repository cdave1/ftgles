//
//  Shader.fsh
//  OpenGLTest3
//
//  Created by David Petrie on 18/05/10.
//  Copyright n/a 2010. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
