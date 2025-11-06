cmake_minimum_required(VERSION 3.10)

project(idtech3_mixup VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

add_library(idtech3_mixup_lib STATIC)

set_target_properties(idtech3_mixup_lib PROPERTIES OUTPUT_NAME "idtech3_mixup")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
set(ARCH "x64")
else()
set(ARCH "x86")
endif()

# Output paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${ARCH}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${ARCH}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${ARCH}/lib)

aux_source_directory(../extern/detours/src DETOURS_SOURCES)

target_sources(idtech3_mixup_lib PRIVATE
    h2_refgl.cpp
    hooking.cpp
    surface_sorting.cpp
    h2_refgl.h
    hooking.h
    surface_sorting.h
)

target_include_directories(idtech3_mixup_lib PRIVATE
    ../extern/detours/src
    ../extern/mINI
    ../extern/dx9sdk/include
    ../extern/dxut
    ../extern/remix
    ../extern
    ../code
)

target_compile_definitions(idtech3_mixup_lib PRIVATE
    WIN32
    _CONSOLE
)

target_compile_definitions(idtech3_mixup_lib PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

if(MSVC)
    target_compile_options(idtech3_mixup_lib PRIVATE /permissive-)

    target_compile_options(idtech3_mixup_lib PRIVATE
        /W3 # WarningLevel = Level3
        /sdl # SDLCheck = true
    )

    target_compile_options(idtech3_mixup_lib PRIVATE $<$<CONFIG:Debug>:
        /Od 
        /MTd # RuntimeLibrary = MultiThreadedDebug
        /Ob1 # InlineFunctionExpansion = OnlyExplicitInline
    >)

    target_compile_options(idtech3_mixup_lib PRIVATE $<$<CONFIG:Release>:
        /O2
        /Ob2 # InlineFunctionExpansion = AnySuitable
        /Ot # FavorSizeOrSpeed = Speed
        /GL # WholeProgramOptimization = true
        /MT # RuntimeLibrary = MultiThreaded
        /Oi # IntrinsicFunctions = true
        /Gy # FunctionLevelLinking = true 
    >)

    set_target_properties(idtech3_mixup_lib PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()

target_link_directories(idtech3_mixup_lib INTERFACE
    ../extern/detours/lib.X86
    $<TARGET_FILE_DIR:idtech3_mixup_lib>
)

message(STATUS "idtech3_mixup library configured. Output directory will be set based on configuration.")