cmake_minimum_required(VERSION 3.20)

# Enable modern MSVC runtime library selection (MultiThreaded / MultiThreadedDebug)
cmake_policy(SET CMP0091 NEW)

# Set the single source of truth for project version (Major.Minor.Patch.Tweak)
project(QindieGL VERSION 1.2.2.999 LANGUAGES C CXX RC)

# Fallback to 0 if fewer than 4 version components are specified in project()
if(NOT DEFINED PROJECT_VERSION_PATCH OR "${PROJECT_VERSION_PATCH}" STREQUAL "")
    set(PROJECT_VERSION_PATCH 0)
endif()
if(NOT DEFINED PROJECT_VERSION_TWEAK OR "${PROJECT_VERSION_TWEAK}" STREQUAL "")
    set(PROJECT_VERSION_TWEAK 0)
endif()

message(STATUS "QindieGL version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")

# Ensure build runs on Windows with MSVC-compatible toolchain
if(NOT WIN32 OR NOT MSVC)
    message(FATAL_ERROR "QindieGL requires Windows and the MSVC toolchain.")
endif()

# -----------------------------------------------------------------------------
# Build Options
# -----------------------------------------------------------------------------
option(QINDIEGL_NO_REMIX_MODS "Build without RTX Remix modifications (equivalent to ReleaseNoRemixMods)" OFF)
option(QINDIEGL_SHADERS_FROM_SOURCE "Embed raw HLSL shaders instead of precompiled CSO bytecode" OFF)

# -----------------------------------------------------------------------------
# Architecture Detection (x86 vs x64)
# -----------------------------------------------------------------------------
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(QINDIEGL_ARCH "x64")
    set(DETOURS_ARCH "X64")
    set(QINDIEGL_BIN_SUFFIX ".x64")
else()
    set(QINDIEGL_ARCH "x86")
    set(DETOURS_ARCH "X86")
    set(QINDIEGL_BIN_SUFFIX "")
endif()

message(STATUS "Configuring QindieGL for architecture: ${QINDIEGL_ARCH}")

# -----------------------------------------------------------------------------
# Global Compiler & Runtime Settings (matching QindieGL-src.vcxproj)
# -----------------------------------------------------------------------------
# Use static CRT (/MT for Release, /MTd for Debug)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Use Multi-Byte Character Set (disable UNICODE)
remove_definitions(-DUNICODE -D_UNICODE)
add_compile_definitions(_MBCS WIN32 _WINDOWS)

# Common MSVC compiler flags: Level 4 warnings, standard conformance, Release optimizations
# Note: semicolon-separated list inside generator expression prevents CMake whitespace splitting
add_compile_options(
    /W4
    /permissive-
    "$<$<NOT:$<CONFIG:Debug>>:/O2;/Ob2;/Oi;/Ot;/Gy;/GL>"
)

# Only pass /MP on MSBuild/Visual Studio generators (Ninja parallelizes builds itself and passes /showIncludes)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    add_compile_options(/MP)
endif()

