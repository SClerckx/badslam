cmake_minimum_required(VERSION 3.6...3.14)
project(libvis LANGUAGES C CXX)

# Enable C++14 globally
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Support IDEs: https://cliutils.gitlab.io/modern-cmake/chapters/features/ides.html
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake-default-targets")

if(MSVC)
  # Fixes DLL generation with MSVC.
  # See http://gernotklingler.com/blog/creating-using-shared-libraries-different-compilers-different-operating-systems/
  # and also https://stackoverflow.com/questions/37128555/getting-cmake-to-build-shared-library-for-msvc
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols in Visual Studio")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  #set(CMake_MSVC_PARALLEL)
  #message ("Enabling parallel builds in visual studio")
endif()

# Make CMake find the Find<Package>.cmake files in the cmake subdirectory.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(CheckLanguage)

set(CMAKE_PREFIX_PATH "D:/Bladesense")
#list(APPEND CMAKE_PREFIX_PATH "D:/Bladesense/glew-2.1.0-win32")
list(APPEND CMAKE_PREFIX_PATH "D:/Bladesense/vcpkg/installed")
list(APPEND VCPKG_FEATURE_FLAGS "versions")

################################################################################
# Toolchain
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "D:/Bladesense/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

################################################################################
#Bigobj
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

message(STATUS "Target name: ${my_target_name}")


################################################################################
# Dependencies.

