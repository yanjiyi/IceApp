cmake_minimum_required(VERSION 3.5)

project(IceApp)

include_directories(
	"framework"
	 "framework/IObject"
	 "framework/IOService"
	"framework/OGL"
	"framework/OGL/Camera"
	"framework/OGL/Shader"
	"framework/OGL/Texture"
	"application"
)

set(FRAMEWORK_SRC 
	framework/IObject/IObject.h
	framework/IObject/IObject.cpp
	framework/IOService/IOService.h
	framework/IOService/IOService.cpp
	framework/OGL/Camera/Camera.h
	framework/OGL/Camera/Camera.cpp
	framework/OGL/Shader/Shader.h
	framework/OGL/Shader/Shader.cpp
	framework/OGL/Texture/Texture.h
	framework/OGL/Texture/Texture.cpp
)

set(APP_SRC
	application/IceApp.h
	application/IceApp.cpp
)

add_executable(IceApp ${FRAMEWORK_SRC} ${APP_SRC} App.h App.cpp main.cpp)
target_link_libraries(IceApp SDL2 GLEW GL X11 GLU SOIL)
