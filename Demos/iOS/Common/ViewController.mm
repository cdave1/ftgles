#import "ViewController.h"

static int frames;
static CFTimeInterval CurrentTime;
static CFTimeInterval LastFPSUpdate;
static BOOL clientInitted = NO;

@interface ViewController () {}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
@end


@implementation ViewController

@synthesize delegate;

- (void)dealloc {
    [_context release];
    [_effect release];
    [super dealloc];
}


- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [view  setMultipleTouchEnabled:YES];
    
    self.preferredFramesPerSecond = 60;
    [self setupGL];
}


- (void)viewDidUnload {
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
            interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}


- (void)setupGL {
    [EAGLContext setCurrentContext:self.context];
    
    glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    if (self.delegate) {
        [self.delegate performSelector:@selector(Setup)];
    }
}


- (void)tearDownGL {
    [EAGLContext setCurrentContext:self.context];
}


#pragma mark - GLKView and GLKViewController delegate methods

- (void)update {
    if (self.delegate) {
        [self.delegate performSelector:@selector(Update)];
    }
    
    ++frames;
	CurrentTime = CACurrentMediaTime();
	
	if ((CurrentTime - LastFPSUpdate) > 1.0f) {
        if (self.delegate) {
            [self.delegate performSelector:@selector(ReportFPS:) withObject:[NSNumber numberWithFloat:frames]];
        }
        
		printf("fps: %d\n", frames);
		frames = 0;
		LastFPSUpdate = CurrentTime;
	}
}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    if (self.delegate) {
        [self.delegate performSelector:@selector(Render)];
    }
}

@end