# CUDA (external)
# NOTE: The user of this CMake file should set the CUDA arch when invoking
#       CMake, for example with: -DCMAKE_CUDA_FLAGS="-arch=sm_61"
#       Common settings are either the 'native' arch for which the binary will
#       be known to run on exclusively, or a broader set of PTX architectures
#       in case the binary is intended for distribution.
check_language(CUDA)
set(CUDATOOLKITDIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.1")
set(CMAKE_CUDA_COMPILER "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.1/bin/nvcc.exe")
if (CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
  
  # Enable C++14 globally for CUDA code
  if(NOT DEFINED CMAKE_CUDA_STANDARD)
    set(CMAKE_CUDA_STANDARD 14)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
  endif()
  
  # It is very important that CUDA's nvcc uses exactly the same gcc version and
  # flags as is used for other C++ code. Otherwise, it can happen that classes
  # have different layouts as seen by code compiled by nvcc's gcc and by the
  # other used gcc, which leads to memory-corruption type bugs. See e.g.:
  # https://devtalk.nvidia.com/default/topic/1024953/linking-problem-due-to-different-size-of-std-string/
  # ccbin must be set to the full path, otherwise there is the possibility that
  # nvcc searches in the wrong location first according to the linked forum
  # posts above.
  if (NOT MSVC)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -ccbin=${CMAKE_CXX_COMPILER}")
  else()
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_75,code=sm_75")
    #set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS}")
    #set(CMAKE_CUDA_FLAGS "-arch=sm_75")
    message (STATUS "CMAKE_CUDA_FLAGS: ${CMAKE_CUDA_FLAGS}")
    set(CUDA_cublas_LIBRARY "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.1/lib/x64/cublas.lib")
    message (STATUS "CUDA_cublas_LIBRARY: ${CUDA_cublas_LIBRARY}")
  endif()
else()
  message(FATAL_ERROR "CMake did not find CUDA, which is required to build this program.")
endif()

# Cross-platform threading. See:
# https://cmake.org/cmake/help/latest/module/FindThreads.html
find_package(Threads REQUIRED)

find_package(OpenGL REQUIRED)

# Eigen (external)
#set(Eigen3_DIR "D:/Bladesense/Eigen/eigen-3.3.9/eigen-3.3.9/out/build/x64-Debug")
set(Eigen3_DIR "D:/Bladesense/vcpkg/installed/x64-windows/share/eigen3")
#list(APPEND CMAKE_PREFIX_PATH "D:/Bladesense/Eigen/eigen-3.3.9/eigen-3.3.9")
find_package(Eigen3 REQUIRED NO_MODULE)

#list(APPEND CMAKE_PREFIX_PATH "D:/Bladesense/Eigen/eigen-3.3.9")
#find_package(Eigen3 3.3.9 REQUIRED NO_MODULE)

#set(EIGEN3_INCLUDE_DIR "D:/Bladesense/Eigen/eigen-3.3.9")
#include_directories(${EIGEN3_INCLUDE_DIR})

#set(Boost_INCLUDE_DIR "D:/Bladesense/boost_1_81_0")
#set(BOOST_ROOT "D:/Bladesense/boost_1_81_0")

#set(BOOST_ROOT "D:/Bladesense/boost_1_69_0")
#set(BOOST_LIBRARYDIR "D:/Bladesense/boost_1_54_0_precompiled/lib64-msvc-11.0")
#set(BOOST_INCLUDE_DIRS "D:/Bladesense/boost_1_54_0_precompiled/boost")
#set(Boost_COMPILER "-vc141")

#set(Boost_NO_SYSTEM_PATHS ON) #Forces only looking in boost root
#set(Boost_NO_BOOST_CMAKE  ON) # additional set this one
#set(Boost_USE_STATIC_LIBS ON)

#Print compiler version in vcxxx format
#set(BOOST_ROOT "D:/Bladesense/boost_1_54_0")
#set(BOOST_INCLUDEDIR "${BOOST_ROOT}/boost")
#set(BOOST_LIBRARYDIR "D:/Bladesense/boost_1_54_0/lib64-msvc-11.0")

#set(Boost_SERIALIZATION_LIBRARY "${BOOST_LIBRARYDIR}/boost_serialization-vc110-mt-1_54.lib")
#set(Boost_FILESYSTEM_LIBRARY "${BOOST_LIBRARYDIR}/boost_filesystem-vc110-mt-1_54.lib")
#set(Boost_SYSTEM_LIBRARY "${BOOST_LIBRARYDIR}/boost_system-vc110-mt-1_54.lib")
# Boost (external)
#if (POLICY CMP0074) # get rid of annoying warning
  #cmake_policy(SET CMP0074 NEW)
#endif()

set(BOOST_ROOT "D:/Bladesense/boost_1_73_0")

#set(Boost_DEBUG ON)
#set(Boost_COMPILER "-c143")
#set(BOOST_COMPILER "-vc143")
#set(_boost_COMPILER "-vc143")
#set(Boost_USE_STATIC_LIBS ON)
message(STATUS "BOOST_COMPILER ${BOOST_COMPILER}")
message(STATUS "MSVC_TOOLSET_VERSION ${MSVC_TOOLSET_VERSION}")

find_package(Boost 1.73 REQUIRED COMPONENTS serialization filesystem system) 

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#set(ZLIB_LIBRARY = "D:/Bladesense/zlib1213/zlib-1.2.13")
#set(ZLIB_INCLUDE_DIR = "D:/Bladesense/zlib1213/zlib-1.2.13/out/build/x64-Debug")
# Loguru (packaged)
add_library(loguru SHARED
  libvis/third_party/loguru/loguru.cpp
)
set_property(TARGET loguru PROPERTY FOLDER "libvis/third_party")
target_include_directories(loguru PUBLIC
  libvis/third_party/loguru/
)
set_target_properties(loguru PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)
target_compile_definitions(loguru PRIVATE
  LOGURU_WITH_STREAMS
)
if(MSVC)
  set(LOGURU_DLL_EXPORT "__declspec(dllexport)")
  #set(LOGURU_DLL_IMPORT "__declspec(dllimport)")
endif()
target_compile_definitions(loguru PRIVATE
  LOGURU_EXPORT=${LOGURU_DLL_EXPORT}
)
target_link_libraries(loguru
  Threads::Threads
  ${CMAKE_DL_LIBS}
)

# GTest (packaged)
add_subdirectory(libvis/third_party/gtest)
set_target_properties(gtest gtest_main PROPERTIES FOLDER "libvis/third_party")

if(MSVC)
  if (MSVC_VERSION GREATER_EQUAL 1900)
    target_compile_definitions(gtest PUBLIC _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
    target_compile_definitions(gtest_main PUBLIC _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
  endif()
endif()
enable_testing()

# libpng (packaged)
add_subdirectory(libvis/third_party/libpng)
set_target_properties(png png_static genfiles PROPERTIES FOLDER "libvis/third_party/png")

#set(GLEW_INCLUDE_DIR "D:/Bladesense/glew-2.1.0-win32/glew-2.1.0/include")
#set(GLEW_LIBRARY "D:/Bladesense/glew-2.1.0-win32/glew-2.1.0/lib/Release/x64/glew32.lib") # Assuming you're using the release version and x64 architecture
#set(GLEW_LIBRARIES ${GLEW_LIBRARY})

#include_directories(${GLEW_INCLUDE_DIR})

#target_link_libraries(badslam ${GLEW_LIBRARIES})


# GLEW (external, required)
#set(GLEW_INCLUDE_DIRS = "D:/Bladesense/glew-2.1.0-win32/glew-2.1.0/bin/Release/x64")
#https://stackoverflow.com/questions/34230853/how-to-fix-could-not-find-glew-missing-glew-include-dir-glew-library
#set(GLEW_INCLUDE_DIR =  "D:/Bladesense/glew-2.1.0-win32/glew-2.1.0/include")
#set(GLEW_LIBRARY =  "D:/Bladesense/glew-2.1.0-win32/glew-2.1.0/lib")
#set(GLEW_LIBRARIES = "D:/Bladesense/glew-2.1.0-win32/glew-2.1.0")
#set(GLEW_LIBRARY "D:/Bladesense/glew-2.1.0-win32/glew-2.1.0/lib/Release/x64/glew32.lib") # Assuming you're using the release version and x64 architecture
#set(GLEW_LIBRARIES ${GLEW_LIBRARY})
set(GLEW_DIR "D:/Bladesense/vcpkg/installed/x64-windows/share/glew")
find_package(GLEW REQUIRED) #was find_package(GLEW REQUIRED)

# Vulkan (external, optional)
set(VULKAN_PATH "" CACHE PATH "Set this to the path of the Vulkan SDK installation (the directory containing include and lib) if installed locally, otherwise leave this empty.")
if(NOT (VULKAN_PATH STREQUAL ""))
  set(VULKAN_FOUND TRUE)
else()
  set(VULKAN_FOUND FALSE)
endif()

# Qt5 (external, required)
# TODO: Qt should be an optional dependency but is required currently.
# Find includes in corresponding build directories.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Instruct CMake to run rcc (resource compiler) automatically when needed.
set(CMAKE_AUTORCC ON)

#find_package(Qt5 5.8 REQUIRED COMPONENTS Core OpenGL Widgets)
#find_package(Qt5 5.8 REQUIRED COMPONENTS Core OpenGL Widgets)
set(Qt5_DIR "D:/Bladesense/vcpkg/installed/x64-windows/share/cmake/Qt5")
find_package(Qt5 5.8 REQUIRED COMPONENTS Core OpenGL Widgets)

if (UNIX AND NOT APPLE)
  find_package(Qt5X11Extras REQUIRED)
endif()
if (TARGET Qt5::Core AND UNIX)
  # HACK, see: https://gitlab.kitware.com/cmake/cmake/issues/16915
  get_property( core_options TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_OPTIONS )
  string( REPLACE "-fPIC" "" new_core_options "${core_options}" )
  set_property( TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_OPTIONS ${new_core_options} )
  set_property( TARGET Qt5::Core PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE "ON" )
  set( CMAKE_CXX_COMPILE_OPTIONS_PIE "-fPIC" )
endif()

# OpenCV (external, required)
#find_package(OpenCV 3.1.0 REQUIRED)
set(OpenCV_DIR "D:/Bladesense/vcpkg/installed/x64-windows/share/opencv3")
find_package(OpenCV 3 REQUIRED)

# Settings.
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUXX)
  set(LIBVIS_WARNING_OPTIONS "$<$<COMPILE_LANGUAGE:CXX>:-Wall>")
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:-Wextra>")
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:-O2>")
  if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:-msse2>")
    set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:-msse3>")
  endif()
  
  # TODO: It seems gcc cannot disable this locally, therefore added it here as a
  #       workaround.
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:-Wno-unknown-pragmas>")
  
  # NOTE: Disabled this since it triggered in a lot of places, including external headers, creating a lot of warning spam.
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:-Wno-sign-compare>")
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:-Wno-missing-field-initializers>")
elseif(MSVC)
  #  set(LIBVIS_WARNING_OPTIONS "$<$<COMPILE_LANGUAGE:CXX>:/O2")
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:/MP>") #EDITED: REMOVED A SPACE HERE AFTER MP
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:/W4>")
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:/wd4068>")
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:/wd4127>") # https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4127?view=vs-2019
  
  # NOTE: Disabled this since it triggered in a lot of places, including external headers, creating a lot of warning spam.
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:/wd4244>")
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:/wd4267>")
  set(LIBVIS_WARNING_OPTIONS "${LIBVIS_WARNING_OPTIONS};$<$<COMPILE_LANGUAGE:CXX>:/wd4389>")
  #ADD_DEFINITIONS("-D _USE_MATH_DEFINES /bigobj /wd4305 /wd4244 /MP")
  #target_compile_definitions(libvis PUBLIC _USE_MATH_DEFINES)
endif()


################################################################################
# Helper application bin2c required for building libvis.

add_executable(bin2c
  ./libvis/src/bin2c/main.cc
)
set_property(TARGET bin2c PROPERTY FOLDER "libvis")

target_include_directories(bin2c PRIVATE
  ${Boost_INCLUDE_DIR}
)

target_compile_options(bin2c PRIVATE
  "${LIBVIS_WARNING_OPTIONS}"
)
target_link_libraries(bin2c
  ${Boost_LIBRARIES}
)


################################################################################
# Function for generating headers with SPIR-V bytecode from GLSL code.

# Sets up compile and header-conversion commands for Vulkan shaders. The
# resulting header file paths are appended to the variable whose name is passed
# in for _GENERATED_HEADERS. These headers must be specified in a target to
# trigger the shader build steps.
# NOTE: _GENERATED_HEADERS must differ from the variable name being passed in,
#       otherwise the dereferencing returns the wrong variable.
function(add_vulkan_shader TARGETNAME INPUT_FILEPATH _GENERATED_HEADERS)
  get_filename_component(INPUT_FILENAME ${INPUT_FILEPATH} NAME)
  
  # Vertex shader.
  add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.vert.h
    COMMAND glslangValidator -V -S vert -o ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.vert ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILEPATH}.vert
    COMMAND bin2c -n vis -H ${CMAKE_CURRENT_SOURCE_DIR}/libvis/src/libvis/shaders/license_header.h ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.vert
    DEPENDS bin2c ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILEPATH}.vert
    )
  
  # Fragment shader.
  add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.frag.h
    COMMAND glslangValidator -V -S frag -o ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.frag ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILEPATH}.frag
    COMMAND bin2c -n vis -H ${CMAKE_CURRENT_SOURCE_DIR}/libvis/src/libvis/shaders/license_header.h ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.frag
    DEPENDS bin2c ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILEPATH}.frag
    )
  
  # Return generated headers in the variable named ${_GENERATED_HEADERS} in the
  # parent scope.
  set(${_GENERATED_HEADERS}
    ${${_GENERATED_HEADERS}}
    ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.vert.h
    ${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILENAME}.frag.h
    PARENT_SCOPE
  )
