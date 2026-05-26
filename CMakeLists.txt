cmake_minimum_required(VERSION 3.16)

project(FunctionVis)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIB_FOLDER_NAME Libraries)
set(INCLUDE_FOLDER_NAME Include)
set(LIB_LINK_FOLDER_NAME Lib)
set(FETCHED_CONTENT_FOLDER_NAME External)

#cmake varibales:
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/Run)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Run)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/${LIB_FOLDER_NAME}/${FETCHED_CONTENT_FOLDER_NAME})
set(BUILD_STATIC_EXECUTEABLE OFF)


#fetchExternals

# === GLFW ===
FetchContent_Declare(
   glfw
   GIT_REPOSITORY https://github.com/glfw/glfw.git
   GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# === GLM ===
FetchContent_Declare(
   glm
   GIT_REPOSITORY https://github.com/g-truc/glm.git
   GIT_TAG 0.9.9.8
)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm)

# === LuaSol ===
FetchContent_Declare(
   sol2
   GIT_REPOSITORY https://github.com/ThePhD/sol2.git
   GIT_TAG v3.5.0
)

FetchContent_MakeAvailable(sol2)

FetchContent_Declare(
   lua
   GIT_REPOSITORY https://github.com/walterschell/Lua.git
   GIT_TAG        v5.4.5
)
set(LUA_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(LUA_BUILD_BINARY   OFF CACHE BOOL "" FORCE)
set(LUA_BUILD_COMPILER OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(lua)

#vcpkg:
find_package(OpenGL REQUIRED)

add_library(glad STATIC
   ${LIB_FOLDER_NAME}/${LIB_LINK_FOLDER_NAME}/glad.c
)

#includes sources
set(EXTERNAL_INCLUDE_PATHS
   ${litehtml_SOURCE_DIR}/include
)

include_directories(
   ${EXTERNAL_INCLUDE_PATHS}
   ${CMAKE_SOURCE_DIR}/${LIB_FOLDER_NAME}/${INCLUDE_FOLDER_NAME}
)

link_directories(
   ${CMAKE_SOURCE_DIR}/${LIB_FOLDER_NAME}/${LIB_LINK_FOLDER_NAME}
)

add_executable(FunctionVis
   SRC/Display/GlfwDisplay/GlfwDisplay.cpp
   SRC/GraphicsApi/OpenGlApi/GlApi.cpp
   SRC/Generators/Generator.cpp
   SRC/Renderer/Mesh.cpp
   SRC/Renderer/RenderObject.cpp
   SRC/Renderer/Material.cpp
   SRC/Engine/Engine.cpp
   SRC/Engine/LuaEngine.cpp
   main.cpp
)

target_link_libraries(FunctionVis PUBLIC 
   glfw
   glad
   OpenGL::GL
   glm
   sol2 lua_static
)

target_compile_options(FunctionVis PRIVATE -O3)