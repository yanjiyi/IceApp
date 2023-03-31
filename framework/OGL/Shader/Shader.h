#ifndef IFRAMEWORK_GL_SHADER_HEADER
#define IFRAMEWORK_GL_SHADER_HEADER

#include "IObject.h"
#include <GL/glew.h>

class Shader : public IObject {
public:
	Shader(IObject* parent = nullptr);
	~Shader();
	
	bool CompileFromFile(const char* vertexPath,const char* fragmentPath);
	bool CompileFromSource(const char* vertexSource,const char* fragmentSource);

	bool BuildFromBinrary(const char* vertexSpirv,size_t vertexSize,const char* fragmentSpriv,size_t fragmentSize);
	bool BuildFromBinrary(const char* vertexSpirv,size_t vertexSize,const char* fragmentSpriv,size_t fragmentSize,const char* entryPoint);
	bool BuildFromBinraryFile(const char* vertexPath,const char* fragmentPath);
	bool BuildFromBinraryFile(const char* vertexPath,const char* fragmentPath,const char* entryPoint);

	GLint getUniformLocation(const char* name);

	void setUniform1f(GLint location,float value);
	void setUniform2f(GLint location,float *value);
	void setUniform3f(GLint location,float *value);
	void setUniform4f(GLint location,float *value);
	void setUniform1i(GLint location,int value);
	void setUniform1f(const char* name,float value);
	void setUniform2f(const char* name,float *value);
	void setUniform3f(const char* name,float *value);
	void setUniform4f(const char* name,float *value);
	void setUniform1i(const char* name,int value);

	void Apply();
	void Revoke();
protected:
	GLuint _shader_program_id = 0;
};

#endif