# Shared include directories for QindieGL and idtech3_mixup
set(SHADER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/shaders")
set(SHADER_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

set(QINDIEGL_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/code"
    "${SHADER_SRC_DIR}"
    "${SHADER_BIN_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/detours/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/dx9sdk/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/dxut"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/remix"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/fnvhash"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/mINI"
    "${CMAKE_CURRENT_SOURCE_DIR}/idtech3_mixup"
)

# -----------------------------------------------------------------------------
# 1. Dependency: Microsoft Detours (Static Library)
# -----------------------------------------------------------------------------
set(DETOURS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern/detours/src")
if(EXISTS "${DETOURS_DIR}/detours.cpp")
    # Note: uimports.cpp is included by creatwth.cpp and must NOT be compiled standalone
    add_library(Detours STATIC
        "${DETOURS_DIR}/creatwth.cpp"
        "${DETOURS_DIR}/detours.cpp"
        "${DETOURS_DIR}/disasm.cpp"
        "${DETOURS_DIR}/disolarm.cpp"
        "${DETOURS_DIR}/disolarm64.cpp"
        "${DETOURS_DIR}/disolia64.cpp"
        "${DETOURS_DIR}/disolx64.cpp"
        "${DETOURS_DIR}/disolx86.cpp"
        "${DETOURS_DIR}/image.cpp"
        "${DETOURS_DIR}/modules.cpp"
    )
    target_include_directories(Detours PUBLIC "${DETOURS_DIR}")
    set_target_properties(Detours PROPERTIES
        OUTPUT_NAME "detours"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib/${QINDIEGL_ARCH}"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/lib/${QINDIEGL_ARCH}/Debug"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/lib/${QINDIEGL_ARCH}/Release"
    )
endif()

# -----------------------------------------------------------------------------
# 2. Dependency: idtech3_mixup (Static Library)
# -----------------------------------------------------------------------------
set(IDTECH3_MIXUP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/idtech3_mixup")
if(NOT QINDIEGL_NO_REMIX_MODS AND EXISTS "${IDTECH3_MIXUP_DIR}")
    file(GLOB IDTECH3_MIXUP_SOURCES CONFIGURE_DEPENDS
        "${IDTECH3_MIXUP_DIR}/*.c"
        "${IDTECH3_MIXUP_DIR}/*.cpp"
        "${IDTECH3_MIXUP_DIR}/*.h"
        "${IDTECH3_MIXUP_DIR}/*.hpp"
    )
    add_library(idtech3_mixup STATIC ${IDTECH3_MIXUP_SOURCES})
    target_include_directories(idtech3_mixup PUBLIC ${QINDIEGL_INCLUDE_DIRS})
    if(TARGET Detours)
        target_link_libraries(idtech3_mixup PUBLIC Detours)
    endif()
    # Output into the legacy path expected by any #pragma comment(lib, ...) directives
    set_target_properties(idtech3_mixup PROPERTIES
        OUTPUT_NAME "idtech3_mixup"
        ARCHIVE_OUTPUT_DIRECTORY "${IDTECH3_MIXUP_DIR}/lib.${QINDIEGL_ARCH}/$<CONFIG>"
    )
endif()

# -----------------------------------------------------------------------------
# 3. HLSL Shader Compilation (ortho_vs.hlsl & ortho_ps.hlsl -> .cso)
# -----------------------------------------------------------------------------
if(NOT QINDIEGL_SHADERS_FROM_SOURCE)
    find_program(FXC_EXECUTABLE fxc
        HINTS
            "$ENV{WindowsSdkVerBinPath}/${QINDIEGL_ARCH}"
            "$ENV{WindowsSdkDir}/bin/$ENV{WindowsSDKVersion}/${QINDIEGL_ARCH}"
            "C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/${QINDIEGL_ARCH}"
            "C:/Program Files (x86)/Windows Kits/10/bin/10.0.22621.0/${QINDIEGL_ARCH}"
            "C:/Program Files (x86)/Windows Kits/10/bin/10.0.19041.0/${QINDIEGL_ARCH}"
            "${CMAKE_CURRENT_SOURCE_DIR}/extern/dx9sdk/Utilities/bin/${QINDIEGL_ARCH}"
        REQUIRED
    )

    set(ORTHO_VS_CSO "${SHADER_BIN_DIR}/ortho_vs.cso")
    set(ORTHO_PS_CSO "${SHADER_BIN_DIR}/ortho_ps.cso")

    # COMMAND_EXPAND_LISTS ensures empty generator expressions (in Debug) are omitted instead of passing ""
    add_custom_command(
        OUTPUT "${ORTHO_VS_CSO}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${SHADER_BIN_DIR}"
        COMMAND "${FXC_EXECUTABLE}" /nologo /T vs_2_0 /E main /WX "$<$<NOT:$<CONFIG:Debug>>:/Zpc>"
                /Fo "${ORTHO_VS_CSO}" "${SHADER_SRC_DIR}/ortho_vs.hlsl"
        MAIN_DEPENDENCY "${SHADER_SRC_DIR}/ortho_vs.hlsl"
        COMMENT "Compiling vertex shader ortho_vs.hlsl -> ortho_vs.cso"
        VERBATIM
        COMMAND_EXPAND_LISTS
    )

    add_custom_command(
        OUTPUT "${ORTHO_PS_CSO}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${SHADER_BIN_DIR}"
        COMMAND "${FXC_EXECUTABLE}" /nologo /T ps_2_0 /E main /WX "$<$<NOT:$<CONFIG:Debug>>:/Zpc>"
                /Fo "${ORTHO_PS_CSO}" "${SHADER_SRC_DIR}/ortho_ps.hlsl"
        MAIN_DEPENDENCY "${SHADER_SRC_DIR}/ortho_ps.hlsl"
        COMMENT "Compiling pixel shader ortho_ps.hlsl -> ortho_ps.cso"
        VERBATIM
        COMMAND_EXPAND_LISTS
    )

    # Custom target ensures Ninja and Visual Studio emit rules for the .cso outputs
    add_custom_target(QindieGL_Shaders DEPENDS "${ORTHO_VS_CSO}" "${ORTHO_PS_CSO}")

    # Ensure QindieGL.rc is compiled only after both .cso files are generated
    set_property(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/msvc/QindieGL.rc"
        APPEND PROPERTY OBJECT_DEPENDS "${ORTHO_VS_CSO}" "${ORTHO_PS_CSO}"
    )
endif()

# -----------------------------------------------------------------------------
# 4. Main Target: QindieGL (opengl32.dll)
# -----------------------------------------------------------------------------
set(QINDIEGL_SOURCES
    # Core wrapper sources
    code/d3d_array.cpp
    code/d3d_blend.cpp
    code/d3d_clip.cpp
    code/d3d_combiners.cpp
    code/d3d_cpu_detect.cpp
    code/d3d_eval.cpp
    code/d3d_extension.cpp
    code/d3d_feedback.cpp
    code/d3d_get.cpp
    code/d3d_global.cpp
    code/d3d_helpers.cpp
    code/d3d_immediate.cpp
    code/d3d_light.cpp
    code/d3d_lists.cpp
    code/d3d_material.cpp
    code/d3d_matrix.cpp
    code/d3d_matrix_detection.cpp
    code/d3d_matrix_stack.cpp
    code/d3d_misc.cpp
    code/d3d_object.cpp
    code/d3d_pixels.cpp
    code/d3d_state.cpp
    code/d3d_stencil.cpp
    code/d3d_texgen.cpp
    code/d3d_texture.cpp
    code/d3d_wrapper.cpp
    code/rmx_gen.cpp
    code/rmx_light.cpp
    code/win_imgui.cpp

    # Bundled external sources
    extern/dxut/dxerr_a.cpp
    extern/dxut/dxerr_w.cpp
    extern/fnvhash/hash_32a.c
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/backends/imgui_impl_dx9.cpp
    extern/imgui/backends/imgui_impl_win32.cpp

    # Windows resources and module definition file (.def)
    msvc/QindieGL.rc
    msvc/opengl32.def
)

add_library(QindieGL SHARED ${QINDIEGL_SOURCES})

if(TARGET QindieGL_Shaders)
    add_dependencies(QindieGL QindieGL_Shaders)
endif()

# Target must be named "opengl32" to match "LIBRARY opengl32" in opengl32.def
# WINDOWS_EXPORT_ALL_SYMBOLS must be OFF so CMake does not override opengl32.def aliases
set_target_properties(QindieGL PROPERTIES
    OUTPUT_NAME "opengl32"
    PREFIX ""
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)

if(QINDIEGL_SHADERS_FROM_SOURCE)
    set(SHADERS_FROM_SOURCE_VAL 1)
else()
    set(SHADERS_FROM_SOURCE_VAL 0)
endif()

# QINDIEGLSRC_EXPORTS is required so OPENGL_API expands to __declspec(dllexport),
# allowing link.exe on x86 to match undecorated aliases in opengl32.def to __stdcall symbols
target_compile_definitions(QindieGL PRIVATE
    _USRDLL
    QINDIEGLSRC_EXPORTS
    SHADERS_FROM_SOURCE=${SHADERS_FROM_SOURCE_VAL}
    QINDIEGL_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    QINDIEGL_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    QINDIEGL_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    QINDIEGL_VERSION_TWEAK=${PROJECT_VERSION_TWEAK}
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
    $<$<BOOL:${QINDIEGL_NO_REMIX_MODS}>:QINDIEGLSRC_NO_REMIX>
)

# CMake automatically forwards target_include_directories to both cl.exe and rc.exe
target_include_directories(QindieGL PRIVATE ${QINDIEGL_INCLUDE_DIRS})

# DirectX 9 SDK and internal library search paths
target_link_directories(QindieGL PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/dx9sdk/lib/${QINDIEGL_ARCH}"
    "${CMAKE_CURRENT_SOURCE_DIR}/idtech3_mixup/lib.${QINDIEGL_ARCH}/$<CONFIG>"
    "${CMAKE_CURRENT_BINARY_DIR}/lib/${QINDIEGL_ARCH}/$<CONFIG>"
    "${CMAKE_CURRENT_BINARY_DIR}/lib/${QINDIEGL_ARCH}"
)

target_link_libraries(QindieGL PRIVATE
    d3dx9
    legacy_stdio_definitions
    winmm
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
)

if(TARGET idtech3_mixup)
    target_link_libraries(QindieGL PRIVATE idtech3_mixup)
endif()
if(TARGET Detours)
    target_link_libraries(QindieGL PRIVATE Detours)
endif()

# Linker flags: Windows subsystem, PDB generation, LTCG/COMDAT in Release, SafeSEH disabled on x86
target_link_options(QindieGL PRIVATE
    /SUBSYSTEM:WINDOWS
    /DEBUG
    $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>:/SAFESEH:NO>
    "$<$<NOT:$<CONFIG:Debug>>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/LTCG>"
)

# -----------------------------------------------------------------------------
# 5. Post-Build Step: Populate bin/<Config>[.x64] (matching vcxproj PostBuildEvent)
# -----------------------------------------------------------------------------
set(QINDIEGL_DIST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/$<CONFIG>${QINDIEGL_BIN_SUFFIX}")

add_custom_command(TARGET QindieGL POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${QINDIEGL_DIST_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:QindieGL>"
            "${QINDIEGL_DIST_DIR}/$<TARGET_FILE_NAME:QindieGL>"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_PDB_FILE:QindieGL>"
            "${QINDIEGL_DIST_DIR}/$<TARGET_PDB_FILE_NAME:QindieGL>"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/msvc/QindieGL.ini"
            "${QINDIEGL_DIST_DIR}/QindieGL.ini"
    COMMENT "Copying opengl32.dll, opengl32.pdb, and QindieGL.ini to ${QINDIEGL_DIST_DIR}"
    VERBATIM
)
