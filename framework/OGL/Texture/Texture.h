#ifndef IFRAMEWORK_OGL_TEXTURE_HEADER
#define IFRAMEWORK_OGL_TEXTURE_HEADER


#include "Shader.h"

class Texture : public IObject {
	public:
		Texture(IObject * parent = nullptr);
		~Texture();

		bool LoadFromFile2D(const char* Path);
		bool LoadFromFileCube(const char* x_pos_path,const char* x_neg_path,const char* y_pos_path,const char* z_pos_path,const char* z_neg_path);

		void ApplyToShader(Shader* shader,const char* name,int index);

	protected:
		GLuint _texture_id;
};

#endif
