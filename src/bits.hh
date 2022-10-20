#ifndef YCRAFT_BITS_HH
#define YCRAFT_BITS_HH

#include "_components.hh"

namespace Bits {
	std::vector <uint8_t> Split16(uint16_t integer);
	uint16_t              Join16(std::vector <uint8_t> bytes);
}

#endif
