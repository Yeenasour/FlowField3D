cmake_minimum_required(VERSION 3.10.0)
include(FetchContent)
project(FlowField3D VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

find_package(OpenGL REQUIRED)

find_package(glfw3 3.4 QUIET)
if(NOT glfw3_FOUND)
	if(UNIX)
		set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
		set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
	endif()
	FetchContent_Declare(
		glfw3
		GIT_REPOSITORY	https://github.com/glfw/glfw.git
		GIT_TAG			3.4
	)
	FetchContent_MakeAvailable(glfw3)
endif()

find_package(GLEW QUIET)
if(NOT GLEW_FOUND)

	set(GLEW_BUILD_DOCS OFF CACHE BOOL "Disable documentation generation")
	set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
	set(glew-cmake_BUILD_STATIC ON CACHE BOOL "" FORCE)

	FetchContent_Declare(
		glew
		GIT_REPOSITORY 	https://github.com/Perlmint/glew-cmake.git
		GIT_TAG			glew-cmake-2.2.0
	)
	FetchContent_MakeAvailable(glew)

	set(GLEW_LIB libglew_static)
else()
	set(GLEW_LIB GLEW::GLEW)
endif()

find_package(glm 1.0.1 QUIET)
if(NOT glm_FOUND)
	FetchContent_Declare(
		glm
		GIT_REPOSITORY	https://github.com/g-truc/glm.git
		GIT_TAG			1.0.1
	)
	FetchContent_MakeAvailable(glm)
endif()

file(GLOB IMGUI_SOURCES 
	"include/imgui/*.cpp" 
	"include/imgui/backends/imgui_impl_glfw.cpp" 
	"include/imgui/backends/imgui_impl_opengl3.cpp")

file(GLOB SOURCES "*.cpp" "src/*.cpp" "src/Engine/*.cpp")

add_executable(FlowField3D ${SOURCES} ${IMGUI_SOURCES})

target_compile_definitions(FlowField3D PRIVATE GLEW_STATIC)

target_include_directories(FlowField3D PRIVATE include include/imgui)

target_link_libraries(FlowField3D PRIVATE glfw ${GLEW_LIB} glm::glm OpenGL::GL)