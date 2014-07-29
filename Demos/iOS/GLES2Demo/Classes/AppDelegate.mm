#import "AppDelegate.h"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "RenderController.h"

#include <FTGL/ftglesGlue.h>


@interface AppDelegate ()
- (void) SetupFonts;
- (GLuint)compileShader:(NSString *)filePath withType:(GLenum)type;
- (void) SetupGL;
@end


@implementation AppDelegate

- (void) Setup {
    [self SetupFonts];
    [self SetupGL];
}


- (void) SetupFonts {
    NSString *fontpath = [NSString stringWithFormat:@"%@/Diavlo_BLACK_II_37.otf",
                          [[NSBundle mainBundle] resourcePath]];

    polygonFont = new FTPolygonFont([fontpath UTF8String]);
    assert (!polygonFont->Error());
    polygonFont->FaceSize(screenWidth * 0.2f);

    textureFont = new FTTextureFont([fontpath UTF8String]);
    assert (!textureFont->Error());
    textureFont->FaceSize(screenWidth * 0.2f);
}


- (GLuint)compileShader:(NSString *)filePath withType:(GLenum)type {
    assert(shaderProgram);

    GLuint shader;
    GLint status;
    const GLchar *source;

    if (!(source = (GLchar *)[[NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil] UTF8String])) {
        NSLog(@"Failed to load vertex shader");
        return FALSE;
    }

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    char *log;
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length > 0) {
        log = (char *)calloc(1, length);
        glGetShaderInfoLog(shader, length, &status, log);
        fprintf(stderr, "Shader error: %s %s\n", [filePath cStringUsingEncoding:NSASCIIStringEncoding], log);
        free(log);
    }

    if (status == GL_FALSE ) {
        glDeleteShader(shader);
        return 0;
    }

    glAttachShader(shaderProgram, shader);
    return shader;
}


- (void)SetupGL {
    GLint status;

    shaderProgram = glCreateProgram();

    NSString *vshPath = [[NSBundle mainBundle] pathForResource:@"vertex" ofType:@"vsh"];
    NSString *fshPath = [[NSBundle mainBundle] pathForResource:@"fragment" ofType:@"fsh"];

    GLuint vertexShader = [self compileShader:vshPath withType:GL_VERTEX_SHADER];
    GLuint fragmentShader = [self compileShader:fshPath withType:GL_FRAGMENT_SHADER];

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    cameraUniform = glGetUniformLocation(shaderProgram, "camera");
    colorUniform = glGetUniformLocation(shaderProgram, "altColor");
    useTextureUniform = glGetUniformLocation(shaderProgram, "useTexture");

    positionLocation = glGetAttribLocation(shaderProgram, "position");
    colorLocation = glGetAttribLocation(shaderProgram, "color");
    texCoordLocation = glGetAttribLocation(shaderProgram, "texCoord");
}


static float mover = 0.0f;
- (void) Update {
    float perspectiveMatrix[16];
    aglMatrixPerspectiveFovRH(perspectiveMatrix, 90.0f, screenWidth/screenHeight, 0.01f, 1000.0f);

    mover += 0.01f;

    vec3_t pos, at, up;
    vec3Set(pos, 420, 200, -500.0f);
    vec3Set(at,  420.0f + 100.0f * sinf(mover), 200, -1000.0f);
    vec3Set(up,  0.0f, 1.0f, 0.0f);

    // Render the polygon font
    aglMatrixLookAtRH(polygonFontViewMatrix, pos, at, up);
    aglMatrixMultiply(polygonFontViewMatrix, polygonFontViewMatrix, perspectiveMatrix);

    // Render the texture font
    const float moveUpBy = 450.0f;
    vec3Set(at, at[0], at[1] - moveUpBy, at[2]);
    vec3Set(pos, pos[0], pos[1] - moveUpBy, pos[2]);
    aglMatrixLookAtRH(textureFontViewMatrix, pos, at, up);
    aglMatrixMultiply(textureFontViewMatrix, textureFontViewMatrix, perspectiveMatrix);
}


void glerr(const char *src) {
    GLenum error = glGetError();
    switch (error) {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            printf("GL Error (%x): GL_INVALID_ENUM. %s\n", error, src);
            break;
        case GL_INVALID_VALUE:
            printf("GL Error (%x): GL_INVALID_VALUE. %s\n", error, src);
            break;
        case GL_INVALID_OPERATION:
            printf("GL Error (%x): GL_INVALID_OPERATION. %s\n", error, src);
            break;
        case GL_OUT_OF_MEMORY:
            printf("GL Error (%x): GL_OUT_OF_MEMORY. %s\n", error, src);
            break;
        default:
            printf("GL Error (%x): %s\n", error, src);
            break;
    }
}


- (void) Render {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, polygonFontViewMatrix);
    glUniform4f(colorUniform, 0.3f, 0.7f, 1.0f, 1.0f);
    glUniform1i(useTextureUniform, 0);

    ftglBindPositionAttribute(positionLocation);
    ftglBindColorAttribute(colorLocation);
    ftglBindTextureAttribute(texCoordLocation);

    if (polygonFont)
        polygonFont->Render("Polygon Font");

    // Render the texture font
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, textureFontViewMatrix);
    glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
    glUniform1i(useTextureUniform, 1);

    if (textureFont)
        textureFont->Render("Texture Font");
}


- (void) ReportFPS:(NSNumber *)frames {
    printf("fps: %d\n", [frames intValue]);
}


- (void) applicationDidFinishLaunching:(UIApplication*)application {
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];

    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.viewController = [[[ViewController alloc] initWithNibName:@"ViewController_iPhone" bundle:nil] autorelease];
    } else {
        self.viewController = [[[ViewController alloc] initWithNibName:@"ViewController_iPad" bundle:nil] autorelease];
    }

    scale = [[UIScreen mainScreen] scale];
    screenWidth = scale * [[UIScreen mainScreen] bounds].size.width;
    screenHeight = scale * [[UIScreen mainScreen] bounds].size.height;
    
    self.viewController.delegate = self;
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
}


- (void) dealloc {
    delete textureFont;
    delete polygonFont;
    [self.window release];
    [super dealloc];
}

@end
