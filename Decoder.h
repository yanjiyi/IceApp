#ifndef DECODER_HEADER
#define DECODER_HEADER
#include "IObject.h"
#include <vector>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>

class Decoder : public IObject
{
	public:
		Decoder(IObject * parent = nullptr);
		~Decoder();

		bool OpenMedia(const char* Path);
		
	protected:
		std::vector<AVFrame> decode_frame;
		AVFormatContext * fmt_ctx = nullptr;		
};

#endif
