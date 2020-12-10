
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



# Adds the shader folder to the bin directory, making its content available for out program to read.
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Adds the resource folder to the bin directory, making the content available for our program to read
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)



add_executable(autumn2020
	main.cpp
	src/Environment/Movobj.h
	src/Environment/Movobj.cpp
	
	
	src/CoreGL/VertexBuffer.h
	src/CoreGL/VertexBuffer.cpp
	src/CoreGL/IndexBuffer.h
	"src/CoreGL/IndexBuffer.cpp" 
	src/CoreGL/VertexArray.h 
	src/CoreGL/VertexArray.cpp
	src/CoreGL/VertexBufferLayout.h 
	src/CoreGL/Renderer.h
	"src/CoreGL/Renderer.cpp"
	src/CoreGL/Texture.h
	src/CoreGL/Texture.cpp
	 
	src/Engine/Camera.h
	src/CoreGL/Shader.h
	src/CoreGL/stb_image.h
	src/Engine/mesh.h
	"src/Environment/Terrain.h"
	src/Environment/Terrain.cpp
	src/Engine/model.h   "src/Environment/Trees.h" "src/Environment/Trees.cpp" "src/Engine/Camera.cpp" "src/Environment/Global.h" "src/Environment/Aerialobj.h" "src/Environment/Aerialobj.cpp")


target_compile_definitions(autumn2020 PRIVATE GLEW_STATIC)

target_link_libraries(autumn2020
  PRIVATE
  libglew_static
  assimp
  glfw
  glm
  OpenGL::GL)