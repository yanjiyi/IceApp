#include "Decoder.h"
#include <thread>

Decoder::Decoder(IObject * parent) : IObject(parent)
{

}

Decoder::~Decoder()
{

}

bool Decoder::OpenMedia(const char *Path)
{
	int ret = avformat_open_input(&fmt_ctx, Path, nullptr, nullptr);
	if(ret < 0)
		return false;


	return true;
}
