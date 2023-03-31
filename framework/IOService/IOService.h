#ifndef IFRAMEWORK_IOSERVICE_HEADER
#define IFRAMEWORK_IOSERVICE_HEADER

#include <vector>

class IOService {
public:
	static std::vector<char> ReadFile(const char* Path);
};

#endif

