#include "IceApp.h"
#include "SDL2/SDL.h"
#include <GL/glew.h>

namespace {
	IceApp * gApp = nullptr;
}

IceApp::IceApp(IObject * parent) : IObject(parent)
{
	if(gApp!=nullptr)
		delete gApp;

	gApp = this;
}

IceApp::~IceApp() {}

IceApp* IceApp::GetApp() { return gApp; }

int IceApp::Run()
{
	preper();

	bool run = true;
	SDL_Event event;

	Uint64 start = SDL_GetPerformanceCounter();

	while(run)
	{
		Uint64 end = SDL_GetPerformanceCounter();
		float frametime = (end - start) / (float)SDL_GetPerformanceFrequency();

		start = end;

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				run = false;
				break;
			}

			if(event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(m_ptrWnd))
			{
				resize();
			}

			doevent(&event);
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f,0.0f,0.0f,1.0f);

		paint(frametime);

		SDL_GL_SwapWindow(m_ptrWnd);
	}

	return statusCode;
}

void IceApp::SetHookWindow(SDL_Window *ptrWnd)
{
	this->m_ptrWnd = ptrWnd;
}
