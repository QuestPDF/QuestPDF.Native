# configure CMAKE
cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc")
endif()

project(QuestPdfSkia)


# enable IPO to achieve better performance, reduce artifact size
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_IS_SUPPORTED)

if(IPO_IS_SUPPORTED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()


# include all code source files
file(GLOB_RECURSE SOURCES "src/*.cpp")


# select between running as application (main.cpp) or producing the library
add_library(QuestPdfSkia SHARED ${SOURCES})
#add_executable(QuestPdfSkia ${SOURCES})


# include Skia library code
set(SKIA_DIR ${SKIA_DIR})
include_directories(${SKIA_DIR}/)


# MacOS configuration
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    file(GLOB SKIA_LIBRARY "${SKIA_DIR}/out/release/*.a")
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    find_library(CORETEXT_LIBRARY CoreText)
    target_link_libraries(QuestPdfSkia PRIVATE ${SKIA_LIBRARY} ${CORETEXT_LIBRARY} ${COREGRAPHICS_LIBRARY} ${COREFOUNDATION_LIBRARY})
endif()


# Linux configuration
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    file(GLOB SKIA_LIBRARY "${SKIA_DIR}/out/release/*.a")
    target_link_libraries(QuestPdfSkia PRIVATE ${SKIA_LIBRARY})
endif()


# Windows configuration
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    file(GLOB SKIA_LIBRARY "${SKIA_DIR}/out/release/*.lib")
    target_link_libraries(QuestPdfSkia PRIVATE ${SKIA_LIBRARY})

    set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64")
    target_link_libraries(QuestPdfSkia PRIVATE "${WINDOWS_SDK_PATH}/fontsub.lib")
    target_link_libraries(QuestPdfSkia PRIVATE "${WINDOWS_SDK_PATH}/dwrite.lib")
endif()


# reduce output package size
if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_custom_command(TARGET QuestPdfSkia POST_BUILD COMMAND ${CMAKE_STRIP} -x $<TARGET_FILE:QuestPdfSkia>)
endif()