endfunction()


################################################################################
# libvis.

if(VULKAN_FOUND)
  add_vulkan_shader(libvis
    libvis/src/libvis/shaders/shader
    GENERATED_HEADERS
  )
endif()

set(LIBVIS_FILES
  libvis/src/libvis/camera.h
  libvis/src/libvis/camera_frustum_opengl.h
  libvis/src/libvis/command_line_parser.cc
  libvis/src/libvis/command_line_parser.h
  libvis/src/libvis/dlt.h
  libvis/src/libvis/eigen.h
  libvis/src/libvis/geometry.h
  libvis/src/libvis/glew.cc
  libvis/src/libvis/glew.h
  libvis/src/libvis/image.cc
  libvis/src/libvis/image.h
  libvis/src/libvis/image_cache.h
  libvis/src/libvis/image_display.cc
  libvis/src/libvis/image_display.h
  libvis/src/libvis/image_display_qt_widget.cc
  libvis/src/libvis/image_display_qt_widget.h
  libvis/src/libvis/image_display_qt_window.cc
  libvis/src/libvis/image_display_qt_window.h
  libvis/src/libvis/image_frame.h
  libvis/src/libvis/image_io.cc
  libvis/src/libvis/image_io.h
  libvis/src/libvis/image_io_libpng.cc
  libvis/src/libvis/image_io_libpng.h
  libvis/src/libvis/image_io_netpbm.cc
  libvis/src/libvis/image_io_netpbm.h
  libvis/src/libvis/image_io_qt.cc
  libvis/src/libvis/image_io_qt.h
  libvis/src/libvis/libvis.cc
  libvis/src/libvis/libvis.h
  libvis/src/libvis/lm_optimizer.h
  libvis/src/libvis/lm_optimizer_impl.h
  libvis/src/libvis/lm_optimizer_update_accumulator.h
  libvis/src/libvis/loss_functions.h
  libvis/src/libvis/mesh.h
  libvis/src/libvis/mesh_opengl.h
  libvis/src/libvis/opengl.cc
  libvis/src/libvis/opengl.h
  libvis/src/libvis/opengl_context.cc
  libvis/src/libvis/opengl_context.h
  libvis/src/libvis/opengl_context_glx.cc
  libvis/src/libvis/opengl_context_glx.h
  libvis/src/libvis/opengl_context_qt.cc
  libvis/src/libvis/opengl_context_qt.h
  libvis/src/libvis/patch_match_stereo.cc
  libvis/src/libvis/patch_match_stereo.h
  libvis/src/libvis/point_cloud.h
  libvis/src/libvis/point_cloud_opengl.h
  libvis/src/libvis/qt_thread.cc
  libvis/src/libvis/qt_thread.h
  libvis/src/libvis/render_display.cc
  libvis/src/libvis/render_display.h
  libvis/src/libvis/render_window.cc
  libvis/src/libvis/render_window.h
  libvis/src/libvis/render_window_qt.cc
  libvis/src/libvis/render_window_qt.h
  libvis/src/libvis/render_window_qt_opengl.cc
  libvis/src/libvis/render_window_qt_opengl.h
  libvis/src/libvis/renderer.cc
  libvis/src/libvis/renderer.h
  libvis/src/libvis/rgbd_video.h
  libvis/src/libvis/rgbd_video_io_tum_dataset.h
  libvis/src/libvis/shader_program_opengl.cc
  libvis/src/libvis/shader_program_opengl.h
  libvis/src/libvis/sophus.h
  libvis/src/libvis/statistics.h
  libvis/src/libvis/timing.cc
  libvis/src/libvis/timing.h
  libvis/src/libvis/util.h
  
  ${GENERATED_HEADERS}
  libvis/resources/resources.qrc
)
if(VULKAN_FOUND)
  set(LIBVIS_FILES
    libvis/src/libvis/render_window_qt_vulkan.cc
    libvis/src/libvis/render_window_qt_vulkan.h
    libvis/src/libvis/vulkan.cc
    libvis/src/libvis/vulkan.h
    ${LIBVIS_FILES}
  )
