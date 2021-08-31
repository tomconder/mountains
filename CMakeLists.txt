cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

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

include(FetchContent)

# easyloggingpp
FetchContent_Declare(
        easyloggingpp
        GIT_REPOSITORY  https://github.com/amrayn/easyloggingpp.git
        GIT_TAG         49da71da884d1e756a51df617437b87071c2b1a7
)
FetchContent_GetProperties(easyloggingpp)
string(TOLOWER "easyloggingpp" lcName)
if(NOT ${lcName}_POPULATED)
  FetchContent_Populate(easyloggingpp)

  set(CMAKE_CXX_FLAGS "-std=c++11")

  add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR})
endif()

# glm
FetchContent_Declare(
  glm
  GIT_REPOSITORY  https://github.com/g-truc/glm.git
  GIT_TAG         06ed280db4e274fa5e1f36d5ea4f7dfd654ff9b0
)
FetchContent_MakeAvailable(glm)
if(NOT glm_POPULATED)
  FetchContent_Populate(glm)
endif()

# assimp
FetchContent_Declare(
  assimp
  GIT_REPOSITORY  https://github.com/assimp/assimp.git
  GIT_TAG         6841397798051919a23a3fbe1405fd934fb5f39c
)

FetchContent_GetProperties(assimp)
string(TOLOWER "assimp" lcName)
if(NOT ${lcName}_POPULATED)
  FetchContent_Populate(assimp)

  set(BUILD_SHARED_LIBS FALSE CACHE INTERNAL "override Assimp flags" FORCE)

  # turn off assimp options to improve build time and reduce size
  set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "override Assimp flags" FORCE)
  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "override Assimp flags" FORCE)
  set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE INTERNAL "override Assimp flags" FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "override Assimp flags" FORCE)
  set(ASSIMP_DOUBLE_PRECISION ON CACHE INTERNAL "override Assimp flags" FORCE)
  set(ASSIMP_IGNORE_GIT_HASH ON CACHE INTERNAL "override Assimp flags" FORCE)
  set(ASSIMP_INSTALL_PDB FALSE CACHE INTERNAL "override Assimp flags" FORCE )
  set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "override Assimp flags" FORCE)

  add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR})
endif()

file(GLOB SOURCE_FILES LIST_DIRECTORIES false
        src/*.cpp
        src/core/*.cpp
        src/platform/opengl/*.cpp
        src/renderer/*.cpp)

file(GLOB HEADER_FILES LIST_DIRECTORIES false
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

  add_library(glad STATIC EXCLUDE_FROM_ALL 3rdparty/glad/src/gles2.c)
  target_include_directories(glad PUBLIC 3rdparty/glad/include)

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

  target_include_directories(mountains PUBLIC src)
  target_include_directories(mountains PRIVATE ${easyloggingpp_SOURCE_DIR}/src/)

  target_link_libraries(mountains PRIVATE assimp)
  target_link_libraries(mountains PRIVATE ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc)
  target_link_libraries(mountains PRIVATE glad)
  target_link_libraries(mountains PRIVATE glm::glm)

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
else ()
  add_library(glad EXCLUDE_FROM_ALL 3rdparty/glad/src/gl.c)
  target_include_directories(glad PUBLIC 3rdparty/glad/include)

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

  target_include_directories(mountains PRIVATE ${SDL2_IMAGE_INCLUDE_DIR})
  target_include_directories(mountains PRIVATE ${SDL2_INCLUDE_DIR})
  target_include_directories(mountains PRIVATE ${easyloggingpp_SOURCE_DIR}/src/)
  target_include_directories(mountains PUBLIC src)

  target_link_libraries(mountains PRIVATE ${SDL2_IMAGE_LIBRARY})
  target_link_libraries(mountains PRIVATE ${SDL2_LIBRARY})
  target_link_libraries(mountains PRIVATE Freetype::Freetype)
  target_link_libraries(mountains PRIVATE OpenGL::GL)
  target_link_libraries(mountains PRIVATE assimp)
  target_link_libraries(mountains PRIVATE ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc)
  target_link_libraries(mountains PRIVATE glad)
  target_link_libraries(mountains PRIVATE glm::glm)

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
endif ()
