#include "App.h"
#include <GL/glew.h>

App theApp;

App::App(IObject * parent) : IceApp(parent)
{

}

App::~App()
{

}

void App::preper()
{

}

void App::resize()
{
	int width,height;
	SDL_GetWindowSize(m_ptrWnd, &width, &height);
	glViewport(0, 0, width, height);
}

void App::paint(float frametime)
{

}

void App::doevent(SDL_Event* e)
{}
