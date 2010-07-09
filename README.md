# FTGLES 0.1

FTGLES is a C++ library for rendering fonts on mobile operating systems with OpenGL ES 1.1.  It is a port of (FTGL)[http://homepages.paradise.net.nz/henryj/code/#FTGL].  It currently targets the iPhone and iPad, with plans to port to Android in the very near future.  iOS 4 is currently supported.

From the FTGL notes:

"Unlike other OpenGL font libraries FTGL uses standard font file formats so doesn't need a preprocessing step to convert the high quality font data into a lesser quality, proprietary format."

"FTGL uses the [Freetype](www.freetype.org) font library to open and 'decode' the fonts. It then takes that output and stores it in a format most efficient for OpenGL rendering."


## INSTALL

#### XCode

The "iPhone" folder contains several demo xcode projects set up and ready to go.

#### Automake

The ftgles folder contains a full automake system if you want to go that way.  The conf files do NOT include the iPhone libraries, and thus you will get build errors if you just "./configure; make; make install".  Instead, use the very useful iphone-configure tool to include your preferred iPhone SDK files and set the target device.  iphone-configure is (currently available here)[http://github.com/jlongster/configure-iphone].


## USAGE

Include the library in your code:
	
	#include "FTGL/ftgles.h"

Create a font from a truetype file or opentype font:
	
	FTFont *font = new FTTextureFont("myfont.ttf");
	FTFont *font2 = new FTTextureFont("myotherfont.otf");
	font->FaceSize(72);
	
Render text within your render loop:

	font->Render("Lorem ipsum dolor sit amet");

See iPhone folder for an in-depth example that can run on an iPhone, including an example of using layouts to easily align and position your text.


## Differences between FTGL and FTGLES

FTGLES 0.1 supports the following font types from the original FTGL library:
	* FTBufferFont
	* FTGLOutlineFont
	* FTGLPolygonFont
	* FTGLTextureFont

Extrusion and pixmap rendering from the original FTGL library are currently not supported.  Bitmap font support is currently in progress.

Open GL ES 2 support is coming soon.