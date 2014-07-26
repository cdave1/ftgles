#import <UIKit/UIKit.h>
#include <FTGL/ftgles.h>
#import "ViewController.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate> {
@private
	FTFont *polygonFont;
    FTFont *textureFont;
}

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) ViewController *viewController;

- (void) Setup;
- (void) Update;
- (void) ReportFPS:(NSNumber *)frames;
- (void) Render;

@end



