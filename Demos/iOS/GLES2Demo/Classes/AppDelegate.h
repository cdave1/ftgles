#import <UIKit/UIKit.h>
#include <FTGL/ftgles.h>
#import "ViewController.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate> {
@private
	FTFont *polygonFont;
    FTFont *textureFont;

    GLint positionLocation;
    GLint colorLocation;
    GLint texCoordLocation;

    GLuint shaderProgram;
    GLint cameraUniform;
    GLint colorUniform;
    GLint useTextureUniform;

    float polygonFontViewMatrix[16];
    float textureFontViewMatrix[16];

    float screenWidth, screenHeight, scale;
}

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) ViewController *viewController;

- (void) Setup;
- (void) Update;
- (void) ReportFPS:(NSNumber *)frames;
- (void) Render;

@end



