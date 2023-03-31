#ifndef ICEAPP_HEADER
#define ICEAPP_HEADER
#include "IObject.h"
#include <SDL2/SDL.h>

class IceApp : public IObject
{
	public:
		IceApp(IObject * parent = nullptr);
		~IceApp();

		static IceApp* GetApp();

		void SetHookWindow(SDL_Window * ptrWnd);

		int Run();

	protected:
		virtual void preper() = 0;
		virtual void paint(float frametime) = 0;
		virtual void resize() = 0;
		virtual void doevent(SDL_Event *e) = 0;

		int statusCode = 0;
		SDL_Window* m_ptrWnd = nullptr;
};

#endif