endif()

add_library(libvis SHARED
  ${LIBVIS_FILES}
)
set_property(TARGET libvis PROPERTY FOLDER "libvis")

target_compile_options(libvis PRIVATE
  "${LIBVIS_WARNING_OPTIONS}"
)
if (VULKAN_FOUND)
  target_compile_options(libvis PUBLIC -DLIBVIS_HAVE_VULKAN)
  target_include_directories(libvis PRIVATE "${VULKAN_PATH}/include")
  target_link_directories(libvis PRIVATE "${VULKAN_PATH}/lib")
endif()

if(Qt5Widgets_FOUND)
  target_compile_options(libvis PUBLIC -DLIBVIS_HAVE_QT)
endif()

if(OpenCV_FOUND)
  target_compile_options(libvis PUBLIC -DLIBVIS_HAVE_OPENCV)
endif()

if (CMAKE_CUDA_COMPILER)
  target_compile_options(libvis PUBLIC -DLIBVIS_HAVE_CUDA)  # TODO: is this used anywhere?
endif()

target_include_directories(libvis
  PUBLIC
    ${EIGEN3_INCLUDE_DIR}
	${OpenCV_INCLUDE_DIRS}
    libvis/third_party/sophus
    ./libvis/src
  PRIVATE
    ${Boost_INCLUDE_DIR}
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
    libvis/third_party
    ${GLEW_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Opengl_INCLUDE_DIRS}
    ${Qt5Opengl_INCLUDE_DIRS}
)
# Using the static version of libpng hopefully ensures that we won't get a
# totally outdated version at runtime, resulting in an error like:
# libpng warning: Application was compiled with png.h from libpng-1.6.27                                                               
# libpng warning: Application  is  running with png.c from libpng-1.2.50                                                               
# libpng error: Incompatible libpng version in application and library
# TODO: However, it does not seem to work together with Qt
target_link_libraries(libvis
  PUBLIC
    loguru
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Opengl_LIBRARIES}
    ${Qt5Core_LIBRARIES}
  PRIVATE
    ${Boost_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARY}
    ${Qt5X11Extras_LIBRARIES}
    png  #_static
    #z
    Threads::Threads
)
if(VULKAN_FOUND)
  target_link_libraries(libvis PRIVATE
    vulkan
  )
