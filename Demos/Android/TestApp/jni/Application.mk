APP_BUILD_SCRIPT := Android.mk
$(info $(APP_BUILD_SCRIPT))
# Defines the native projects that must be compiled. These names point to the libs
# defined in the Android.mk file.
APP_MODULES      := TestApp gnustl_static
# Can be set to release or debug. For optimization purpose.
APP_OPTIM        := debug


APP_ABI := armeabi armeabi-v7a
APP_PLATFORM := android-8
APP_STL := gnustl_static
APP_CPPFLAGS += -frtti