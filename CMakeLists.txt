# CMake 最低要求版本
cmake_minimum_required(VERSION 3.27)
# 项目信息
project(skia_tutorial)

Message("")
Message( STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR} )
Message( STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR} )
Message( STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND} )
Message( STATUS "CMAKE_VERSION: " ${CMAKE_VERSION} )
Message( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )
Message( STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME} )
Message( STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE )
Message( STATUS "CMAKE_HOST_SYSTEM_NAME: " ${CMAKE_HOST_SYSTEM_NAME} )
Message( STATUS "CMAKE_HOST_SYSTEM_PROCESSOR: " ${CMAKE_HOST_SYSTEM_PROCESSOR} )


# Check if the compiler supports C++11
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(NOT COMPILER_SUPPORTS_CXX17)
    message(FATAL_ERROR "The compiler does not support C++17.")
endif()

# Check for atomic support
include(CheckCXXSourceRuns)
set(CMAKE_REQUIRED_FLAGS "-std=c++17")
check_cxx_source_runs("
    #include <atomic>
    int main() {
        std::atomic<int> testAtomic;
        return 0;
    }
" ATOMIC_SUPPORTED)

if(NOT ATOMIC_SUPPORTED)
    message(FATAL_ERROR "The compiler does not support atomic operations.")
endif()

# ------- Set Vars ------- #
# 获取OS信息
# 操作系统	公共定义	64位系统定义
# Windows	_WIN32	_WIN64
# macOS	__APPLE__	__LP64__
# Linux	__linux__	__LP64__
# Android	__ANDROID__	__LP64__

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__cplusplus=201703L -std=c++17 -Wall -fno-omit-frame-pointer -pthread -fexceptions -frtti")
# set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")
set(SKIA_SDK "${CMAKE_SOURCE_DIR}/third_party/SKIA_SDK/Skia-m116-47d3027-1-macos-Release-${CMAKE_HOST_SYSTEM_PROCESSOR}")
set(SKIA_SDK_LIB "${SKIA_SDK}/out/Release-macos-${CMAKE_HOST_SYSTEM_PROCESSOR}")
Message( STATUS "SKIA_SDK: ${SKIA_SDK}")
Message( STATUS "SKIA_SDK_LIB: ${SKIA_SDK_LIB}")

if (APPLE)
# Undefined symbols for architecture x86_64:此类错误一般出现依赖不正确的情况下。
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++ -framework OpenGL  -framework AppKit -framework Security -framework CoreFoundation -framework CoreVideo -framework CoreText  -framework CoreMedia -framework CoreHaptics -framework QuartzCore -framework CoreFoundation -framework VideoDecodeAcceleration -framework Cocoa -framework AudioToolbox -framework VideoToolbox -framework CoreAudio -framework ForceFeedback -framework Carbon -framework Metal -framework IOKit -framework GameController -framework CoreGraphics -framework OpenCL  ")
endif()
# ------- End Set Vars --- #


#[[include_directories(${SKIA_SDK}/include
        ${SKIA_SDK}/modules
        ${SKIA_SDK}/src
        ${SKIA_SDK}/third_party)]]
INCLUDE_DIRECTORIES(${SKIA_SDK})
INCLUDE_DIRECTORIES(src)
set(SK_GL=1)
set(SK_GANESH=1)
add_definitions(-DSK_GL=1)
add_definitions(-DSK_GANESH=1)
LINK_DIRECTORIES(${SKIA_SDK_LIB})


# ------- Executable ----- #
# add_executable(skia_tutorial src/sdl2/sdl2_test.cpp)
# add_executable(skia_tutorial src/sdl2/example.cpp)
# add_executable(skia_tutorial src/sdl2/example2.cpp)
# add_executable(skia_tutorial src/skia/hello_skia.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/hello_skia_sdl2.cpp)
# add_executable(skia_tutorial src/skia/canvas/hello_canvas.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/canvas/learn_canvas.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/drawable/circle_rect.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/drawable/circle_rect_shadow.cpp src/skia/file.cpp)

# ------- test ---------- #
# add_executable(skia_tutorial src/skia/test/make_background.cpp src/skia/file.cpp)

# ------- watermark ---------- #
# add_executable(skia_tutorial src/skia/watermark/water_mark_test.cpp src/skia/file.cpp)



# ------- percent ---------- #
# add_executable(skia_tutorial src/skia/50_percent/50_percent_gray.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/50_percent/50_percent_srgb.cpp src/skia/file.cpp)