endif()


# libvis optional library: libvis_external_io.
# Contains I/O functions for less common external formats such as MeshLab
# project files.
add_library(libvis_external_io SHARED
  libvis/src/libvis/external_io/colmap_model.cc
  libvis/src/libvis/external_io/colmap_model.h
  libvis/src/libvis/external_io/meshlab_project.cc
  libvis/src/libvis/external_io/meshlab_project.h
  
  libvis/third_party/tinyxml2/tinyxml2.cpp
)
set_property(TARGET libvis_external_io PROPERTY FOLDER "libvis")
target_compile_options(libvis_external_io PRIVATE
  "${LIBVIS_WARNING_OPTIONS}"
)
target_link_libraries(libvis_external_io PUBLIC
  ${Boost_LIBRARIES}
  libvis
)
target_include_directories(libvis_external_io PUBLIC
  ${Boost_INCLUDE_DIR}
  libvis/third_party/
)


# libvis optional library: libvis_cuda.
# Contains CUDA functionality, which is only useful with NVIDIA graphics cards.
if (CMAKE_CUDA_COMPILER)
  add_library(libvis_cuda SHARED
    libvis/src/libvis/cuda/cuda_auto_tuner.h
    libvis/src/libvis/cuda/cuda_buffer.cu
    libvis/src/libvis/cuda/cuda_buffer.cuh
    libvis/src/libvis/cuda/cuda_buffer.h
    libvis/src/libvis/cuda/cuda_buffer_inl.h
    libvis/src/libvis/cuda/cuda_matrix.cuh
    libvis/src/libvis/cuda/cuda_unprojection_lookup.cuh
    libvis/src/libvis/cuda/cuda_unprojection_lookup.h
    libvis/src/libvis/cuda/cuda_util.h
    libvis/src/libvis/cuda/patch_match_stereo.cc
    libvis/src/libvis/cuda/patch_match_stereo.cu
    libvis/src/libvis/cuda/patch_match_stereo.cuh
    libvis/src/libvis/cuda/patch_match_stereo.h
    libvis/src/libvis/cuda/pixel_corner_projector.cuh
    libvis/src/libvis/cuda/pixel_corner_projector.h
  )
  set_property(TARGET libvis_cuda PROPERTY FOLDER "libvis")
  target_include_directories(libvis_cuda PUBLIC
    ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
    ${OpenCV_INCLUDE_DIRS}
  )
  target_compile_options(libvis_cuda PRIVATE
    "${LIBVIS_WARNING_OPTIONS}"
  )
  target_link_libraries(libvis_cuda PUBLIC
    libvis
  )
  target_compile_options(libvis_cuda PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:-use_fast_math>
    $<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>
  )
