#include "Texture.h"

extern "C"{
#include <SOIL/SOIL.h>
}

#include <unordered_map>
#include <string>

class TextureCache {
	public:
		static std::unordered_map<std::string, GLuint> texture_cache;	
};

std::unordered_map<std::string, GLuint> TextureCache::texture_cache;

class TextureManager {
	public:
		~TextureManager()
		{
			for(auto& iter : _texture_cache.texture_cache)
			{
				glDeleteTextures(1,&iter.second);
			}
		}

		bool FindTexture(std::string path,GLuint* texture_id)
		{
			auto iter = _texture_cache.texture_cache.find(path);
			if(iter!=_texture_cache.texture_cache.end())
			{
				*texture_id = iter->second;
				return true;
			}

			return false;
		}

		void InsertTexture(std::string path,GLuint texture_id)
		{
			_texture_cache.texture_cache.insert(std::pair<std::string,GLuint>(path,texture_id));
		}

		static TextureCache _texture_cache;
};

TextureCache TextureManager::_texture_cache;

TextureManager _texture_manager;

Texture::Texture(IObject * parent) : IObject(parent)
{

}

Texture::~Texture()
{
}

bool Texture::LoadFromFile2D(const char *Path)
{
	GLuint _pre_texture_id;

	if(_texture_manager.FindTexture(std::string(Path), &_pre_texture_id))
	{
		_texture_id = _pre_texture_id;
		return true;
	}

	glGenTextures(1,&_pre_texture_id);
	int width,height,channels;

	unsigned char * ptrData = SOIL_load_image(Path, &width, &height, &channels, SOIL_LOAD_RGBA);

	if(!ptrData)
		return false;

	glBindTexture(GL_TEXTURE_2D,_pre_texture_id);
	glTexImage3D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,0,GL_RGBA,GL_UNSIGNED_BYTE,ptrData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	_texture_manager.InsertTexture(std::string(Path), _pre_texture_id);

	_texture_id = _pre_texture_id;

	SOIL_free_image_data(ptrData);
	return true;
}


