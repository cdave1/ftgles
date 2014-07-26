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


static GLuint shaderProgram = 0;
static GLuint cameraUniform;

static float cameraMatrix[16];
static float screenWidth, screenHeight, scale;


@implementation AppDelegate

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
    if (status == GL_FALSE ) {
        char *log;
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        log = (char *)calloc(0, length);
        glGetShaderInfoLog(shader, length, &status, log);
        fprintf(stderr, "Shader error: %s %s\n", [filePath cStringUsingEncoding:NSASCIIStringEncoding], log);
        free(log);
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
    
    glBindAttribLocation(shaderProgram, RENDER_ATTRIB_VERTEX, "position");
    glBindAttribLocation(shaderProgram, RENDER_ATTRIB_COLOR, "color");
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint length;
        char *log;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        log = (char *)calloc(0, length);
        glGetProgramInfoLog(shaderProgram, length, &status, log);
        fprintf(stderr, "Shader link error: %s\n", log);
        free(log);
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    
    cameraUniform = glGetUniformLocation(shaderProgram, "camera");
    
    aglOrtho(cameraMatrix, -screenWidth, screenWidth, -screenHeight, screenHeight, -10000.0f, 10000.0f);
}


- (void) Setup {
	[self SetupFonts];
    [self SetupGL];
	glClearColor(0, 0, 0, 1.0f);
}


- (void) Update {
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


static float mover = 0.0f;
- (void) Render {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    
    float orthoMatrix[16];
    float translationMatrix[16];
    float resultMatrix[16];
    float perspectiveMatrix[16];
    
    mover += 0.01f;
    
    //aglOrtho(cameraMatrix, 0, screenWidth, 0, screenHeight, 0.1f, 1000.0f);
    //aglMatrixIdentity(translationMatrix);
    //aglMatrixTranslation(translationMatrix, sinf(pos)-1.2f, -2.0f, 0.0f);
    //aglMatrixMultiply(resultMatrix, cameraMatrix, translationMatrix);
    
    vec3_t pos, at, up;
    
    vec3Set(pos, 500, 400, -500.0f);
    vec3Set(at,  500.0f + 100.0f * sinf(mover), 400, -1000.0f);
    vec3Set(up,  0.0f, 1.0f, 0.0f);
    
    aglMatrixLookAtRH(cameraMatrix, pos, at, up);
    aglMatrixPerspectiveFovRH(perspectiveMatrix, 90.0f, screenWidth/screenHeight, 0.01f, 1000.0f);
    aglMatrixMultiply(cameraMatrix, cameraMatrix, perspectiveMatrix);
    aglMatrixIdentity(translationMatrix);
    aglMatrixMultiply(resultMatrix, cameraMatrix, translationMatrix);
    
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, resultMatrix);
    
    ftglColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	if (polygonFont)
		polygonFont->Render("Polygon Font");

    glLineWidth(16.0f);
    aglBegin(GL_LINE_LOOP);
    aglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    aglVertex3f(0.0f, 0.0f, 0.0f);
    aglVertex3f(screenWidth, 0.0f, 0.0f);
    aglVertex3f(screenWidth, screenHeight, 0.0f);
    aglVertex3f(0.0f, screenHeight, 0.0f);
    aglEnd();

    aglMatrixIdentity(cameraMatrix);
    aglMatrixIdentity(resultMatrix);
    aglMatrixIdentity(translationMatrix);
    aglMatrixLookAtRH(cameraMatrix, pos, at, up);
    aglMatrixMultiply(cameraMatrix, cameraMatrix, perspectiveMatrix);
    aglMatrixTranslation(translationMatrix, 0.1f * sinf(mover), 0.25f, 0.0f);
    aglMatrixMultiply(resultMatrix, cameraMatrix, translationMatrix);
    
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, resultMatrix);
    ftglColor4f(0.5f, 0.5f, 0.5f, 1.0f);
    if (textureFont)
		textureFont->Render("Texture Font");
    
    
    aglOrtho(cameraMatrix, 0, screenWidth, 0, screenHeight, 0.1f, 1000.0f);
    aglMatrixIdentity(translationMatrix);
    aglMatrixTranslation(translationMatrix, 0.0f, -2.0f, 0.0f);
    aglMatrixMultiply(resultMatrix, cameraMatrix, translationMatrix);
    
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, resultMatrix);
    
    glLineWidth(16.0f);
    aglBegin(GL_LINE_LOOP);
    aglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    aglVertex3f(0.0f, 0.0f, 0.0f);
    aglVertex3f(screenWidth, 0.0f, 0.0f);
    aglVertex3f(screenWidth, screenHeight, 0.0f);
    aglVertex3f(0.0f, screenHeight, 0.0f);
    aglEnd();
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
