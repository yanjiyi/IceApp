#include "IOService.h"
#include <fstream>
#include <stdexcept>
#include <cstring>

std::vector<char> IOService::ReadFile(const char *Path)
{
	std::ifstream io;
	io.open(Path,std::ios::in|std::ios::binary|std::ios::ate);
	if(!io.is_open())
	{
		char msg[1024];
		memset(msg,0,sizeof(msg));

		sscanf(msg,"IOService Exception : Failed Open %s !",Path);

		throw std::runtime_error(msg);		
	}

	size_t fileSize = io.tellg();
	io.seekg(0, std::ios::beg);

	std::vector<char> buffer(fileSize+1);
	memset(&buffer,0,fileSize+1);
	io.read(&buffer[0], fileSize);
	io.close();

	return buffer;
}
