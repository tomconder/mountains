cmake_minimum_required(VERSION 3.20)

project(mountains CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS TRUE)

# Set a default build type if none was specified
# See https://blog.kitware.com/cmake-and-the-default-build-type/ for details.
set(default_build_type "Release")
if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "Debug")
endif ()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

add_subdirectory(3rdparty/glm)

option(BUILD_SHARED_LIBS OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_NO_EXPORT ON)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)

option(ASSIMP_IGNORE_GIT_HASH ON)
option(ASSIMP_INSTALL OFF)

#option(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT FALSE)
#option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE)
#option(ASSIMP_BUILD_OBJ_IMPORTER FALSE)

add_subdirectory(3rdparty/assimp)

include_directories(
        3rdparty/assimp/include/
        3rdparty/easyloggingpp/src
        3rdparty/glad/include/
        src)

file(GLOB HEADER_FILES LIST_DIRECTORIES false
        3rdparty/easyloggingpp/src/easylogging++.h
        src/*.h
        src/core/*.h
        src/platform/opengl/*.h
        src/renderer/*.h)

file(GLOB SHADERS LIST_DIRECTORIES false
        src/shaders/*.frag
        src/shaders/*.vert)

file(GLOB ASSETS LIST_DIRECTORIES false assets/*)

if (EMSCRIPTEN)
  message(STATUS "Emscripten: Building for Emscripten")

  file(GLOB SOURCE_FILES LIST_DIRECTORIES false
          3rdparty/easyloggingpp/src/easylogging++.cc
          3rdparty/glad/src/gles2.c
          src/*.cpp
          src/core/*.cpp
          src/platform/opengl/*.cpp
          src/renderer/*.cpp)

  include_directories(3rdparty/glm)

  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_FREETYPE=1")

  add_executable(mountains ${SOURCE_FILES} ${HEADER_FILES})

  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/html)
  set(ASSETS_BUILD_PATH ${PROJECT_SOURCE_DIR}/build)

  set(TARGET_LINK_PROPERTIES "-std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1")
  set(TARGET_LINK_PROPERTIES "${TARGET_LINK_PROPERTIES} -s USE_SDL=2 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")
  set(TARGET_LINK_PROPERTIES "${TARGET_LINK_PROPERTIES} -s USE_SDL_IMAGE=2 -s \"SDL2_IMAGE_FORMATS=['jpg','png']\"")
  set(TARGET_LINK_PROPERTIES "${TARGET_LINK_PROPERTIES} -s USE_FREETYPE=1")
  set(TARGET_LINK_PROPERTIES "${TARGET_LINK_PROPERTIES} --preload-file ${ASSETS_BUILD_PATH}/assets@assets")
  set_target_properties(mountains PROPERTIES LINK_FLAGS ${TARGET_LINK_PROPERTIES})

  target_link_libraries(mountains assimp)

  add_custom_command(
          TARGET mountains PRE_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets ${ASSETS_BUILD_PATH}/assets
          COMMENT "Copying assets" VERBATIM
          DEPENDS ${ASSETS})

  add_custom_command(
          TARGET mountains PRE_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/src/shaders ${ASSETS_BUILD_PATH}/assets/shaders
          COMMENT "Copying shaders" VERBATIM
          DEPENDS ${SHADERS})
else (EMSCRIPTEN)
  file(GLOB SOURCE_FILES LIST_DIRECTORIES false
          3rdparty/easyloggingpp/src/easylogging++.cc
          3rdparty/glad/src/gl.c
          src/*.cpp
          src/core/*.cpp
          src/platform/opengl/*.cpp
          src/renderer/*.cpp)

  set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
  find_package(SDL2 REQUIRED)
  find_package(SDL2_IMAGE REQUIRED)
  find_package(Freetype REQUIRED)
  find_package(OpenGL REQUIRED)

  # for debugging
  # include(CMakePrintHelpers)
  # cmake_print_variables(SDL2_INCLUDE_DIR)
  # cmake_print_variables(SDL2_LIBRARY)

  add_executable(mountains ${SOURCE_FILES} ${HEADER_FILES})

  target_include_directories(mountains PUBLIC 3rdparty/glm)
  target_include_directories(mountains PUBLIC ${FREETYPE_INCLUDE_DIRS})
  target_include_directories(mountains PUBLIC ${OPENGL_INCLUDE_DIR})
  target_include_directories(mountains PUBLIC ${SDL2_IMAGE_INCLUDE_DIR})
  target_include_directories(mountains PUBLIC ${SDL2_INCLUDE_DIR})

  target_link_libraries(mountains ${FREETYPE_LIBRARIES})
  target_link_libraries(mountains ${OPENGL_LIBRARIES})
  target_link_libraries(mountains ${SDL2_IMAGE_LIBRARY})
  target_link_libraries(mountains ${SDL2_LIBRARY})
  target_link_libraries(mountains assimp)

  # use ASan to find memory errors. See https://github.com/google/sanitizers/wiki/AddressSanitizer
  #  set(TARGET_LINK_PROPERTIES "-std=c++17 -O2 -fsanitize=address -fno-omit-frame-pointer")
  #  set_target_properties(mountains PROPERTIES LINK_FLAGS ${TARGET_LINK_PROPERTIES})

  set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
  set(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

  add_custom_command(
          TARGET mountains POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:mountains>/assets
          COMMENT "Copying assets" VERBATIM
          DEPENDS ${ASSETS})

  add_custom_command(
          TARGET mountains POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:mountains>/assets/shaders
          COMMENT "Copying shaders" VERBATIM
          DEPENDS ${SHADERS})
endif (EMSCRIPTEN)
