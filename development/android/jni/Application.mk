
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
APP_STL := c++_static

NDK_TOOLCHAIN_VERSION := clang

# x86_64 is disabled for now, because it is not supported by the Google Play Games SDK
APP_ABI := armeabi-v7a arm64-v8a x86
#APP_ABI := armeabi-v7a

# Min SDK level
APP_PLATFORM=android-23

# Release options:
APP_CFLAGS += -w
APP_CFLAGS += -s
APP_CFLAGS += -O2
APP_CFLAGS += -fexpensive-optimizations

# Debug options:
#APP_CFLAGS += -g
#APP_CFLAGS += -Wall
#APP_CFLAGS += -fno-inline-functions
#APP_CFLAGS += -fno-omit-frame-pointer

# Global options:
APP_CPPFLAGS += -std=c++11
APP_CFLAGS += -fexceptions
APP_CFLAGS += -DGAME_OS_ANDROID
