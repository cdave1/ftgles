# Get the local path and assign it so we can recall it
# after the modules have been installed.
LOCAL_PATH := $(call my-dir)
MY_LOCAL_PATH := $(LOCAL_PATH)

FREETYPE_PATH := ../../../freetype2-android
FTGLES_PATH := ../../../ftgles

# Install the freetype2 module
include $(FREETYPE_PATH)/Android/jni/module.mk

# For some reason, LOCAL_PATH points to the build tool's core
# directory after the shared library is installed.

LOCAL_PATH := $(MY_LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := ftgles

LOCAL_CFLAGS := -DANDROID_NDK \
		-DFT2_BUILD_LIBRARY=1

FTGLES_SRC_PATH := $(FTGLES_PATH)/ftgles/src

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include_all \
			$(FREETYPE_PATH)/include \
                        $(FREETYPE_PATH)/include/freetype \
                        $(FREETYPE_PATH)/include/freetype/config \
                        $(FREETYPE_PATH)/include/freetype/internal \
			$(FTGLES_PATH)/ftgles \
			$(FTGLES_SRC_PATH)/ \
			$(FTGLES_SRC_PATH)/iGLU-1.0.0/include

LOCAL_SRC_FILES := \
	$(FTGLES_SRC_PATH)/FTBuffer.cpp \
	$(FTGLES_SRC_PATH)/FTCharmap.cpp \
	$(FTGLES_SRC_PATH)/FTContour.cpp \
	$(FTGLES_SRC_PATH)/FTFace.cpp \
	$(FTGLES_SRC_PATH)/FTFont/FTFont.cpp \
	$(FTGLES_SRC_PATH)/FTFont/FTFontGlue.cpp \
	$(FTGLES_SRC_PATH)/FTFont/FTOutlineFont.cpp \
	$(FTGLES_SRC_PATH)/FTFont/FTPolygonFont.cpp \
	$(FTGLES_SRC_PATH)/FTFont/FTTextureFont.cpp \
	$(FTGLES_SRC_PATH)/FTGL/ftglesGlue.cpp \
	$(FTGLES_SRC_PATH)/FTGlyph/FTGlyph.cpp \
	$(FTGLES_SRC_PATH)/FTGlyph/FTGlyphGlue.cpp \
	$(FTGLES_SRC_PATH)/FTGlyph/FTOutlineGlyph.cpp \
	$(FTGLES_SRC_PATH)/FTGlyph/FTPolygonGlyph.cpp \
	$(FTGLES_SRC_PATH)/FTGlyph/FTTextureGlyph.cpp \
	$(FTGLES_SRC_PATH)/FTGlyphContainer.cpp \
	$(FTGLES_SRC_PATH)/FTLayout/FTLayout.cpp \
	$(FTGLES_SRC_PATH)/FTLayout/FTLayoutGlue.cpp \
	$(FTGLES_SRC_PATH)/FTLayout/FTSimpleLayout.cpp \
	$(FTGLES_SRC_PATH)/FTLibrary.cpp \
	$(FTGLES_SRC_PATH)/FTPoint.cpp \
	$(FTGLES_SRC_PATH)/FTSize.cpp \
	$(FTGLES_SRC_PATH)/FTVectoriser.cpp \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/dict.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/geom.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/memalloc.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/mesh.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/normal.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/priorityq.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/render.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/sweep.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/tess.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libtess/tessmono.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libutil/error.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libutil/glue.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libutil/project.c \
	$(FTGLES_SRC_PATH)/iGLU-1.0.0/libutil/registry.c

LOCAL_STATIC_LIBRARIES += freetype2-prebuilt

LOCAL_LDLIBS := -lGLESv1_CM -llog -ldl -lstdc++

include $(BUILD_SHARED_LIBRARY)