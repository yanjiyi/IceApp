#include "Shader.h"
#include "IOService.h"
#include <vector>

extern "C"{
#include <SDL2/SDL.h>
}

Shader::Shader(IObject* parent) : IObject(parent)
{

}

Shader::~Shader()
{
	if(_shader_program_id>0)
		glDeleteProgram(_shader_program_id);
}

bool Shader::CompileFromSource(const char *vertexSource, const char *fragmentSource)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader,1,(const GLchar* const*)&vertexSource,NULL);
	glShaderSource(fragmentShader,1,(const GLchar* const*)&fragmentSource,NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	GLint isCompiled = GL_FALSE;
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&isCompiled);
	if(GL_FALSE == isCompiled)	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH,&maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader,maxLength,&maxLength,&infoLog[0]);

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OGL - Vertex Shader", (char*)&infoLog[0], nullptr);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return false;
	}

	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&isCompiled);
	if(GL_FALSE == isCompiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader,maxLength,&maxLength,&infoLog[0]);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OGL - Fragment Shader", (char*)&infoLog[0], nullptr);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return false;

	}

	GLuint program = glCreateProgram();
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);

	glLinkProgram(program);

	GLint isLinked = GL_FALSE;
	glGetProgramiv(program,GL_LINK_STATUS,&isLinked);
	if(GL_FALSE == isLinked)
	{
		GLint maxLength = 0;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program,maxLength,&maxLength,&infoLog[0]);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OGL - Shader Linkging", (char*)&infoLog[0], nullptr);

		glDetachShader(program,vertexShader);
		glDetachShader(program,fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		return false;
	}

	if(_shader_program_id>0)
		glDeleteProgram(_shader_program_id);

	_shader_program_id = program;

	glDetachShader(program,vertexShader);
	glDetachShader(program,fragmentShader);
	return true;	
}

bool Shader::CompileFromFile(const char *vertexPath, const char *fragmentPath)
{
	auto vertex_buffer = IOService::ReadFile(vertexPath);
	auto fragment_buffer = IOService::ReadFile(fragmentPath);

	return CompileFromSource(vertex_buffer.data(), fragment_buffer.data());
}

bool Shader::BuildFromBinrary(const char *vertexSpirv, size_t vertexSize, const char *fragmentSpriv, size_t fragmentSize)
{
	return BuildFromBinrary(vertexSpirv, vertexSize, fragmentSpriv, fragmentSize,"main");
}

bool Shader::BuildFromBinrary(const char *vertexSpirv, size_t vertexSize, const char *fragmentSpriv, size_t fragmentSize, const char* entryPoint)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderBinary(1,&vertexShader,GL_SHADER_BINARY_FORMAT_SPIR_V,vertexSpirv,vertexSize);

	glSpecializeShader(vertexShader,(const GLchar*)entryPoint,0,nullptr,nullptr);

	// Specialization is equivalent to compilation.
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		// Use the infoLog as you see fit.
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OGL - Vertex Shader", (char*)&infoLog[0], nullptr);
		// In this simple program, we'll just leave
		return false;
	}

	glShaderBinary(1,&fragmentShader,GL_SHADER_BINARY_FORMAT_SPIR_V,fragmentSpriv,fragmentSize);
	glSpecializeShader(fragmentShader,(const GLchar*)entryPoint,0,nullptr,nullptr);

	// Specialization is equivalent to compilation.
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		// Use the infoLog as you see fit.
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OGL - Fragment Shader", (char*)&infoLog[0], nullptr);

		// In this simple program, we'll just leave
		return false;
	}

	GLuint program = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// Use the infoLog as you see fit.
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OGL - Shader Linking", (char*)&infoLog[0], nullptr);

		// In this simple program, we'll just leave
		return false;
	}

	if(_shader_program_id>0)
		glDeleteProgram(_shader_program_id);

	_shader_program_id = program;

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	return true;
}

bool Shader::BuildFromBinraryFile(const char *vertexPath, const char *fragmentPath)
{
	return BuildFromBinraryFile(vertexPath, fragmentPath, "main");
}

bool Shader::BuildFromBinraryFile(const char *vertexPath, const char *fragmentPath, const char *entryPoint)
{
	std::vector<char> vertexBinraries = IOService::ReadFile(vertexPath);
	std::vector<char> fragmentBinraries = IOService::ReadFile(fragmentPath);

	return BuildFromBinrary(vertexBinraries.data(), vertexBinraries.size(), fragmentBinraries.data(), fragmentBinraries.size(),entryPoint);
}

void Shader::Apply()
{
	if(_shader_program_id>0)
		glUseProgram(_shader_program_id);
}

void Shader::Revoke()
{
	glUseProgram(0);
}

GLint Shader::getUniformLocation(const char *name)
{
	if(_shader_program_id<=0)
		return -1;

	return glGetUniformLocation(_shader_program_id,name);
}

void Shader::setUniform1i(GLint location, int value)
{
	if(location < 0)
		return;

	glUniform1i(location,value);	
}

void Shader::setUniform1i(const char *name, int value)
{
	GLint location = getUniformLocation(name);
	if(location<0)
		return;

	setUniform1i(location,value);
}

void Shader::setUniform1f(GLint location, float value)
{
	if(location<0)
		return;

	glUniform1f(location,value);
}

void Shader::setUniform1f(const char *name, float value)
{
	GLint location = getUniformLocation(name);
	if(location<0)
		return;

	return setUniform1f(location,value);
}

void Shader::setUniform2f(GLint location, float *value)
{
	if(location<0)
		return;

	glUniform2f(location,value[0],value[1]);
}

void Shader::setUniform2f(const char *name, float *value)
{
	GLint location = getUniformLocation(name);
	if(location<0)
		return;

	setUniform2f(location,value);
}

void Shader::setUniform3f(GLint location, float *value)
{
	if(location<0)
		return;

	glUniform3f(location,value[0],value[1],value[2]);
}

void Shader::setUniform3f(const char *name, float *value)
{
	GLint location = getUniformLocation(name);
	if(location<0)
		return;

	setUniform3f(location,value);
}

void Shader::setUniform4f(GLint location, float *value)
{
	if(location<0)
		return;

	glUniform4f(location,value[0],value[1],value[2],value[3]);
}

void Shader::setUniform4f(const char *name, float *value)
{
	GLint location = getUniformLocation(name);
	if(location<0)
		return;

	setUniform4f(location, value);
}
