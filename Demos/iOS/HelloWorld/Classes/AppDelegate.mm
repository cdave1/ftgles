/*
 
 Copyright (c) 2010 David Petrie david@davidpetrie.com
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the 
 use of this software. Permission is granted to anyone to use this software for
 any purpose, including commercial applications, and to alter it and 
 redistribute it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim 
 that you wrote the original software. If you use this software in a product, an 
 acknowledgment in the product documentation would be appreciated but is not 
 required.
 2. Altered source versions must be plainly marked as such, and must not be 
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

#import "AppDelegate.h"
#include "HelloWorld.h"

static HelloWorldApp app;

@implementation AppDelegate

- (void) applicationDidFinishLaunching:(UIApplication*)application {
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        self.viewController = [[[ViewController alloc] initWithNibName:@"ViewController_iPhone" bundle:nil] autorelease];
    } else {
        self.viewController = [[[ViewController alloc] initWithNibName:@"ViewController_iPad" bundle:nil] autorelease];
    }
	
	NSString *path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Diavlo_BLACK_II_37.otf"];
    
    float scale = [[UIScreen mainScreen] scale];
    float width = [[UIScreen mainScreen] bounds].size.width;
    float height = [[UIScreen mainScreen] bounds].size.height;

    app.SetupFonts([path UTF8String]);
    app.SetScreenDimensions(width, height, scale);
    
    self.viewController.delegate = self;
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    return YES;
}


- (void) Update {
}


- (void) ReportFPS:(NSNumber *)frames {
}


- (void) Render {
	app.Render();
}

@end
