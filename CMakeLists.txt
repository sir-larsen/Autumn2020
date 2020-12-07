
cmake_minimum_required(VERSION 3.12)


project(autumn2020)


find_package(OpenGL REQUIRED)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



add_subdirectory(external/glfw-3.3.2)


# (https://github.com/Perlmint/glew-cmake)
add_subdirectory(external/glew-cmake-2.1.0)


add_subdirectory(external/glm-0.9.9.8)

add_subdirectory(external/assimp-5.0.0)



# Adds the levels folder to the bin directory, making the content available for our program to read
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/levels DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Adds the shader folder to the bin directory, making its content available for out program to read.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Adds the resource folder to the bin directory, making the content available for our program to read
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)



add_executable(autumn2020
	main.cpp
	src/ScenarioLoader.h
	src/ScenarioLoader.cpp
	src/VertexBuffer.h
	src/VertexBuffer.cpp
	src/IndexBuffer.h
	src/IndexBuffer.cpp 
	src/VertexArray.h 
	src/VertexArray.cpp
	src/VertexBufferLayout.h 
	src/Renderer.h
	src/Renderer.cpp
	src/Texture.h
	src/Texture.cpp
	src/Shaderr.h 
	src/Camera.h
	src/Shader.h
	src/stb_image.h
	src/mesh.h
	"src/Terrain.h"
	src/Terrain.cpp
	src/model.h "src/Ghost3D.h" "src/Ghost3D.cpp")


target_compile_definitions(autumn2020 PRIVATE GLEW_STATIC)

target_link_libraries(autumn2020
  PRIVATE
  libglew_static
  assimp
  glfw
  glm
  OpenGL::GL)