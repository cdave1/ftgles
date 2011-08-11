LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ftgles-prebuilt
LOCAL_SRC_FILES := ../libs/$(TARGET_ARCH_ABI)/libftgles.so
include $(PREBUILT_SHARED_LIBRARY)