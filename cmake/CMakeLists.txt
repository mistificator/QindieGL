cmake_minimum_required(VERSION 3.10)

# Project name
project(QindieGL VERSION 1.0 LANGUAGES CXX)

# C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

# DirectX9 SDK path
# default path for this SDK https://www.microsoft.com/ru-ru/download/details.aspx?id=6812
set(DIRECTX9_SDK_PATH "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)" CACHE PATH "Path to DirectX9 SDK")

# Output paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)

# shared DLL
add_library(QindieGL SHARED)

# Output name
set_target_properties(QindieGL PROPERTIES OUTPUT_NAME "opengl32")

# Sources
target_sources(QindieGL PRIVATE
    # .cpp
    ../code/d3d_array.cpp
    ../code/d3d_blend.cpp
    ../code/d3d_clip.cpp
    ../code/d3d_combiners.cpp
    ../code/d3d_cpu_detect.cpp
    ../code/d3d_eval.cpp
    ../code/d3d_extension.cpp
    ../code/d3d_feedback.cpp
    ../code/d3d_get.cpp
    ../code/d3d_global.cpp
    ../code/d3d_immediate.cpp
    ../code/d3d_light.cpp
    ../code/d3d_lists.cpp
    ../code/d3d_material.cpp
    ../code/d3d_matrix.cpp
    ../code/d3d_matrix_stack.cpp
    ../code/d3d_misc.cpp
    ../code/d3d_object.cpp
    ../code/d3d_pixels.cpp
    ../code/d3d_state.cpp
    ../code/d3d_stencil.cpp
    ../code/d3d_texgen.cpp
    ../code/d3d_texture.cpp
    ../code/d3d_wrapper.cpp
    # msvc resource
    ../msvc/QindieGL.rc
)

# Headers
target_sources(QindieGL PRIVATE
    ../code/d3d_array.hpp
    ../code/d3d_combiners.hpp
    ../code/d3d_extension.hpp
    ../code/d3d_global.hpp
    ../code/d3d_immediate.hpp
    ../code/d3d_matrix_stack.hpp
    ../code/d3d_object.hpp
    ../code/d3d_pixels.hpp
    ../code/d3d_state.hpp
    ../code/d3d_texture.hpp
    ../code/d3d_utils.hpp
    ../code/d3d_wrapper.hpp
)

# includes
target_include_directories(QindieGL PRIVATE
    "${DIRECTX9_SDK_PATH}/Include"
	./
)

# libs
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
target_link_directories(QindieGL PRIVATE
    "${DIRECTX9_SDK_PATH}/Lib/x64"
)
else()
target_link_directories(QindieGL PRIVATE
    "${DIRECTX9_SDK_PATH}/Lib/x86"
)
endif()

# macro
target_compile_definitions(QindieGL PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    QINDIEGLSRC_EXPORTS
)

# macro for Debug
target_compile_definitions(QindieGL PRIVATE $<$<CONFIG:Debug>:_DEBUG>)
# macro for Release
target_compile_definitions(QindieGL PRIVATE $<$<CONFIG:Release>:NDEBUG>)

# msvc config
if(MSVC)
    # Уровень предупреждений (WarningLevel = Level4)
    target_compile_options(QindieGL PRIVATE
        $<$<CONFIG:Debug>:
            /W4
            /Od # no opt
            /MTd # static crt
        >
        $<$<CONFIG:Release>:
            /W4
            /O2 # max speed
            /Ob2 # all intrinsic
            /Ot # max speed
            /GL # whole program optimization
            /MT # static crt
        >
    )

    # def
    set_target_properties(QindieGL PROPERTIES
        LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/../msvc/opengl32.def\""
    )
endif()

# libs
target_link_libraries(QindieGL PRIVATE
    # DirectX
    d3dx9.lib
    dxerr.lib
    # Windows
    kernel32.lib
    user32.lib
    gdi32.lib
    winspool.lib
    comdlg32.lib
    advapi32.lib
    shell32.lib
    ole32.lib
    oleaut32.lib
    uuid.lib
    odbc32.lib
    odbccp32.lib
	# legacy
	legacy_stdio_definitions.lib
)
