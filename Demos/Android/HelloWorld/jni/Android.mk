# Get the local path and assign it so we can recall it
# after the modules have been installed.
LOCAL_PATH := $(call my-dir)
MY_LOCAL_PATH := $(LOCAL_PATH)

FTGLES_PATH := ../../../../../ftgles
FREETYPE_PATH := ../../../../../freetype2-android

# Install the freetype2 module
include $(FTGLES_PATH)/jni/module.mk
# include ~/android/android-ndk-r6/sources/cxx-stl/stlport/module.mk

# For some reason, LOCAL_PATH points to the build tool's core
# directory after the shared library is installed.

LOCAL_PATH := $(MY_LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := helloftgles-static

LOCAL_CFLAGS := -DANDROID_NDK \
		-DFT2_BUILD_LIBRARY=1

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include_all \
			$(FREETYPE_PATH)/include \
			$(FTGLES_PATH)/ftgles/ \
			$(FTGLES_PATH)/ftgles/src/ \
			$(FTGLES_PATH)/ftgles/src/iGLU-1.0.0/include

LOCAL_SRC_FILES := \
	app.cpp

LOCAL_STATIC_LIBRARIES += ftgles-prebuilt

LOCAL_LDLIBS := -lGLESv1_CM -llog -ldl

include $(BUILD_SHARED_LIBRARY)