endif()


################################################################################
# Applications.

add_subdirectory(applications)


################################################################################
# Tests.

add_executable(libvis_test
  libvis/src/libvis/test/camera.cc
  libvis/src/libvis/test/dlt.cc
  libvis/src/libvis/test/geometry.cc
  libvis/src/libvis/test/image.cc
  libvis/src/libvis/test/image_cache.cc
  libvis/src/libvis/test/lm_optimizer.cc
  libvis/src/libvis/test/main.cc
  libvis/src/libvis/test/point_cloud.cc
  libvis/src/libvis/test/util.cc
)
set_property(TARGET libvis_test PROPERTY FOLDER "libvis")

find_library(CUDBLASLIB_ABS_PATH
             NAMES cublas.lib
             PATHS "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.1/lib/x64"
             NO_DEFAULT_PATH)
if(NOT CUDBLASLIB_ABS_PATH)
  message(FATAL_ERROR "cublas.lib not found ${CUDA_TOOLKIT_ROOT_DIR} path: ${CUDBLASLIB_ABS_PATH}")
endif()

message(STATUS "CUDBLASLIB_ABS_PATH ${CUDBLASLIB_ABS_PATH}")
target_link_libraries(libvis_test PRIVATE
  gtest
  Threads::Threads
  libvis
  #cublas  # TODO: currently required because of lm_optimizer.h. That should not be necessary.
  ${CUDBLASLIB_ABS_PATH}
)

message(STATUS "CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")
target_include_directories(libvis_test PRIVATE
  ${gtest_SOURCE_DIR}/include
  #${gtest_SOURCE_DIR}
  ${OpenCV_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}  # TODO: currently required because of lm_optimizer.h. That should not be necessary.
)
add_test(libvis_test
  libvis_test
)


################################################################################
# Include a global summary of found/not found pkgs. See:
# https://cmake.org/cmake/help/latest/module/FeatureSummary.html

include(FeatureSummary)
feature_summary(WHAT ALL)

#add_custom_command(TARGET libvis POST_BUILD 
 #   COMMAND Qt5::windeployqt
 #   ARGS $<TARGET_FILE:libvis>
#)