# ------- Alpha ---------- #
# add_executable(skia_tutorial src/skia/alpha/Alpha_Constants_a.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/alpha/Alpha_Constants_b.cpp src/skia/file.cpp)

# ------- Bitmap ---------- #
# add_executable(skia_tutorial src/skia/bitmap/bitmap_012.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/bitmap/bitmap_computeisopaque.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/bitmap/Bitmap_HeapAllocator_allocPixelRef.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/bitmap/Bitmap_allocN32Pixels.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/bitmap/Bitmap_allocPixels.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/bitmap/Bitmap_allocPixelsFlags.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/bitmap/Bitmap_allocPixels_2.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/bitmap/Bitmap_allocPixels_3.cpp src/skia/file.cpp)
# add_executable(skia_tutorial src/skia/bitmap/Bitmap_allocPixels_4.cpp src/skia/file.cpp)


# ------- Text ---------- #
add_executable(skia_tutorial src/skia/text/text_rendering.cpp src/skia/file.cpp)
# ------- End Executable - #

# ------- Finds ---------- #
FIND_PACKAGE(SDL2 REQUIRED)
Message("")
Message( STATUS "FINDING SDL2" )
IF (${SDL2_FOUND})
    Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message( STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
    Message( STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
ELSE()
    Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message( FATAL_ERROR "SDL2 NOT FOUND" )
ENDIF()

FIND_PACKAGE(SDL2_IMAGE)
Message("")
Message( STATUS "FINDING SDL2_IMAGE" )
Message( STATUS "SDL2_IMAGE_FOUND: " ${SDL2_IMAGE_FOUND})
IF (${SDL2_IMAGE_FOUND})
    Message( STATUS "SDL2_IMAGE_INCLUDE_DIR: " ${SDL2_IMAGE_INCLUDE_DIR})
    Message( STATUS "SDL2_IMAGE_LIBRARY: " ${SDL2_IMAGE_LIBRARY})
ELSE()
    Message( FATAL_ERROR "SDL2_IMAGE NOT FOUND" )
ENDIF()

FIND_PACKAGE(SDL2TTF)
Message("")
Message( STATUS "FINDING SDL2TTF" )
Message( STATUS "SDL2TTF_FOUND: " ${SDL2TTF_FOUND} )
IF (${SDL2TTF_FOUND})
    Message( STATUS "SDL2TTF_INCLUDE_DIR: " ${SDL2TTF_INCLUDE_DIR})
    Message( STATUS "SDL2TTF_LIBRARY: " ${SDL2TTF_LIBRARY})
ELSE()
    Message( FATAL_ERROR "SDL2TTF NOT FOUND" )
ENDIF()
Message("")

FIND_PACKAGE(SDL2Mixer)

Message( STATUS "FINDING SDL2Mixer" )
Message( STATUS "SDL2Mixer_FOUND: " ${SDL2Mixer_FOUND} )

Message("")

file(COPY resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# ------- End Finds ------ #

# ------- Inc & Link ---- #
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2Mixer_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(skia_tutorial
        ${SKIA_SDK_LIB}/libdng_sdk.a
        ${SKIA_SDK_LIB}/libexpat.a
        ${SKIA_SDK_LIB}/libharfbuzz.a
        ${SKIA_SDK_LIB}/libicu.a
        ${SKIA_SDK_LIB}/libjpeg.a
        ${SKIA_SDK_LIB}/libpiex.a
        ${SKIA_SDK_LIB}/libpng.a
        ${SKIA_SDK_LIB}/libskcms.a
        ${SKIA_SDK_LIB}/libskia.a
        ${SKIA_SDK_LIB}/libskottie.a
        ${SKIA_SDK_LIB}/libskparagraph.a
        ${SKIA_SDK_LIB}/libskresources.a
        ${SKIA_SDK_LIB}/libsksg.a
        ${SKIA_SDK_LIB}/libskshaper.a
        ${SKIA_SDK_LIB}/libskunicode.a
        # ${SKIA_SDK_LIB}/libspvtools.a
        # ${SKIA_SDK_LIB}/libspvtools_val.a
        ${SKIA_SDK_LIB}/libsvg.a
        ${SKIA_SDK_LIB}/libwebp.a
        ${SKIA_SDK_LIB}/libwebp_sse41.a
        ${SKIA_SDK_LIB}/libwuffs.a
        ${SKIA_SDK_LIB}/libzlib.a
        ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2Mixer_LIBRARY}
)
# ------- End ----------- #

