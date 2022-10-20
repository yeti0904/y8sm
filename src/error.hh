#ifndef Y8SM_ERROR_HH
#define Y8SM_ERROR_HH

#include "_components.hh"

namespace Errors {
	void Expected(std::string expected, std::string fname, size_t line, std::string got);
}

#endif