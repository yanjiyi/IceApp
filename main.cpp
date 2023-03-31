#include "IceApp.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

int main(int,char**)
{
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
	{
		std::cerr << SDL_GetError() << std::endl;
		return 1;		
	}

	SDL_Window * ptrWindow = SDL_CreateWindow("IceUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_OPENGL);
	if(!ptrWindow)
	{
		std::cerr << SDL_GetError() << std::endl;
	    return 1;	
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(ptrWindow);
	GLenum error = glewInit();
	if(GLEW_OK!=error)
	{
		std::cerr << (char*)glewGetErrorString(error) << std::endl;
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(ptrWindow);
		SDL_Quit();
		return 1;
	}

	IceApp * pApp = IceApp::GetApp();
	if(!pApp)
	{
		std::cerr <<"No App Instance Found!" << std::endl;
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(ptrWindow);
		SDL_Quit();
		return 1;
	}

	pApp->SetHookWindow(ptrWindow);
	int retCode = pApp->Run();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(ptrWindow);
	SDL_Quit();

	return retCode;
}
