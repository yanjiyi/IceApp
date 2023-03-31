#ifndef APP_HEADER
#define APP_HEADER
#include "IceApp.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

class App : public IceApp
{
	public:
		App(IObject * parent = nullptr);
		~App();

	protected:
		void preper() override;
		void resize() override;
		void paint(float frametime) override;
		void doevent(SDL_Event* e) override;
	protected:
		Camera camera;
};

#endif
